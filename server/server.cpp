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

int counter = 0;
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

    void setfileList(vector<string> fileList){       
        fileList = fileList;
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
int main(){
    InitSocket();
    Bind();
    Listen();
    while(1){
        Accept();
        createThread();

        if(counter == 3)    break;
    }
    show(allFile);
    close(client_sockfd);
    return 0;
}

void *connection_handler(void *server_sockfd){
    int sock = *(int *)server_sockfd;
    int nbytes;
    char buffer[bufferSize];
    vector<string> list;
    Peer p;
    p.setIP(inet_ntoa(client_address.sin_addr));
    p.setPort(client_address.sin_port);
    p.setState(true);

    pthread_detach(pthread_self());
    while(1){
        char fileName[bufferSize];
        cout<<"Server waiting..."<<endl;
        bzero(&fileName, sizeof(fileName));
        // receive filenames from client
        if((nbytes = read(sock, fileName,sizeof(fileName))) <= 0){
            break;
        } 
        // store filenames 
        p.setfileList(list);
        // store info peer     
        peer.push_back(p);
        // cout<<"Received: "<<fileName<<endl;
        list = split(fileName);  
        addFile(list);            // them danh sach file vao allFile
        char select[bufferSize] ;
        read(sock, select, sizeof(select));
        if(strcmp(select,"mot") == 0){
            char temp[bufferSize];
            // temp = concat(allFile);
            int i=0, j = allFile.size();
            if(j > 0)   strcpy(temp, (allFile[0]+" ").c_str());
            i++;
            while(i < j){
                strcat(temp, (allFile[i]+" ").c_str());
                i++;
            }
            strcat(temp,"\0");
            cout<<"Send: "<<temp<<endl;
            write(sock, temp, strlen(temp)+1);
        }   else if(strcmp(select, "hai") == 0){
            /*
            Download
            */
        }   else if(strcmp(select, "ba") == 0){
            cout<<"Disconnected from client"<<endl;
            close(sock);
        }

        pthread_mutex_lock(&counter_mutex);         // tach danh sach file 
        counter++;
        pthread_mutex_unlock(&counter_mutex);
        pthread_mutex_destroy(&counter_mutex);
    }
    close(sock);
    return NULL;
}

vector<string> split(char fileList[]){       // tach fileList tu chuoi char sang vector
    vector<string> list;
    char *temp = strtok(fileList, "\n");
    while(temp != NULL){
        list.push_back(temp);
        temp = strtok(NULL, "\n");
    }
    return list;
}

bool checkFile(vector<string> fileList, string fileName){         //kiem tra xem fileName co trong danh sach fileList khong ???
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
}

void createThread(){
    pthread_t thread_id;
    if(pthread_create(&thread_id, NULL, connection_handler, (void *)&client_sockfd) < 0){
        perror("Counld not create thread");
        return ;
    }
    pthread_join(thread_id, NULL);
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

// char[bufferSize] concat(vector<string> list){
//     int i=0;
//     int j = list.size();
//     char* result = NULL;
//     if(j > 0)   strcpy(result, list[0].c_str());
//     i++;
//     while(i < j){
//         strcat(result, list[i].c_str());
//         i++;
//     }
//     result[strlen(result)] = '\0';
//     return result;
// }