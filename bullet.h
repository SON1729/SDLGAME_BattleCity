#ifndef _BULLET__H
#define _BULLET__H

#include <SDL.h>
#include "defs.h"

class Bullet {
private:
    int x, y; // Toạ độ
    int dir_x, dir_y; // Hướng di chuyển
    int angle; // Góc quay
    SDL_Texture *texture;

public:
    SDL_Rect rect;
    bool active;
    Bullet();
    Bullet(int _x, int _y, int _dir_x, int _dir_y, SDL_Texture *bulletTexture);
    void move(); // Cách di chuyển
    void render(SDL_Renderer *renderer); // Vẽ
};

#endif

