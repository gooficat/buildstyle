#include "g_game_state.h"
#include "r_renderer.h"
#include "w_window.h"
#include "g_player.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCRW 640
#define SCRH 480

int main() {
    G_Init(SCRW, SCRH, 60);
    W_Init();
    R_Init();
    P_Init();

    while (game_state.running) {
        G_FrameStart();
        P_Update();
        R_Render();
        G_FrameEnd();
    }
    
    R_Destroy();
    W_Destroy();
    return 0;
}