#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "wall.h"
#include "bullet.h"
#include "PlayerTank.h"
#include "enemyTank.h"
#include "game.h"
#include "defs.h"

using namespace std;


int main(int argc, char *argv[]){
    Game game;
    if(game.running){
         game.run();
    }
    return 0;
}

