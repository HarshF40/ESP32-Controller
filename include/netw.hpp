#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include<unistd.h>
#include<iostream>

int crSocket();
//int connectServer(int, const char*, int);
void sendData(int, const char*, int, std::string);
