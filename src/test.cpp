#include "../include/netw.hpp"

int main(){
  int sockFd = crSocket();
  sendData(sockFd, "127.0.0.1", 6969, "Hello World");
  return 0;
}
