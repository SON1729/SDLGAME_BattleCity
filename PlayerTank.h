#ifndef _PLAYERTANK__H
#define _PLAYERTANK__H

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <algorithm>

#include "bullet.h"
#include "wall.h"
#include "defs.h"

using namespace std;

class PlayerTank{
private:
    int dir_x, dir_y;
    int shootCooldown = 0; // Thời gian đã hồi
    int MAX_COOLDOWN = 45; // Thời gian hồi đạn
    SDL_Texture *bulletTexture;
    SDL_Texture *tankTexture;
    Mix_Chunk *shootSound; // Âm than bắn
public:
    int x, y;
    SDL_Rect rect;
    vector<Bullet> Bullets;
    PlayerTank();
    PlayerTank(int _x, int _y);
    void move(int dx, int dy, const vector<Wall> &walls); // Di chuyển của xe
    void setShootSound(Mix_Chunk * sound);
    void shot(); // Xe bắn
    void setBulletTexture(SDL_Texture *tex);
    void updateBullets(); // Cập nhật trạng thái của đạn
    void setTankTexture(SDL_Texture *tex);
    void render(SDL_Renderer *renderer);
};

#endif
