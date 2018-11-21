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


// vector<char*> split(char listFile[]){
//     char *index = strtok(listFile," ");
//     vector<char*> temp;
//     while(listFile){
//         temp.push_back(index);
//         index = strtok(NULL," ");
//     }
//     return temp;
// }

int main(){
    int server_sockfd, client_sockfd, nbytes, bufferSize = 1024;
    int server_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char quit[] = "QUIT";
    int port = 1236, opt = 1;
    int max_client = 10;

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
        return 1;
    }

    //listen
    if(-1 == listen(server_sockfd, max_client)){
        perror("Listen Error");
    }
    cout<<"Server listen at port "<<port<<"\n";

    while(1){
        unsigned int client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
        if(client_sockfd < 0){
            perror("Accept Error");
            exit(1);
        }

        while(1){
                cout<<"Server waiting...\n";
                char buffer[bufferSize];
                char listFile[bufferSize];
                int sizeList = 5;
                vector<char*> list;
                if((nbytes = read(client_sockfd, listFile,sizeof(listFile))) <= 0){
                    break;
                }
                cout<<"List file: "<<listFile<<"t\n";
                char *temp = strtok(listFile, " ");
                while(temp != NULL){
                    list.push_back(temp);
                    temp = strtok(NULL, " ");
                }
                for(int i=0; i<list.size(); i++){
                    cout<<list[i]<<"\t";
                }
        }
        close(client_sockfd);
    }
}

