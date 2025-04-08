#ifndef _ENEMYTANK__H
#define _ENEMYTANK__H

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <algorithm>
#include <cmath>

#include "bullet.h"
#include "wall.h"
#include "defs.h"
#include "PlayerTank.h"

using namespace std;

class EnemyTank{
private:
    int x, y;
    int dir_x, dir_y;
    SDL_Texture *bulletTexture;
    SDL_Texture *tankTexture;
public:
    int moveDelay, shootDelay; // Thời gian hồi di chuyển và bắn
    SDL_Rect rect;
    bool active;
    vector<Bullet> Bullets;
    EnemyTank();
    EnemyTank(int _x, int _y);
    void move (const PlayerTank &player, vector<Wall> &walls);
    void shoot();
    void updateBullets();
    void setTankTexture(SDL_Texture * tex);
    void render(SDL_Renderer *renderer);
    void setBulletTexture(SDL_Texture *tex);
};



#endif
