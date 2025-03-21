#include "../include/netw.hpp"

//UDP "CLIENT" for web server
int crSocket(){
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock == -1 ){
    std::cerr<<"Failed to create a Socket"<<std::endl;
    return -1;
  } else {
    std::cout<<"Socket Created Successfully!"<<std::endl;
    return sock;
  }
}

void sendData(int sock, const char* serverIP, int serverPort, std::string data="NULL"){
  sockaddr_in server{};
  server.sin_family = AF_INET;
  server.sin_port = htons(serverPort);
  server.sin_addr.s_addr = inet_addr(serverIP);
  int sentBytes = sendto(sock, data.c_str(), data.size(), 0, (sockaddr*)&server, sizeof(server));
  if(sentBytes < 0) std::cerr<<"Failed to send data!"<<std::endl;
  close(sock);
}

//int connectServer(int sock, const char* serverIP, int port){
//
//  sockaddr_in server;
//  server.sin_port = htons(port);
//  server.sin_family = AF_INET;
//  if(inet_pton(AF_INET, serverIP, &server.sin_addr) <= 0){
//    std::cerr<<"Invalid Address or address not supported!"<<std::endl;
//    return -1;
//  }
//
//  if(connect(sock, (sockaddr*)&server, sizeof(server)) == -1){
//    std::cerr<<"Connection failed"<<std::endl;
//    return -1;
//  }
//
//  std::cout<<"Connected to server!"<<std::endl;
//  return 0;
//
//}
