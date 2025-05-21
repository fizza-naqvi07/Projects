#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "Food.h"
#include <vector>

class Game {
private:
    const int width = 20;
    const int height = 20;
    bool gameOver;

    Snake snake1;
    Snake snake2;
    Food food;

public:
    Game();
    void draw();
    void run();
    void update();
    void processInput();
    void checkCollisions();
};

#endif
