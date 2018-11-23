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
using namespace std;

vector<char*> allFile;
    pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
    int server_sockfd, client_sockfd, nbytes, bufferSize = 1024;
    int server_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char quit[] = "QUIT";
    int port = 1236, opt = 1;
    int max_client = 10;

vector<char*> split(char listFile[]){
    vector<char*> list;
    char *temp = strtok(listFile, " ");
    while(temp != NULL){
        list.push_back(temp);
        temp = strtok(NULL, " ");
    }
    return list;
}

class Peer{
private:
    char *IP;
    int Port;
    bool State = false;
    vector<char*> ListFile;
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

    void setListFile(vector<char*> listFile){
        ListFile = listFile;
    }
    vector<char*> getListFile(){
        return ListFile;
    }
};

vector<Peer> peer;

bool checkFile(vector<char*> listFile, char *nameFile){
    int i=0;
    int size = listFile.size();
    cout<<"Check "<<size<<endl;
    while(i<size){
        if(strcmp(listFile[i], nameFile) == 0)  return true;
        i++;
    }
    // do {
    //     if(strcmp(listFile[i], nameFile) == 0)  return true;
    //     i++;
    // }while(i<size);
    return false;
}
void init(){
    bzero(&server_address, sizeof(server_address));
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_address.sin_port = htons(port);
    server_len = sizeof(server_address);
    setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    //bind

    if(bind(server_sockfd, (struct sockaddr*)&server_address, server_len) < 0){
        cout<<"Error in binding";
        return ;
    }

    //listen
    if(-1 == listen(server_sockfd, max_client)){
        perror("Listen Error");
    }
    cout<<"Server listen at port "<<port<<"\n";
}
void addFile(vector<char*> list){
    int i=0;
    int size = list.size();
    cout<<"counter: "<<size<<endl;
    while(i < size){
        if(checkFile(allFile, list[i]) == 0)    allFile.push_back(list[i]);
        // allFile.push_back(list[i]);
        i++;
    }
}

void *connection_handler(void *);

int counter = 0;
int main(){

    init();

    while(1){
        unsigned int client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
        if(client_sockfd < 0){
            perror("Accept Error");
            exit(1);
        }
        

        pthread_t thread_id;
        if(pthread_create(&thread_id, NULL, connection_handler, (void *)&client_sockfd) < 0){
            perror("Counld not create thread");
            return 1;
        }
        // cout<<2<<endl;
        pthread_join(thread_id, NULL);
        // cout<<3<<endl;
        if(counter == 3)    break;
    }
    // cout<<1<<endl;
    int size = allFile.size();
    cout<<"dong thanh "<<size<<endl;
    for(int i=0; i<size; i++){
        cout<<allFile[i]<<endl;
    }
    close(client_sockfd);
    return 0;
}


void *connection_handler(void *server_sockfd){
    int sock = *(int *)server_sockfd;
    // char quit[] = "QUIT";
    int nbytes;
    pthread_detach(pthread_self());
    char buffer[bufferSize];
    char listFile[bufferSize];
    bzero(listFile, sizeof(listFile));
        Peer p ;
        p.setIP(inet_ntoa(client_address.sin_addr));
        p.setPort(ntohs(client_address.sin_port));
        p.setState(true);
        // while() > 0){
            read(client_sockfd, listFile, sizeof(listFile));
            vector<char*> list;
            list = split(listFile);
            p.setListFile(list);
            peer.push_back(p);
            addFile(list);
            int i=0;
            int size = list.size();
            while(i < size){
                cout<<list[i]<<endl;
                i++;
            }
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
        pthread_mutex_destroy(&counter_mutex);
        close(sock);
        return NULL;
}