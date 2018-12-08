#ifndef PEER_H
#define PEER_H
#include<vector>
using namespace std;
class Peer{
private:
    char *IP;
    int Port;
    bool State = false;
    vector<char*> ListFile;
public:
    Peer();
    Peer(char* ip, int port);
    void setIP(char* ip);
    char* getIP(){return IP; }

    void setPort(int port);
    int getPort(){return Port; }

    void setState(bool state);
    bool getState(){return State; }

    void setListFile(vector<char*> ListFile);
    vector<char*> getListFile(){return ListFile; }
};

#endif