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
void get_right_trigger_val();
void get_left_trigger_val();
void listen_dpadL();
void listen_dpadR();
void listen_Bup();
