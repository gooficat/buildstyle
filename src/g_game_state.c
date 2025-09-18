#include "g_game_state.h"

game_state_s game_state;

void G_Init(unsigned short scrW, unsigned short scrH, short targetFPS) {
    game_state.scrW = scrW;
    game_state.scrH = scrH;
    game_state.targetFPS = targetFPS;
    game_state.targetFrameTime = 1000.0f / targetFPS;
    game_state.running = true;
    SDL_Init(SDL_INIT_VIDEO);
}

void G_FrameStart() {
    game_state.frameStartTime = SDL_GetTicks();
    while (SDL_PollEvent(&game_state.event)) {
        switch(game_state.event.type) {
            case SDL_QUIT:
                game_state.running = false;
                break;
            case SDL_KEYDOWN:
                game_state.keys[game_state.event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                game_state.keys[game_state.event.key.keysym.scancode] = false;
                break;
            default:
                break;
        }
    }
}

void G_FrameEnd() {
    game_state.deltaTime = SDL_GetTicks() - game_state.frameStartTime;
}