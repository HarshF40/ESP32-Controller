#include "../include/SDLCONF.hpp"
#include "../include/netw.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <chrono>

#define ESP32IP "127.0.0.1"

std::mutex event_mutex;
SDL_Event event;
SDL_GameController *controller = nullptr;
float left_trigger_axis = 0;
float right_trigger_axis = 0;

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
  int button = (int)(event.caxis.axis);
  int value = (int)(event.caxis.value) <= 0 ? 0 : (int)(event.caxis.value);
  lock.unlock();
  if(button == 4) {
    left_trigger_axis = value;
  }
  }
}

void get_right_trigger_val(){
  while(true){
  std::unique_lock<std::mutex> lock(event_mutex);
  int button = std::abs((int)(event.caxis.axis));
  int value = (int)(event.caxis.value) <= 0 ? 0 : (int)(event.caxis.value);
  lock.unlock();
  if(button == 5) {
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
        sendData(sock, ESP32IP, 8080, "(" + std::to_string(left_trigger_axis) + "," + std::to_string(right_trigger_axis) + ")\n");
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT) return 0;
      if(event.type == SDL_CONTROLLERAXISMOTION){ 
        sendData(sock, ESP32IP, 8080, "(" + std::to_string(left_trigger_axis) + "," + std::to_string(right_trigger_axis) + ")\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
      if(event.type == SDL_CONTROLLERDEVICEREMOVED){
        std::cout<<"Controller Disconnected!"<<std::endl;
        right.join();
        left.join();
        close(sock);
        return 0;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
