#include "g_collision.h"


bool G_Polygon_Point(sect_t* s, vec2_s p) {
    for (size_t i = 0; i != s->vertCt; i++) {
        if (s->verts[i].to != 0) continue;
        vec2_s va = (vec2_s){s->verts[i].x, s->verts[i].y};
        vec2_s vb = (vec2_s){s->verts[(i + 1) % s->vertCt].x, s->verts[(i + 1) % s->vertCt].y};

        if (p.y > fmin(va.y, vb.y) &&
            p.y <= fmax(va.y, vb.y) &&
            p.x <= fmax(va.x, vb.x)) {
            
            float x_intersection = (p.y - va.y) * (vb.x - va.x) / (vb.y - va.y) + va.x;

            if (va.x == vb.x || p.x <= x_intersection) {
                return true;
            }
        }
    }
    return false;
}