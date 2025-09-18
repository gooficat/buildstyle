#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "defs.h"
#include "g_game_state.h"
#include "w_window.h"
#include "g_player.h"

void R_Init();

void R_DrawPixel(int x, int y, Uint32 color);

void R_Render();

void R_Destroy();

void R_DrawSector(struct sector* sector);