#ifndef UTILS_H
#define UTILS_H

#include <utility>
#include <vector>
#include <deque>

namespace Utils {
    std::pair<int, int> generateFoodPosition(int width, int height, const std::vector<std::deque<std::pair<int, int>>>& snakes);
    void clearScreen();
    void delay(int milliseconds);
}

#endif
