#include "w_window.h"

SDL_Window* window;

void W_Init() {
    window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game_state.scrW, game_state.scrH, SDL_WINDOW_SHOWN);
}
void W_Destroy() {
    SDL_DestroyWindow(window);
}

SDL_Window* W_GetWindow() {
    return window;
}