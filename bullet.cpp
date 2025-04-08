#include "bullet.h"
Bullet::Bullet() { // trạng thái ban đầu
    x = 0;
    y = 0;
    dir_x = 0;
    dir_y = 0;
    active = true;
}
Bullet::Bullet(int _x, int _y, int _dir_x, int _dir_y, SDL_Texture *bulletTexture){
    x = _x;
    y = _y;
    dir_x = _dir_x;
    dir_y = _dir_y;
    active = true;
    rect = {x, y, 10, 15};
    texture = bulletTexture;

    //Tính toán góc quay tuỳ vào hướng di chuyển
    if(abs(dir_x) > abs(dir_y)){
        angle = (dir_x > 0 ) ? 90 :270;
    }
    else{
        angle = (dir_y > 0 ) ? 180 : 0;
    }
}
void Bullet::move(){
    x += dir_x * 10; // Toạ độ mới của viên đạn theo hướng di chuyển
    y += dir_y * 10;
    rect.x = x; // Vị trí x của viên đạn trong SDL_Rect
    rect.y = y;
    if(x < TILE_SIZE or x > SCREEN_WIDTH - TILE_SIZE or
       y < TILE_SIZE or y > SCREEN_HEIGHT - TILE_SIZE){
        active = false; // Nếu bay khỏi màn hình thì mất đạn
    }
}
void Bullet::render(SDL_Renderer *renderer){
    if(active and texture){
            // Vẽ viên đạn với góc quay
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }
}


