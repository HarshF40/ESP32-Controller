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
//#define WRAPPER 300

std::mutex event_mutex;
SDL_Event event;
SDL_GameController *controller = nullptr;

int left_trigger_axis = 0;
int right_trigger_axis = 0;
int dpad_val = 0;

bool dpadRightHeld = false;
bool dpadLeftHeld = false;

int initSDL(){
    if(SDL_Init(SDL_INIT_GAMECONTROLLER) != 0){
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
    controller = SDL_GameControllerOpen(0);
    if(controller) std::cout<<"Controller 0: "<<SDL_GameControllerName(controller)<<" Opened"<<std::endl;
    else std::cerr<<"Failed to Open Game Controller 0!"<<std::endl;
  }
}

void get_left_trigger_val(){
  while(true){
  std::unique_lock<std::mutex> lock(event_mutex);
  int button = (int)(event.caxis.axis);
  //int value = ((((int)(event.caxis.value)+ 32768)/2)); 
  int value = (int)(event.caxis.value);
  lock.unlock();
  if(button == 4) left_trigger_axis = value < 10922 ? 0 : value < 21844 ? 1 : 2;
  }
}

void get_right_trigger_val(){
  while(true){
  std::unique_lock<std::mutex> lock(event_mutex);
  int button = std::abs((int)(event.caxis.axis));
  //int value = ((((int)(event.caxis.value)+ 32768)/2));
  int value = (int)(event.caxis.value);
  lock.unlock();
  if(button == 5) right_trigger_axis = value < 10922 ? 0 : value < 21844 ? 1 : 2;
  }
}

void listen_dpadL(){
  while(true){ if(dpadLeftHeld) dpad_val = -1; }
}

void listen_dpadR(){
  while(true){ if(dpadRightHeld) dpad_val = 1; }
}

void listen_Bup(){
  while(true){
  if(event.type == SDL_CONTROLLERBUTTONDOWN){
    if(event.cbutton.button == 13) dpadLeftHeld = true;
    if(event.cbutton.button == 14) dpadRightHeld = true;
  } else if(event.type == SDL_CONTROLLERBUTTONUP){
    if(event.cbutton.button == 13) dpadLeftHeld = false; dpad_val = 0;
    if(event.cbutton.button == 14) dpadRightHeld = false; dpad_val = 0;
    }
  }
}

int input(){


  bool running = true;
  int sock = crSocket();

  std::thread right(get_right_trigger_val);
  std::thread left(get_left_trigger_val);
  std::thread dpadL(listen_dpadL);
  std::thread dpadR(listen_dpadR);
  std::thread listenbup(listen_Bup);

  while(true){

    sendData(sock, ESP32IP, 8080, "(" + std::to_string(left_trigger_axis) + "," + std::to_string(right_trigger_axis) + "," + "dpad: " + std::to_string(dpad_val) + ")\n");
    std::cout<<left_trigger_axis<<" "<<right_trigger_axis<<" "<<dpad_val<<"\n";

    while(SDL_PollEvent(&event)){

      if(event.type == SDL_QUIT) return 0;
      if(event.type == SDL_CONTROLLERAXISMOTION || event.type == SDL_CONTROLLERBUTTONDOWN){ 
        std::cout<<"Button: "<<static_cast<int>(event.cbutton.button)<<"\n";
        sendData(sock, ESP32IP, 8080, "(" + std::to_string(left_trigger_axis) + "," + std::to_string(right_trigger_axis) + "," + "dpad: " + std::to_string(dpad_val) + ")\n");
        std::cout<<left_trigger_axis<<" "<<right_trigger_axis<<" "<<dpad_val<<"\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }

      if(event.type == SDL_CONTROLLERDEVICEREMOVED){
        std::cout<<"Controller Disconnected!"<<std::endl;
        right.join();
        left.join();
        dpadL.join();
        dpadR.join();
        listenbup.join();
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
  if(controller) SDL_GameControllerClose(controller);
  SDL_Quit();
}
