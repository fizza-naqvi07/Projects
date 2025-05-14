#ifndef FOOD.H
#define FOOd.H

#include <utility>

class Food {
private:
    std::pair<int, int> position;
    char symbol;

public:
    Food(char sym);
    void spawn(int maxWidth, int maxHeight);
    std::pair<int, int> getPosition() const;
    char getSymbol() const;
};

#endif;
