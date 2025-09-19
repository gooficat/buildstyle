#pragma once

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include "defs.h"
#include "g_game_state.h"


bool G_Polygon_Point(sect_t* s, vec2_s p);

void G_Entity_Move_And_Collide(sect_t* s, vec3_s* p, vec3_s n);