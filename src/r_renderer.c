#include "r_renderer.h"

SDL_Surface* surface;
Uint32* pixels;


void R_DrawLine(vec2i_s a, vec2i_s b, Uint32 color) {
    vec2i_s delta = {b.x - a.x, b.y - a.y};
    int step = abs(delta.x) > abs(delta.y) ? abs(delta.x) : abs(delta.y);
    if (!step)
        step = 1;
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
    for (int y = y1; y != y2; y++) {
        R_DrawPixel(x, y, color);
    }
}

void R_DrawWall(int ax, int bx, int at, int bt, int ab, int bb, Uint32 color) {

    if (ax > bx) {
        int swp = ax;
        ax = bx;
        bx = swp;
        
        swp = at;
        at = bt;
        bt = swp;

        swp = ab;
        ab = bb;
        bb = swp;
    }

    int dx = bx - ax;
    int dyt = bt - at;
    int dyb = bb - ab;

    if (!dx) dx = 1;

    int initialX = ax;

    SDL_clamp(ax, 1, game_state.scrW - 2);
    SDL_clamp(bx, 1, game_state.scrW - 2);
    int x, y;
    for (x = ax; x < bx; x++) {
        int y1 = dyt * (x - initialX) / dx + at;
        int y2 = dyb * (x - initialX) / dx + ab;

        y1 = SDL_clamp(y1, 1, game_state.scrH - 2);
        y2 = SDL_clamp(y2, 1, game_state.scrH - 2);

        for (y = y1; y < y2; y++) {
            R_DrawPixel(x, y, color);
        }
    }
}

sect_t* sectors;
size_t sectorCount;

void R_Init() {
    surface = SDL_GetWindowSurface(window);
    pixels = (Uint32*)surface->pixels;

    sectors = calloc(2, sizeof(*sectors));
    sectorCount = 2;
    sectors[0] = (struct sector){
        {
            {210, 210, .to = 0},
            {300, 210, .to = 2},
            {300, 300, .to = 0},
            {110, 300, .to = 0}
        },
        4,
        -20.0f,
        100.0f,
        .color = 0xFF660055
    };
    sectors[1] = (struct sector){
        {
            {300, 210, .to = 0},
            {400, 210, .to = 0},
            {400, 250, .to = 0},
            {300, 300, .to = 1}
        },
        4,
        -20.0f,
        50.0f,
        .color = 0xFF660000
    };
}

void R_DrawPixel(int x, int y, Uint32 color) {
    y = -y + game_state.scrH - 1;
    if (x < 0 || x >= game_state.scrW || y < 0 || y >= game_state.scrH)
        return;
    pixels[y * game_state.scrW + x] = color;
}


void clip(int *x1, int *y1, int *z1, int x2, int y2, int z2) {
    // float s = (y2 - *y1) ? *y1 / (y2 - *y1) : 1e30;

    // *x1 += s * (x2 - *x1);
    // *y1 += s * (y2 - *y1);
    float da = *y1;
    float d = da - y2;
    if (!d)
        d = 1;
    float s = da / d;

    *x1 = *x1 + s * (x2 - (*x1));
    *y1 = *y1 + s * (y2 - (*y1));
    if (!(*y1))
        *y1 = 1;
    *z1 = *z1 + s * (z2 - (*z1));
}


#define MAX_RECURSIONS 2

void R_DrawSector(struct sector* sector) {
    if (sector->drawn) {
        return;
    }
    sector->drawn = true;
        int wx[4], wy[4], wz[4];
    for (size_t v = 0; v != sector->vertCt; v++) {
        vert_t va = sector->verts[v];
        vert_t vb = sector->verts[(v + 1) % sector->vertCt];

        int x1 = va.x - player.position.x;
        int y1 = va.y - player.position.y;

        int x2 = vb.x - player.position.x;
        int y2 = vb.y - player.position.y;

        if (x2 < x1) {
            int swp = x1;
            x1 = x2;
            x2 = swp;

            swp = y1;
            y1 = y2;
            y2 = swp;
        }

        wx[2] = wx[0] = x1 * player.sinRotation - y1 * player.cosRotation;
        wx[3] = wx[1] = x2 * player.sinRotation - y2 * player.cosRotation;

        wy[2] = wy[0] = y1 * player.sinRotation + x1 * player.cosRotation;
        wy[3] = wy[1] = y2 * player.sinRotation + x2 * player.cosRotation;

        wz[1] = wz[0] = sector->z1 - player.position.z;
        wz[3] = wz[2] = sector->z2 - player.position.z;

        if (wy[0] <= 0 && wy[1] <= 0) continue;

        if (!va.to) {
            if (wy[0] <= 0) {
                clip(&wx[0], &wy[0], &wz[0], wx[1], wy[1], wz[1]);
                clip(&wx[2], &wy[2], &wz[2], wx[3], wy[3], wz[3]);
            }
            if (wy[1] <= 0) {
                clip(&wx[1], &wy[1], &wz[1], wx[0], wy[0], wz[0]);
                clip(&wx[3], &wy[3], &wz[3], wx[2], wy[2], wz[2]);
            }


            wx[0] = wx[0] * player.focal / wy[0] + game_state.scrW / 2;
            wx[1] = wx[1] * player.focal / wy[1] + game_state.scrW / 2;

            wy[0] = wz[0] * player.focal / wy[0] + game_state.scrH / 2;
            wy[1] = wz[1] * player.focal / wy[1] + game_state.scrH / 2;
            wy[2] = wz[2] * player.focal / wy[2] + game_state.scrH / 2;
            wy[3] = wz[3] * player.focal / wy[3] + game_state.scrH / 2;

            R_DrawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], sector->color);
        }
        else {
            R_DrawSector(&sectors[va.to - 1]);
        }
    }  
}

void R_Render() {
    SDL_LockSurface(surface);
    memset(pixels, 0x00000000, surface->h * surface->w * sizeof(Uint32));

    R_DrawSector(&sectors[player.currentSector]);


    for (int i = 0; i != sectorCount; i++) {
        sectors[i].drawn = false;
    }

    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);
}

void R_Destroy() {
    free(sectors);
}