#include "../include/SDLCONF.hpp"
#include "../include/netw.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <mutex>

std::mutex event_mutex;
SDL_Event event;
SDL_GameController *controller = nullptr;
std::string left_trigger_axis = "0";
std::string right_trigger_axis = "0";

int initSDL(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
      std::cerr<<"SDL_Init failed: "<<SDL_GetError()<<std::endl;
      return -1;
    } else {
      std::cout<<"SDL Initialised Successfully!"<<std::endl;
      return 1;
  }
}

void openDetJoy(){
  int numJoysticks = SDL_NumJoysticks();
  if(numJoysticks < 1) std::cout<<"No Controllers Connected!"<<std::endl;
  else {
    std::cout<<numJoysticks<<" device(s) are connected."<<std::endl;
    controller = SDL_GameControllerOpen(0); //Opens the first available controller
    if(controller) std::cout<<"Controller 0: "<<SDL_GameControllerName(controller)<<" Opened"<<std::endl;
    else std::cerr<<"Failed to Open Game Controller 0!"<<std::endl;
  }
}

void get_left_trigger_val(){
  while(true){
  std::unique_lock<std::mutex> lock(event_mutex);
  std::string button = std::to_string(event.caxis.axis);
  std::string value = std::to_string(event.caxis.value);
  lock.unlock();
  if(button == "4") {
    left_trigger_axis = value;
  }
  }
}

void get_right_trigger_val(){
  while(true){
  std::unique_lock<std::mutex> lock(event_mutex);
  std::string button = std::to_string(event.caxis.axis);
  std::string value = std::to_string(event.caxis.value);
  lock.unlock();
  if(button == "5") {
    right_trigger_axis = value;
    }
  }
}

int input(){
  bool running = true;
  int sock = crSocket();

  std::thread right(get_right_trigger_val);
  std::thread left(get_left_trigger_val);

  while(true){
    sendData(sock, "127.0.0.1", 8080, "((4," + left_trigger_axis + "),(5," + right_trigger_axis + "))\n");
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT) return 0;
      if(event.type == SDL_CONTROLLERAXISMOTION){ 
        sendData(sock, "127.0.0.1", 8080, "((4," + left_trigger_axis + "),(5," + right_trigger_axis + "))\n");
      }
      if(event.type == SDL_CONTROLLERDEVICEREMOVED){
        std::cout<<"Controller Disconnected!"<<std::endl;
        right.join();
        left.join();
        close(sock);
        return 0;
      }
    }
  }
  right.join();
  left.join();
  close(sock);
  return 0;
}

void clean(){
  if(controller)
    SDL_GameControllerClose(controller);
  SDL_Quit();
}
