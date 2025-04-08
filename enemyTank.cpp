#include "enemyTank.h"


    EnemyTank::EnemyTank(){
        x = y = 0;
        dir_x = dir_y = 0;
        moveDelay = 10;
        shootDelay = 3;
        active = true;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
    }
    EnemyTank::EnemyTank(int _x, int _y){
        x = _x;
        y = _y;
        moveDelay = 10;
        shootDelay = 3;
        dir_x = 0;
        dir_y = 0;
        active = true;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
    }
        void EnemyTank::move(const PlayerTank &player, vector<Wall> &walls){
            if(--moveDelay > 0) return; // Chưa hết thời gian hồi di chuyển
            moveDelay = 10; // Thời gian hồi di chuyển
            int dx = player.x - x;
            int dy = player.y - y;
            double distance = sqrt(dx * dx + dy * dy); // Khoảng cách từ xe tăng địch đến người chơi

            if(distance < 5) return; // Khoảng cách quá gần không di chuyển
            if(abs(dx) > abs(dy)){
                dir_x = (dx > 0) ? 10 : -10; // Chọn đường di chuyển (trái hoặc phải)
                dir_y = 0;
            }
            else{
                dir_y = (dy > 0) ? 10 : -10; // Chọn đường di chuyển (lên hoặc xuống)
                dir_x = 0;
            }
            int new_x = x + dir_x;
            int new_y = y + dir_y;

        SDL_Rect newRect = {new_x, new_y, TILE_SIZE, TILE_SIZE};
        // Kiểm tra va chạm với tường
        for(const auto &wall : walls){
            if(wall.active and SDL_HasIntersection(&newRect, &wall.rect)){
                return;
            }
        }
        if(new_x >= TILE_SIZE and new_x <= SCREEN_WIDTH - TILE_SIZE * 2 and
           new_y >= TILE_SIZE and new_y <=SCREEN_HEIGHT - TILE_SIZE * 2){
            x = new_x;
            y = new_y;
            rect.x = x;
            rect. y = y;
           }
        }

        void EnemyTank::shoot(){
            if(--shootDelay > 0 ){ // Thời gian chưa hồi chưa bắn
                return;
            }
            shootDelay = 3; // Thời gian hồi bắn
            if(dir_x == 0 and dir_y ==0){
                dir_y = 1; // Nếu đứng im bắn xuống dưới
            }
            // Hướng bắn của viên đạn
            int bullet_dir_x = (abs(dir_x) > abs(dir_y)) ? ((dir_x > 0) ? 1 : -1) : 0;
            int bullet_dir_y = (abs(dir_x) > abs(dir_y)) ? 0 : ((dir_y >0) ? 1 : -1);
            Bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 -5, bullet_dir_x, bullet_dir_y, bulletTexture));
        }

        void EnemyTank::updateBullets(){
            for(auto &bullet : Bullets){
                bullet.move();
            }
            Bullets.erase(remove_if(Bullets.begin(), Bullets.end(),
                                    [](Bullet &b){return !b.active;}),Bullets.end());
        }
        void EnemyTank::setTankTexture(SDL_Texture * tex){
            tankTexture = tex;
        }
        void EnemyTank::render(SDL_Renderer *renderer){
            SDL_RenderCopyEx(renderer, tankTexture, NULL, &rect,
                         (dir_x > 0 ) ? 90 : (dir_y < 0) ? 0 : (dir_y > 0) ? 180 : 270, NULL, SDL_FLIP_NONE);
            for(auto &bullet : Bullets){
                bullet.render(renderer);
            }
        }
        void EnemyTank::setBulletTexture(SDL_Texture *tex){
        bulletTexture = tex;
        }

