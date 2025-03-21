#pragma once
#include<SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_joystick.h>

extern SDL_GameController *controller;

int initSDL();
void openDetJoy();
int input();
void clean();
