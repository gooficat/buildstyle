#include "g_collision.h"


bool G_Polygon_Point(sect_t* s, vec2_s p) {
    bool collision = false;
    for (size_t i = 0; i != s->vertCt; i++) {
        vec2_s va = (vec2_s){s->verts[i].x, s->verts[i].y};
        vec2_s vb = (vec2_s){s->verts[(i + 1) % s->vertCt].x, s->verts[(i + 1) % s->vertCt].y};
        if (((va.y > p.y) != (vb.y > p.y)) &&
            (p.x < (vb.x-va.x) * (p.y-va.y) / (vb.y-va.y) + va.x)) {                
                collision = !collision;
        }
    }
    return collision;
}

bool G_Line_Line(vec2_s a1, vec2_s a2, vec2_s b1, vec2_s b2) {
    float uA = ((b2.x - b1.x) * (a1.y - b1.y) - (b2.y - b1.y) * (a1.x - b1.x)) /
               ((b2.y - b1.y) * (a2.x - a1.x) - (b2.x - b1.x) * (a2.y - a1.y));
    float uB = ((a2.x - a1.x) * (a1.y - b1.y) - (a2.y - a1.y) * (a1.x - b1.x)) /
               ((b2.y - b1.y) * (a2.x - a1.x) - (b2.x - b1.x) * (a2.y - a1.y));
        
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        return true;
    }
    return false;
}

void G_Entity_Move_And_Collide(sect_t* s, vec3_s* p, vec2_s n) {
    for (size_t i = 0; i != s->vertCt; i++) {
        vec2_s va = (vec2_s){s->verts[i].x, s->verts[i].y};
        vec2_s vb = (vec2_s){s->verts[(i + 1) % s->vertCt].x, s->verts[(i + 1) % s->vertCt].y};

        if (s->verts[i].to == 0) { //if to is 0 then it is a wall
            if (G_Line_Line((vec2_s){p->x, p->y}, (vec2_s){n.x, n.y}, va, vb)) {
                return;
            }
        }
    }
    p->x = n.x;
    p->y = n.y;
}