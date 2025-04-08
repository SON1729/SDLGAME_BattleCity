#include "wall.h"

Wall::Wall(int _x, int _y, SDL_Texture * wallTexture){
    x = _x;
    y = _y;
    active = true;
    rect = {x, y, TILE_SIZE, TILE_SIZE}; //Thiết lập vị trí và kích thước cho tường
    texture = wallTexture;//Lưu texture dùng để vẽ tường
}
void Wall::render(SDL_Renderer *renderer){
    if(active and texture){
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        // Vẽ texture lên renderer tại vị trí rect
    }
}



