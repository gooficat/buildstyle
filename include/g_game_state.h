#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct g_game_state_s {
    unsigned short scrW, scrH;
    float targetFPS, deltaTime, targetFrameTime, frameStartTime;
    bool running;
    SDL_Event event;
    bool keys[SDL_NUM_SCANCODES];
} game_state_s;

extern game_state_s game_state;

typedef struct vertex {
    float x, y;
    size_t to;
} vert_t;

typedef struct sector {
    vert_t verts[8];
    size_t vertCt;
    float z1, z2;
    bool drawn;
} sect_t;

extern sect_t *sectors;

extern size_t sectorCount;

void G_Init(unsigned short scrW, unsigned short scrH, short targetFPS);
void G_FrameStart();
void G_FrameEnd();