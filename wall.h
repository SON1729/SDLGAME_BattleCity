#ifndef _WALL__H
#define _WALL__H

#include <SDL.h>
#include "defs.h"

class Wall {
public:
    int x, y; // Toạ độ của tường
    SDL_Rect rect; // HCN đại diện cho tường
    bool active; // Trạng thái của tường 0/1 mất/ hiện
    SDL_Texture *texture; // Hình ảnh của tường

    //Khởi tạo
    Wall(int _x, int _y, SDL_Texture * wallTexture);

    //Vẽ
    void render(SDL_Renderer *renderer);
};

#endif
