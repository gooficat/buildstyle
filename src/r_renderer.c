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
    for (int y = y1; y <= y2; y++) {
        R_DrawPixel(x, y, color);
    }
}

void R_DrawWall(int ax, int at, int ab, int bx, int bt, int bb, Uint32 color) {

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

    SDL_clamp(ax, 0, game_state.scrW - 1);
    SDL_clamp(bx, 0, game_state.scrW - 1);
    int x, y;
    for (x = ax; x != bx; x++) {
        int y1 = dyt * (x - initialX) / dx + at;
        int y2 = dyb * (x - initialX) / dx + ab;

        y1 = SDL_clamp(y1, 0, game_state.scrH - 1);
        y2 = SDL_clamp(y2, 0, game_state.scrH - 1);

        R_VLine(x, y1, y2, color);
    }
    R_DrawLine((vec2i_s){ax, at}, (vec2i_s){bx, bt}, color);
    R_DrawLine((vec2i_s){ax, ab}, (vec2i_s){bx, bb}, color);
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
        50.0f
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
        50.0f
    };
}

void R_DrawPixel(int x, int y, Uint32 color) {
    y = -y + game_state.scrH - 1;
    if (x < 0 || x >= game_state.scrW || y < 0 || y >= game_state.scrH)
        return;
    pixels[y * game_state.scrW + x] = color;
}


void clip(float *x1, float *y1, float x2, float y2) {
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
}


#define MAX_RECURSIONS 2

void R_DrawSector(struct sector* sector) {
    static size_t recursionCount = 0;
    if (recursionCount++ > MAX_RECURSIONS ||sector->drawn) {
        recursionCount = 0;
        return;
    }
    sector->drawn = true;
    for (size_t v = 0; v != sector->vertCt; v++) {
            vert_t va = sector->verts[v];
            vert_t vb = sector->verts[(v + 1) % sector->vertCt];
            R_DrawLine((vec2i_s){va.x, va.y}, (vec2i_s){vb.x, vb.y}, 0xFFFFFFFF);

            vec2_s wallStart = {
                va.x - player.position.x,
                va.y - player.position.y,
            };
            vec2_s wallEnd = {
                vb.x - player.position.x,
                vb.y - player.position.y
            };

            float z1 = sector->z1 - player.position.z;
            float z2 = sector->z2 - player.position.z;


            vec2_s tWallStart = {
                wallStart.x * player.cosRotation - wallStart.y * player.sinRotation,
                wallStart.y * player.cosRotation + wallStart.x * player.sinRotation
            };
            vec2_s tWallEnd = {
                wallEnd.x * player.cosRotation - wallEnd.y * player.sinRotation,
                wallEnd.y * player.cosRotation + wallEnd.x * player.sinRotation
            };

            if (tWallStart.y <= 0 && tWallEnd.y <= 0) {
                continue;
            }
        
            if (!sectors->verts[v].to) {
                if (tWallStart.y <= 0) {
                    clip(&tWallStart.x, &tWallStart.y, tWallEnd.x, tWallEnd.y);
                }
                else if (tWallEnd.y <= 0) {
                    clip(&tWallEnd.x, &tWallEnd.y, tWallStart.x, tWallStart.y);
                }

                int pWallStartX = tWallStart.x * player.focal / tWallStart.y + game_state.scrW / 2;
                int pWallStartY1 = z1 * player.focal / tWallStart.y + game_state.scrH / 2;
                int pWallStartY2 = z2 * player.focal / tWallStart.y + game_state.scrH / 2;

                int pWallEndX = tWallEnd.x * player.focal / tWallEnd.y + game_state.scrW / 2;
                int pWallEndY1 = z1 * player.focal / tWallEnd.y + game_state.scrH / 2;
                int pWallEndY2 = z2 * player.focal / tWallEnd.y + game_state.scrH / 2;

                Uint32 color = (sector->verts[v].to) ? 0xFF00FFFF : 0xFFFF0000;


                R_DrawWall(pWallStartX, pWallStartY1, pWallStartY2, pWallEndX, pWallEndY1, pWallEndY2, color);  
            }
            else {
                R_DrawSector(&sectors[sector->verts[v].to - 1]);
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

    for (int i = -1; i != 2; i += 1) {
        for (int j = -1; j != 2; j += 1) {
            R_DrawPixel(player.position.x + i, player.position.y + j, 0xFFFF00FF);
        }
    }
    R_DrawLine((vec2i_s){player.position.x, player.position.y}, (vec2i_s){player.position.x + player.sinRotation * 10, player.position.y + player.cosRotation * 10}, 0xFFFFFF00);

    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);
}

void R_Destroy() {
    free(sectors);
}