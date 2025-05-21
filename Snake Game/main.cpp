#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdlib>
#include <windows.h> 

// Snake Class
class Snake {
private:
    int* bodyX;
    int* bodyY;
    int size;
    int maxSize;
    int direction;
    int score;

public:
    Snake(int startX, int startY, int maxBodySize = 100) {
        size = 1;
        maxSize = maxBodySize;
        bodyX = new int[maxSize];
        bodyY = new int[maxSize];
        bodyX[0] = startX;
        bodyY[0] = startY;
        direction = 1; // Start moving to the right (1 = right, 0 = up, 2 = down, 3 = left)
        score = 0;
    }

    ~Snake() {
        delete[] bodyX;
        delete[] bodyY;
    }

    void move() {
        for (int i = size; i > 0; --i) {
            bodyX[i] = bodyX[i - 1];
            bodyY[i] = bodyY[i - 1];
        }
        switch (direction) {
            case 0: bodyY[0]--; break; // Up
            case 1: bodyX[0]++; break; // Right
            case 2: bodyY[0]++; break; // Down
            case 3: bodyX[0]--; break; // Left
        }
    }

    void grow() {
        if (size < maxSize) {
            size++;
            score++;
        }
    }

    void changeDirection(int newDir) {
        if ((direction == 0 && newDir != 2) || (direction == 1 && newDir != 3) ||
            (direction == 2 && newDir != 0) || (direction == 3 && newDir != 1)) {
            direction = newDir;
        }
    }

    bool checkSelfCollision() const {
        for (int i = 1; i < size; i++) {
            if (bodyX[i] == bodyX[0] && bodyY[i] == bodyY[0]) {
                return true;
            }
        }
        return false;
    }

    bool isAt(int x, int y) const {
        for (int i = 0; i < size; i++) {
            if (bodyX[i] == x && bodyY[i] == y) {
                return true;
            }
        }
        return false;
    }

    void getHead(int &x, int &y) const {
        x = bodyX[0];
        y = bodyY[0];
    }

    void getBody(int index, int &x, int &y) const {
        if (index < size) {
            x = bodyX[index];
            y = bodyY[index];
        }
    }

    int getScore() const {
        return score;
    }
};

// Food Class
class Food {
private:
    int x, y;

public:
    Food() : x(10), y(10) {}

    void spawn(int maxX, int maxY) {
        x = rand() % maxX;
        y = rand() % maxY;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
};

// Game Class
class Game {
private:
    Snake snake1;
    Snake snake2;
    Food food;
    int width, height;
    bool gameOver;
    sf::RenderWindow window;

public:
    Game() : snake1(10, 10), snake2(30, 30), width(20), height(20), gameOver(false), window(sf::VideoMode(400, 400), "Snake Game") {
        food.spawn(width, height);
        window.setFramerateLimit(10);  // Set frame rate to 10 FPS
    }

    void run() {
        while (!gameOver) {
            processEvents();
            update();
            render();
            Sleep(100); // Control game speed
        }
    }

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                // Controls for snake1 (WASD)
                if (event.key.code == sf::Keyboard::W) snake1.changeDirection(0); // Up
                if (event.key.code == sf::Keyboard::D) snake1.changeDirection(1); // Right
                if (event.key.code == sf::Keyboard::S) snake1.changeDirection(2); // Down
                if (event.key.code == sf::Keyboard::A) snake1.changeDirection(3); // Left

                // Controls for snake2 (Arrow Keys)
                if (event.key.code == sf::Keyboard::Up) snake2.changeDirection(0); // Up
                if (event.key.code == sf::Keyboard::Right) snake2.changeDirection(1); // Right
                if (event.key.code == sf::Keyboard::Down) snake2.changeDirection(2); // Down
                if (event.key.code == sf::Keyboard::Left) snake2.changeDirection(3); // Left
            }
        }
    }

    void update() {
        snake1.move();
        snake2.move();

        // Check if snake1 eats food
        int headX1, headY1;
        snake1.getHead(headX1, headY1);
        if (headX1 == food.getX() && headY1 == food.getY()) {
            snake1.grow();
            food.spawn(width, height);
        }

        // Check if snake2 eats food
        int headX2, headY2;
        snake2.getHead(headX2, headY2);
        if (headX2 == food.getX() && headY2 == food.getY()) {
            snake2.grow();
            food.spawn(width, height);
        }

        // Check if snakes hit themselves or the boundary
        if (snake1.checkSelfCollision() || snake2.checkSelfCollision()) {
            gameOver = true;
        }
    }

    void render() {
        window.clear();

        sf::RectangleShape block(sf::Vector2f(20.f, 20.f));

        // Draw snake1
        block.setFillColor(sf::Color::Green);
        for (int i = 0; i < snake1.getScore(); i++) {
            int x, y;
            snake1.getBody(i, x, y);
            block.setPosition(x * 20.f, y * 20.f);
            window.draw(block);
        }

        // Draw snake2
        block.setFillColor(sf::Color::Blue);
        for (int i = 0; i < snake2.getScore(); i++) {
            int x, y;
            snake2.getBody(i, x, y);
            block.setPosition(x * 20.f, y * 20.f);
            window.draw(block);
        }

        // Draw food
        block.setFillColor(sf::Color::Red);
        block.setPosition(food.getX() * 20.f, food.getY() * 20.f);
        window.draw(block);

        window.display();
    }
};

// Main function
int main() {
    Game game;
    game.run();
    return 0;
}
