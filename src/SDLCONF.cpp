#include "../include/SDLCONF.hpp"
#include "../include/netw.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

SDL_GameController *controller = nullptr;

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

int input(){
  SDL_Event event;
  bool running = true;
  int sock = crSocket();
  while(true){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        return 0;
      }else if(event.type == SDL_CONTROLLERBUTTONDOWN){
        std::cout<<"Button Pressed: "<<(int)event.cbutton.button<<std::endl;
        std::string button_data = "Button Pressed: " + std::to_string(event.cbutton.button) + "\n"; 
        sendData(sock, "127.0.0.1", 8080, button_data);
      }else if(event.type == SDL_CONTROLLERAXISMOTION){
        std::cout<<"Axis: "<<static_cast<int>(event.caxis.axis)<<" Value: "<<event.caxis.value<<std::endl;
        std::string axis_data = "Button: " +  std::to_string(event.caxis.axis) + " Axis Value: " + std::to_string(event.caxis.value) + "\n";
        sendData(sock, "127.0.0.1", 8080, axis_data);
      }else if(event.type == SDL_CONTROLLERDEVICEREMOVED){
        std::cout<<"Controller Disconnected!"<<std::endl;
        return 0;
      }
    }
  }
  close(sock);
}

void clean(){
  if(controller)
    SDL_GameControllerClose(controller);
  SDL_Quit();
}
