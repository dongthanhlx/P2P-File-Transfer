#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<time.h>
#include<string.h>
#include<vector>
#include<string>
using namespace std;

int counter = -1;
int server_sockfd, client_sockfd;
int server_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
static int port = 1235;
static int bufferSize = 1024;
static int max_client = 100;
static int opt = 1;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
class Peer{         // class peer
private:
    char *IP;
    int Port;
    bool State = false;
    vector<string> fileList;
    // int socket;
public:
    void setIP(char *ip){
        IP = ip;
    }
    char *getIP(){
        return IP;
    }

    void setPort(int port){
        Port = port;
    }
    int getPort(){
        return Port;
    }

    void setState(bool state){
        State = state;
    }
    bool getState(){
        return State;
    }

    void setfileList(vector<string> _fileList){       
        fileList = _fileList;
    }
    vector<string> getfileList(){
        return fileList;
    }

};
vector<string> allFile;                      // vector luu tru tat ca cac file trong tat ca cac client
vector<string> split(char[]);
vector<Peer> peer;      // luu tru cac peer connected
bool checkFile(vector<string>, string);
void addFile(vector<string>);

void *connection_handler(void *);

void InitSocket();

void Bind();

void Listen();

void Accept();

void createThread();

void show(vector<string>);

char* concat(vector<string> );

bool EsxitFile(Peer , char* );

char* appendIntToChar(char* , int );

int main(){
    InitSocket();
    Bind();
    Listen();
    while(1){
        Accept();
        createThread();
    }
    close(server_sockfd);
    return 0;
}

void *connection_handler(void *server_sockfd){
    int sock = *(int *)server_sockfd;
	struct sockaddr_in cliaddr;
	unsigned int cliaddr_len = sizeof(cliaddr); //
    int nbytes;
    char buffer[bufferSize];
    vector<string> list;
    pthread_detach(pthread_self());
	int res = getpeername(sock, (struct sockaddr*)&cliaddr, &cliaddr_len);
    pthread_mutex_lock(&counter_mutex);
    counter++;
    pthread_mutex_unlock(&counter_mutex);
    while(1){
        char fileList[bufferSize];
        memset(&fileList,'0', sizeof(fileList));
        cout<<"Server waiting..."<<endl;
        bzero(&fileList, sizeof(fileList));
        // receive fileLists from client
        if((nbytes = read(sock, fileList,sizeof(fileList))) <= 0){
            break;
        } 
        cout<<"Received file list."<<fileList<<endl;
        list = split(fileList);  
        cout<<"counter: "<<counter<<endl;
        peer[counter].setfileList(list);    // add ds vao peer[counter]
        addFile(list);            // them danh sach file vao allFile
        char select[bufferSize] ;
        read(sock, select, sizeof(select));     // reveive select from client
        
        if(!strcmp(select,"request-file-list")){
            char temp[bufferSize];
            int i=0, j = allFile.size();
            if(j > 0)   strcpy(temp, (allFile[0]+" ").c_str());     //
            i++;
            while(i < j){
                strcat(temp, (allFile[i]+" ").c_str());
                i++;
            }
            strcat(temp,"\0");
            cout<<"Send: "<<temp<<endl;
            write(sock, temp, strlen(temp)-1);
        }   else if(!strcmp(select, "download")){
            char fileName[bufferSize];
            bzero(&fileName, sizeof(fileName));
            read(sock, fileName, sizeof(fileName));
            cout<<"File name received: "<<endl;
            int i=0, counterFileEsxit = 0;
            int size = peer.size();
            char listIP[bufferSize] ;
            char notFile[bufferSize] = "QUIT";
            bzero(&listIP, sizeof(listIP));
            while(i < size){        // kiem tra cac peer
                Peer p = peer[i];
                if(EsxitFile(p,fileName) && p.getState()){
                    counterFileEsxit++;
                    char *temp;
                    char ip[bufferSize] ;
                    strcpy(ip, p.getIP());
                    int port = p.getPort();
                    temp = appendIntToChar(ip, port);
                    strcat(listIP, temp);
                }
                i++;
            }
            if(counterFileEsxit == 0)   {
                cout<<"FIle not found"<<endl;
                write(sock, notFile, strlen(notFile));
                continue;
            }
            cout<<"List IP: "<<listIP<<endl;
            write(sock, listIP, strlen(listIP));
        }   else if(!strcmp(select, "disconnect")){
            cout<<"Disconnected from client"<<endl;
            peer[counter].setState(false);
            close(sock);
        }
    }
    close(sock);
    return NULL;
}

vector<string> split(char fileList[]){       // tach fileList tu chuoi char sang vector
    vector<string> list;
    char *temp = strtok(fileList, " ");
    while(temp != NULL){
        list.push_back(temp);
        temp = strtok(NULL, " ");
    }
    return list;
}

bool checkFile(vector<string> fileList, string fileName){         //kiem tra xem fileList co trong danh sach fileList khong ???
    int i=0;
    int size = fileList.size();
    while(i<size){
        if(!fileName.compare(fileList[i]))   return true;
        i++;
    }
    return false;
}
void addFile(vector<string> list){                       // them danh sach list vao allFile
    int i=0;
    int size = list.size();
    while(i < size){
        if(checkFile(allFile, list[i]) == false)    allFile.push_back(list[i]);
        i++;
    }
}

void InitSocket(){
    bzero(&server_address, sizeof(server_address));
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_address.sin_port = htons(port);
    server_len = sizeof(server_address);
    setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
}

void Bind(){
    if(bind(server_sockfd, (struct sockaddr*)&server_address, server_len) < 0){
        cout<<"Error in binding"<<endl;
        exit(1);
    }
}

void Listen(){
    if(-1 == listen(server_sockfd, max_client)){
        perror("Listen Error");
    }
    cout<<"Server listen at port "<<port<<endl;
}

void Accept(){
    unsigned int client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
    if(client_sockfd < 0){
        perror("Accept Error");
        exit(1);
    }  
    Peer p;
    peer.push_back(p);
    peer[counter+1].setIP(inet_ntoa(client_address.sin_addr));
    peer[counter+1].setPort(ntohs(client_address.sin_port));
    peer[counter+1].setState(true);
}

void createThread(){
    pthread_t thread_id;
    if(pthread_create(&thread_id, NULL, &connection_handler, (void *)&client_sockfd) < 0){
        perror("Counld not create thread");
        return ;
    }
}

void show(vector<string> text){
    int i = 0;
    int size = text.size();
    cout<<"--------------"<<endl;
    while(i < size){
        cout<<"\t"<<text[i]<<endl;
        i++;
    }
    cout<<"--------------"<<endl;
}

bool EsxitFile(Peer p, char* name){     // kiem tra xem name co ton tai trong Peer p k 
    int i=0;
    vector<string> list = p.getfileList();
    int size = list.size();
    while(i < size){
        if(strcmp(list[i].c_str(), name) == 0)  return true;
        i++;
    }
    return false;
}

char* appendIntToChar(char* text, int num){     // noi int vao char*
    char temp[bufferSize];
    strcat(text, ":");
    sprintf(temp, "%d", num);
    strcat(text, temp);
    strcat(text," \0");
    return text;
}
