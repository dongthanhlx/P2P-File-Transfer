#include "Peer.h"
#include<vector>
using namespace std;
Peer::Peer(char* IP, int port){
    setIP(IP);
    setPort(port);
}

void Peer::setIP(char *ip){
    IP = ip;
}

void Peer::setState(bool state){
    State = state;
}

void Peer::setListFile(vector<char*> listFile){
    ListFile = listFile;
}