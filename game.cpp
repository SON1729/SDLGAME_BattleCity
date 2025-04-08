#include "game.h"
        // Khởi tạo SDL
        Game::Game(){
        running = true;
        inMenu = true;
        gameOver = false;
        playerWon = false;
        currentLevel = 1;
        // SDL
        if(SDL_Init(SDL_INIT_VIDEO) < 0){
            cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
            running = false;
        }
        // Cửa sổ game
        window = SDL_CreateWindow("Game Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
        if(!window){
            cerr << "Window could not be created! SDL_Error: "<< SDL_GetError() << endl;
            running = false;
        }
        // Renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer){
            cerr << "Renderer could not be created! SDL_Error: "<< SDL_GetError() << endl;
            running = false;
        }
        //SDL_mixer
         if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << endl;
        running = false;
        }
        // Tải âm thanh
        shootSound = Mix_LoadWAV("sound/shoot.wav");
        explosionSound = Mix_LoadWAV("sound/explosion.wav");
        wallHitSound = Mix_LoadWAV("sound/wallHit.wav");
        // Tải ảnh
        loadBackgroundTexture();
        loadBulletTextures();
        loadTankTexture();
        generateWalls();
        loadEndGameTextures();
        initExplosion();
        // Reset lại level
        resetLevel();
    }
    void Game::resetLevel(){
    walls.clear(); // Xoá tường
    EnemyTanks.clear(); // Xoá xe tăng địch
    Player.Bullets.clear();// Xoá đạn

    generateWalls();//Tạo lại tường
    // Tạo lại xe tăng địch
    Player = PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT-2)*TILE_SIZE);
    Player.setTankTexture(playerTankTexture);
    Player.setBulletTexture(playerBulletTexture);
    Player.setShootSound(shootSound);

    EnemyNumber = currentLevel * 2; // Số xe tăng địch tăng theo level
    spawnEnemyTanks(); // Tạo tank địch

    for(auto &enemy : EnemyTanks){
        enemy.setBulletTexture(enemyBulletTexture);
    }
    // Đặt lại trạng thái game
    gameOver = false;
    playerWon = false;
}

    void Game::loadBackgroundTexture(){
        SDL_Surface* bgSurface = IMG_Load("img/background.png");
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);

    }

    void Game::renderMenu(){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu đen
        SDL_RenderClear(renderer); // Xoá màn hình
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // Vẽ lên màn
        SDL_RenderPresent(renderer); // Cập nhật
    }

    void Game::generateWalls(){
            SDL_Surface *surface = IMG_Load("img/wall1.png");
            wallTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        // Vẽ tường tại các vị trí
        for(int i = 2; i < MAP_HEIGHT; i+=3){
            for(int j = 2; j < MAP_WIDTH ; j+=3){
                Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE, wallTexture);
                walls.push_back(w);
            }
        }
    }


    void Game::render(){
        SDL_SetRenderDrawColor(renderer,129,129,129,255); // Màu nền
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,0,0,0,255); // Màu cho các phần tử tiếp theo

        // Vẽ các tile
        for(int i=1;i<MAP_HEIGHT-1;i++){
            for(int j=1;j<MAP_WIDTH-1;j++){
                SDL_Rect tile = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(renderer, &tile);
            }
        }
        // Vẽ tường
        for(int i = 0; i < walls.size(); i++){
            walls[i].render(renderer);
        }
        // Vẽ người chơi
        Player.render(renderer);
        // Vẽ xe tăng địch
        for(auto &enemy : EnemyTanks){
            enemy.render(renderer);
        }
        renderExplosion();// Vẽ hiệu ứng vụ nổ

        // Vẽ thông tin level
        SDL_Color color = {255, 255, 255, 255}; // Màu chữ
        string levelText = "Level: " + to_string(currentLevel); // Tạo chuỗi hiện level
        SDL_Surface* surface = TTF_RenderText_Solid(font, levelText.c_str(), color); // Tạo surface từ chuỗi
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Tạo texture từ surface
        SDL_Rect rect = {10, 10, surface->w, surface->h}; // Vị trí vẽ
        SDL_RenderCopy(renderer, texture, NULL, &rect); // Vẽ
        SDL_FreeSurface(surface); //Giải phóng bộ nhớ
        SDL_DestroyTexture(texture);
        SDL_RenderPresent(renderer); // Cập nhật

    }
    void Game::loadBulletTextures(){
        SDL_Surface *playerBulletSurface = IMG_Load("img/playerBullet.png"); // Đọc ảnh
        playerBulletTexture = SDL_CreateTextureFromSurface(renderer, playerBulletSurface); // Tạo texture
        SDL_FreeSurface(playerBulletSurface); // Giải phóng bộ nhớ

        SDL_Surface *enemyBulletSurface = IMG_Load("img/enemyBullet.png");
        enemyBulletTexture = SDL_CreateTextureFromSurface(renderer, enemyBulletSurface);
        SDL_FreeSurface(enemyBulletSurface);
    }
    void Game::loadTankTexture(){
        SDL_Surface *playerTankSurface = IMG_Load("img/playerTank.png");
        playerTankTexture = SDL_CreateTextureFromSurface(renderer, playerTankSurface);
        SDL_FreeSurface(playerTankSurface);

        SDL_Surface *enemyTankSurface = IMG_Load("img/enemyTank.png");
        enemyTankTexture = SDL_CreateTextureFromSurface(renderer, enemyTankSurface);
        SDL_FreeSurface(enemyTankSurface);
    }

    // Tạo vụ nổ
    void Game::initExplosion(){
        SDL_Surface* surface = IMG_Load("img/explosion.png");
        explosionTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        showExplosion = false; // Lúc đầu chưa có vụ nổ
        explosionDuration = 15;
    }
    void Game::triggerExplosion(int x, int y){
        explosionRect = {x-25, y-25, 60, 60}; // Vị trí và kích thước vụ nổ
        showExplosion = true; // Có vụ nổ
        explosionCounter = 0;
    }
    void Game::updateExplosion(){
        if(!showExplosion) return;
        explosionCounter++;
        if(explosionCounter >= explosionDuration){
            showExplosion = false; // Hết thời gian hiện vụ nổ
        }
    }
    void Game::renderExplosion(){
        if (!showExplosion) return;
        SDL_RenderCopy(renderer, explosionTexture, NULL, &explosionRect);
    }
    // Cập nhật
    void Game::update(){
        updateExplosion();

        Player.updateBullets();

        for(auto &enemy : EnemyTanks){
            enemy.move(Player, walls);
            enemy.updateBullets();
            if(rand()% 100 < 2){ // Random enemy bắn
                enemy. shoot();
            }
        }
        // Va chạm giữa đạn của địch và tường
        for(auto &enemy : EnemyTanks){
            for(auto &bullet : enemy.Bullets){
                for(auto &wall : walls){
                    if(wall.active and SDL_HasIntersection(&bullet.rect, &wall.rect)){ // Nếu va
                        bullet.active = false; // Mất đạn
                        wall.active = false; // Mất tường
                        Mix_PlayChannel(-1, wallHitSound, 0); // Âm thanh va
                        break;
                    }
                }
            }
        }
        // Va chạm giữa đạn người chơi và tường
        for(auto &bullet : Player.Bullets){
            for(auto &wall : walls){
                if(wall.active and SDL_HasIntersection(&bullet.rect, &wall.rect)){
                    bullet.active = false;
                    wall.active = false;
                    Mix_PlayChannel(-1, wallHitSound, 0);
                    break;
                }
            }
        }
        // Va chạm giữa đạn người chơi và địch
        for(auto &bullet : Player.Bullets){
            for(auto &enemy : EnemyTanks){
                if(enemy.active and SDL_HasIntersection(&bullet.rect, &enemy.rect)){
                    triggerExplosion(enemy.rect.x + 25, enemy.rect.y + 25); // Kích hoạt vụ nổ với vị trí tương ứng
                    enemy.active = false;
                    bullet.active = false;
                    Mix_PlayChannel(-1, explosionSound, 0); // Âm thanh nổ
                }
            }
        }
        // Xoá các xe tăng địch bị phá huỷ
        EnemyTanks.erase(remove_if(EnemyTanks.begin(), EnemyTanks.end(),
                                   [](EnemyTank &e){return !e.active;}), EnemyTanks.end());

        // Khi tất cả địch bị tiêu diệt
        if(EnemyTanks.empty()){
            if(currentLevel < 3) {
            currentLevel++; // Tăng level
            resetLevel(); // reset cho level tiếp
        } else {
            gameOver = true; // Kết thúc game
            playerWon = true; // thắng
            }
        }
        // Khi đạn trúng người chơi
        for(auto &enemy : EnemyTanks){
            for(auto &bullet : enemy.Bullets){
                if(SDL_HasIntersection(&bullet.rect, &Player.rect)){
                    gameOver = true; // Kết thúc game
                    playerWon = false; // Thua
                    return;
                }
            }
        }
    }
    // Xử lí các sự kiện (di chuyển, bắn, thoát game)
    void Game::handleEvents(){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){ // Đóng cửa số thì thoát game
                running = false;
            }
            else if(event.type == SDL_KEYDOWN){
                if(inMenu){
                    if(event.key.keysym.sym == SDLK_s){
                        inMenu = false; // Nhấn "s" bắt đầu
                    }
                }
                else if(gameOver){
                    if(event.key.keysym.sym == SDLK_s){
                        running = false; // Nhấn "s" kết thúc
                    }
                     else if(event.key.keysym.sym == SDLK_r){
                        currentLevel = 1;
                        resetLevel(); // Nhấn "r" chơi lại từ đầu
                }
            }
            else{ // Nếu đang chơi
                int dx = 0, dy = 0;

                if(event.key.keysym.sym == SDLK_UP){
                    dy = -10;
                }
                else if(event.key.keysym.sym == SDLK_DOWN){
                    dy = 10;
                }
                else if(event.key.keysym.sym == SDLK_LEFT){
                    dx = -10;
                }
                else if(event.key.keysym.sym == SDLK_RIGHT){
                dx = 10;
                }
                else if(event.key.keysym.sym == SDLK_SPACE){
                Player.shot(); // Nhấn space bắn
                break;
            }
            Player.move(dx, dy, walls);
                }
            }
        }
    }


    void Game::spawnEnemyTanks(){
        EnemyTanks.clear();
        for(int i = 0; i < EnemyNumber; i++){
            int x, y;
            bool pos = false;
            // Kiểm tra vị trí có trùng với tường không
            while(!pos){
                x = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
                y = (rand() % (MAP_HEIGHT - 2)+ 1) * TILE_SIZE;
                pos = true;
                for(const auto &wall : walls){
                    if(wall.active and wall.x == x and wall.y == y){
                        pos = false;
                        break;
                    }
                }
            }
            // Tạo enemy với vị trí đã có
            EnemyTank newEnemy(x, y);
            newEnemy.setTankTexture(enemyTankTexture);
            newEnemy.setBulletTexture(enemyBulletTexture);
            EnemyTanks.push_back(newEnemy);
        }
    }

    // Kết thúc game
    void Game::loadEndGameTextures() {
    SDL_Surface* winSurface = IMG_Load("img/win.png");
    winTexture = SDL_CreateTextureFromSurface(renderer, winSurface);
    SDL_FreeSurface(winSurface);

    SDL_Surface* loseSurface = IMG_Load("img/lose.png");
    loseTexture = SDL_CreateTextureFromSurface(renderer, loseSurface);
    SDL_FreeSurface(loseSurface);

    //Tạo font
    TTF_Init();
    font = TTF_OpenFont("text/font.ttf", 25);
    }

    // Màn hình kết thúc game
    void Game::renderEndGame() {
    SDL_RenderClear(renderer);
    if(playerWon) {
        SDL_RenderCopy(renderer, winTexture, NULL, NULL);
    } else {
        SDL_RenderCopy(renderer, loseTexture, NULL, NULL);
    }
    // Vẽ thông báo chơi lại hoặc thoát game
    SDL_Color color = {255, 255, 255, 255};
    string instruction = "PRESS R TO RESTART OR S TO QUIT";
    SDL_Surface *surface = TTF_RenderText_Solid(font, instruction.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {SCREEN_WIDTH/2 - surface->w/2, SCREEN_HEIGHT/2 - surface->h/2 + 150, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
    }

    void Game::run(){
        while (running) {
            handleEvents();
            if(inMenu){
                renderMenu();
            }
            else if(gameOver) {
            renderEndGame();
            }
            else{
                update();
                render();
            }
            SDL_Delay(16);
        }
    }

    Game::~Game(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(wallTexture);
        SDL_DestroyTexture(playerBulletTexture);
        SDL_DestroyTexture(enemyBulletTexture);
        SDL_DestroyTexture(playerTankTexture);
        SDL_DestroyTexture(enemyTankTexture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(winTexture);
        SDL_DestroyTexture(loseTexture);
        Mix_FreeChunk(shootSound);
        Mix_FreeChunk(explosionSound);
        Mix_FreeChunk(wallHitSound);
        Mix_CloseAudio();
        SDL_Quit();
    }


