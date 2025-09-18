#pragma once

#include "g_game_state.h"
#include "defs.h"
#include "g_collision.h"

typedef struct player {
    vec3_s position, velocity;
    float rotation, sinRotation, cosRotation;
    float focal;
    float speed;
    float lookSpeed;
    size_t currentSector;
} player_s;

extern player_s player;

void P_Init();
void P_Update();
