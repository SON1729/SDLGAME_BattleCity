#include "PlayerTank.h"

    PlayerTank::PlayerTank() {
        x = y = 0;
        dir_x = dir_y = 0;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
    }

     PlayerTank::PlayerTank(int _x, int _y){
        x = _x;
        y = _y;
        dir_x = 0;
        dir_y = 0;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
    }
    void PlayerTank::move(int dx, int dy, const vector<Wall> &walls){
        int new_x = x + dx; // Vị trí mới
        int new_y = y + dy;
        this->dir_x = dx; // Hướng di chuyển
        this->dir_y = dy;

        SDL_Rect newRect = {new_x, new_y, TILE_SIZE, TILE_SIZE};
        for(int i = 0 ;i < walls.size(); i++){
            if(walls[i].active and SDL_HasIntersection(&newRect, &walls[i].rect)){
                return; // Kiểm tra va chạm với tường có thì không đi chuyển
            }
        }
        // Kiểm tra xem xe có ra ngoài màn hình không
        if(new_x >= TILE_SIZE and new_x <= SCREEN_WIDTH - TILE_SIZE * 2 and
           new_y >= TILE_SIZE and new_y <= SCREEN_HEIGHT - TILE_SIZE * 2){
            x = new_x;
            y = new_y;
            rect.x = x;
            rect.y = y;
           }
    }
    void PlayerTank::setShootSound(Mix_Chunk * sound){
        shootSound = sound;
    }
    void PlayerTank::shot(){
        if(shootCooldown > 0) return; // Chưa hồi chiêu không cho bắn
        if(dir_x == 0 and dir_y == 0){ // Không di chuyển mặc định bắn dọc
            dir_y = -1;
        }
        // Hướng bắn của đạn
        int bullet_dir_x = 0;
        int bullet_dir_y = 0;
        if(abs(dir_x) > abs(dir_y)){
            bullet_dir_x = (dir_x > 0) ? 1 : -1; // Theo chiều ngang
        }
        else{
            bullet_dir_y = (dir_y > 0) ? 1 : -1; // Theo chiều dọc
        }
        // Thêm đạn vào danh sách
        Bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE  / 2 - 5, bullet_dir_x, bullet_dir_y, bulletTexture ));
        if(shootSound) Mix_PlayChannel(-1, shootSound, 0);
        shootCooldown = MAX_COOLDOWN; // Đặt lại thời gian hồi chiêu
    }
    void PlayerTank::setBulletTexture(SDL_Texture *tex){
        bulletTexture = tex;
    }
    void PlayerTank::updateBullets(){
        if(shootCooldown > 0 ){
            shootCooldown--; // Giảm thời gian hồi chiêu
        }
        for(auto &bullet : Bullets){
            bullet.move();
        }
        // Xoá các viên đạn không hoạt động
        Bullets.erase(remove_if(Bullets.begin(), Bullets.end(),
                                [](Bullet &b){return !b.active;}), Bullets.end());
    }
    void PlayerTank::setTankTexture(SDL_Texture *tex){
        tankTexture = tex;
    }
    void PlayerTank::render(SDL_Renderer *renderer){
        // Vẽ xe tăng với góc quay
        SDL_RenderCopyEx(renderer, tankTexture, NULL, &rect,
                         (dir_x > 0 ) ? 90 : (dir_y < 0) ? 0 : (dir_y > 0) ? 180 : 270, NULL, SDL_FLIP_NONE);
        for(auto &bullet : Bullets){
                // Vẽ từng viên đạn
            bullet.render(renderer);
        }
    }
