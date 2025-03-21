#include "../include/SDLCONF.hpp"

int sdl();

int main (int argc, char *argv[]) {
  if(sdl() == -1) return -1;
  return 0;
}

int sdl(){
  if(initSDL() == -1) return -1;
  openDetJoy();
  input();
  clean();
  return 0;
}
