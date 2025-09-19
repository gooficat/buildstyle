#include "g_player.h"

player_s player;

void P_Init() {
    player = (player_s){0};
    player.position = (vec3_s){
        250.0f,
        250.0f,
        0.0f
    };
    player.focal = 200.0f;
    player.speed = 0.1f;
    player.lookSpeed = 0.005f;
    player.currentSector = 0;
    player.rotation = 0.0f;
}

void P_Update() {
    vec3_s move = {0};
    player.velocity = (vec3_s){0};
    if (game_state.keys[SDL_SCANCODE_W]) {
        move.y++;
    }
    if (game_state.keys[SDL_SCANCODE_S]) {
        move.y--;
    }
    if (game_state.keys[SDL_SCANCODE_A]) {
        move.x--;
    }
    if (game_state.keys[SDL_SCANCODE_D]) {
        move.x++;
    }
    if (game_state.keys[SDL_SCANCODE_LSHIFT]) {
        move.z--;
    }
    if (game_state.keys[SDL_SCANCODE_SPACE]) {
        move.z++;
    }
    if (game_state.keys[SDL_SCANCODE_Q]) {
        player.rotation += player.lookSpeed * game_state.deltaTime;
    }
    if (game_state.keys[SDL_SCANCODE_E]) {
        player.rotation -= player.lookSpeed * game_state.deltaTime;
    }
    player.sinRotation = sinf(player.rotation);
    player.cosRotation = cosf(player.rotation);
    if (move.x && move.y) {
        move.x *= 0.7071f;
        move.y *= 0.7071f;
    }
    vec3_s newPos = {
        player.position.x + (move.x * player.sinRotation + move.y * player.cosRotation) * player.speed * game_state.deltaTime,
        player.position.y + (move.y * player.sinRotation - move.x * player.cosRotation) * player.speed * game_state.deltaTime,
        player.position.z + (move.z) * player.speed * game_state.deltaTime
    };

    G_Entity_Move_And_Collide(&sectors[player.currentSector], &player.position, newPos);
    
    for (size_t s = 0; s != sectorCount; s++) {
        if (G_Polygon_Point(&sectors[s], (vec2_s){player.position.x, player.position.y})) {
            player.currentSector = s;
            //printf("Player in sector %d\n", player.currentSector);
            break;
        }
    }
}