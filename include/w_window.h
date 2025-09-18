#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "g_game_state.h"

extern SDL_Window* window;

void W_Init();
void W_Destroy();
