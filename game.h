#ifndef _GAME__H
#define _GAME__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <algorithm>
#include <cmath>

#include "bullet.h"
#include "wall.h"
#include "defs.h"
#include "PlayerTank.h"
#include "enemyTank.h"

using namespace std;

class Game{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    vector<Wall> walls;
    PlayerTank Player; // Số kẻ địch
    int  EnemyNumber;
    vector<EnemyTank> EnemyTanks; // Các kẻ địch
    //Các texture sử dụng
    SDL_Texture *wallTexture;
    SDL_Texture *playerBulletTexture;
    SDL_Texture *enemyBulletTexture;
    SDL_Texture *playerTankTexture;
    SDL_Texture *enemyTankTexture;
    SDL_Texture *explosionTexture;
    SDL_Texture *backgroundTexture;
    SDL_Texture *winTexture;
    SDL_Texture *loseTexture;
    // Các hiệu ứng âm thanh
    Mix_Chunk *shootSound;
    Mix_Chunk *explosionSound;
    Mix_Chunk *wallHitSound;
    // Font chữ
    TTF_Font *font;
    SDL_Rect explosionRect;
    int explosionDuration; // Thời gian hiện vụ nổ
    int explosionCounter; // Thời gian đã nổ
    int currentLevel; // level hiện tại
public:
    bool running;// Trạng thái của game (đang chạy hay không)
    bool inMenu;// Trạng thái menu (có đang ở menu chính không)
    bool showExplosion;// Trạng thái hiển thị hiệu ứng nổ
    bool gameOver;// Trạng thái game (game over hay chưa)
    bool playerWon;// Trạng thái chiến thắng (người chơi thắng hay không)
    Game();
    void resetLevel();// Hàm reset lại tất cả mọi thứ trong game
    void loadBackgroundTexture(); // Hàm load nền game
    void renderMenu(); // Hàm vẽ màn hình menu
    void generateWalls(); // Hàm tạo ra tường
    void render(); // Hàm vẽ các phần tử lên màn hình
    void loadBulletTextures(); // Hàm load texture cho đạn
    void loadTankTexture(); // Hàm load texture cho xe tăng (player + enemy)
    void initExplosion(); // Hàm khởi tạo explosion texture
    void triggerExplosion(int x, int y); // Hàm vị trí của vị nổ
    void updateExplosion(); // Hàm cập nhật trạng thái vụ nổ
    void renderExplosion(); // Hàm vẽ vụ nổ
    void update(); // Hàm cập nhật trạng thái game
    void handleEvents(); // Hàm xử lí các sự kiện
    void spawnEnemyTanks(); // Hàm sinh ra các enemy vị trí ngẫu nhiên
    void loadEndGameTextures();// Hàm tải các texture cho màn hình kết thúc game (thắng hoặc thua)
    void renderEndGame();// Hàm vẽ màn hình kết thúc game (hiển thị thông báo thắng hoặc thua)
    void run(); // Hàm chạy game
    ~Game(); // Hàm huỷ

};



#endif
