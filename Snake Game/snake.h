#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <utility>

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake
{
private:
    std::deque<std::pair<int, int>> body;
    Direction direction;
    char symbol;
    int score;

public:
    Snake(int startX, int startY, char sym);
    void move();
    void grow();
    void setDirection(Direction dir);
    std::pair<int, int> getHead() const;
    bool isAt(int x, int y) const;
    const std::deque<std::pair<int, int>> &getBody() const;
    Direction getDirection() const;
    int getScore() const;
};

#endif;