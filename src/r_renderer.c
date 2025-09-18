#include "r_renderer.h"

SDL_Surface* surface;
Uint32* pixels;


void R_DrawLine(vec2i_s a, vec2i_s b, Uint32 color) {
    vec2i_s delta = {b.x - a.x, b.y - a.y};
    int step = abs(delta.x) > abs(delta.y) ? abs(delta.x) : abs(delta.y);
    vec2_s increment = {delta.x / (float)step, delta.y / (float)step};
    float x = a.x;
    float y = a.y;
    for (int i = 0; i <= step; i++) {
        R_DrawPixel((int)x, (int)y, color);
        x += increment.x;
        y += increment.y;
    }
}

void R_VLine(int x, int y1, int y2, Uint32 color) {
    for (int y = y1; y <= y2; y++) {
        R_DrawPixel(x, y, color);
    }
}

sect_t* sectors;
size_t sectorCount;

void R_Init() {
    surface = SDL_GetWindowSurface(window);
    pixels = (Uint32*)surface->pixels;

    sectors = malloc(2 * sizeof(*sectors));
    sectorCount = 2;
    sectors[0] = (struct sector){
        {
            {210, 210},
            {300, 210, .to = 2},
            {300, 300},
            {210, 300}
        },
        4
    };
    sectors[1] = (struct sector){
        {
            {300, 210},
            {400, 210},
            {400, 250},
            {300, 300, .to = 1}
        },
        4
    };
}

void R_DrawPixel(int x, int y, Uint32 color) {
    y = -y + game_state.scrH - 1;
    if (x < 0 || x >= game_state.scrW || y < 0 || y >= game_state.scrH)
        return;
    pixels[y * game_state.scrW + x] = color;
}

void R_DrawSector(struct sector* sector) {
    for (size_t v = 0; v != sector->vertCt; v++) {
        vert_t va = sector->verts[v];
        vert_t vb = sector->verts[(v + 1) % sector->vertCt];

        if (va.to == 1) return;
        if (!va.to) {
            R_DrawLine((vec2i_s){va.x, va.y}, (vec2i_s){vb.x, vb.y}, 0xFFFFFFFF);
        } else {
            R_DrawSector((struct sector*)&sectors[va.to - 1]);
        }
    }
}

void R_Render() {
    SDL_LockSurface(surface);
    memset(pixels, 0x00000000, surface->h * surface->w * sizeof(Uint32));
    R_DrawPixel(player.position.x, player.position.y, 0xFFFF00FF);
    
    R_DrawSector(&sectors[player.currentSector]);

    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);
}

void R_Destroy() {
    free(sectors);
}