
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <optional>

const int CELL_SIZE = 20;
const int WIDTH = 40;
const int HEIGHT = 30;
const int WINDOW_WIDTH = WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = HEIGHT * CELL_SIZE;

enum Direction { Up, Down, Left, Right };

struct Segment {
    int x, y;
    Segment(int x, int y) : x(x), y(y) {}
};

class Snake {
public:
    Snake(sf::Color color, int startX, int startY, Direction startDir)
        : color(color), direction(startDir), alive(true) {
        body.emplace_back(startX, startY);
    }

    void handleInput(Direction newDir) {
        if ((direction == Up && newDir != Down) ||
            (direction == Down && newDir != Up) ||
            (direction == Left && newDir != Right) ||
            (direction == Right && newDir != Left)) {
            direction = newDir;
        }
    }

    void move(bool grow = false) {
        Segment head = body.front();
        switch (direction) {
            case Up:    head.y -= 1; break;
            case Down:  head.y += 1; break;
            case Left:  head.x -= 1; break;
            case Right: head.x += 1; break;
        }
        body.push_front(head);
        if (!grow) body.pop_back();
    }

    void draw(sf::RenderWindow& window) {
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        rect.setFillColor(color);
        for (auto& s : body) {
            rect.setPosition(sf::Vector2f(s.x * CELL_SIZE, s.y * CELL_SIZE));
            window.draw(rect);
        }
    }

    bool checkSelfCollision() {
        Segment head = body.front();
        for (size_t i = 1; i < body.size(); ++i) {
            if (body[i].x == head.x && body[i].y == head.y) return true;
        }
        return false;
    }

    bool checkCollisionWithOther(const Snake& other) {
        Segment head = body.front();
        for (auto& s : other.body) {
            if (s.x == head.x && s.y == head.y) return true;
        }
        return false;
    }

    bool checkWallCollision() {
        Segment head = body.front();
        return head.x < 0 || head.y < 0 || head.x >= WIDTH || head.y >= HEIGHT;
    }

    bool isAlive() const { return alive; }
    void die() { alive = false; }
    Segment getHead() const { return body.front(); }
    void grow() { move(true); }

    std::deque<Segment> body;

private:
    Direction direction;
    sf::Color color;
    bool alive;
};

Segment generateFood(const Snake& s1, const Snake& s2) {
    Segment food(0, 0);
    while (true) {
        food.x = std::rand() % WIDTH;
        food.y = std::rand() % HEIGHT;
        bool valid = true;
        for (auto& s : s1.body) if (s.x == food.x && s.y == food.y) valid = false;
        for (auto& s : s2.body) if (s.x == food.x && s.y == food.y) valid = false;
        if (valid) break;
    }
    return food;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Snake Duel SFML3");
    window.setFramerateLimit(10);

    Snake snake1(sf::Color::Green, 5, 5, Right);
    Snake snake2(sf::Color::Blue, WIDTH - 6, HEIGHT - 6, Left);
    Segment food = generateFood(snake1, snake2);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        using K = sf::Keyboard::Key;

        if (sf::Keyboard::isKeyPressed(K::W)) snake1.handleInput(Up);
        if (sf::Keyboard::isKeyPressed(K::S)) snake1.handleInput(Down);
        if (sf::Keyboard::isKeyPressed(K::A)) snake1.handleInput(Left);
        if (sf::Keyboard::isKeyPressed(K::D)) snake1.handleInput(Right);

        if (sf::Keyboard::isKeyPressed(K::Up)) snake2.handleInput(Up);
        if (sf::Keyboard::isKeyPressed(K::Down)) snake2.handleInput(Down);
        if (sf::Keyboard::isKeyPressed(K::Left)) snake2.handleInput(Left);
        if (sf::Keyboard::isKeyPressed(K::Right)) snake2.handleInput(Right);

        if (snake1.isAlive()) snake1.move();
        if (snake2.isAlive()) snake2.move();

        if (snake1.getHead().x == food.x && snake1.getHead().y == food.y) {
            snake1.grow();
            food = generateFood(snake1, snake2);
        }
        if (snake2.getHead().x == food.x && snake2.getHead().y == food.y) {
            snake2.grow();
            food = generateFood(snake1, snake2);
        }

        if (snake1.checkWallCollision() || snake1.checkSelfCollision() || snake1.checkCollisionWithOther(snake2)) {
            snake1.die();
        }
        if (snake2.checkWallCollision() || snake2.checkSelfCollision() || snake2.checkCollisionWithOther(snake1)) {
            snake2.die();
        }

        window.clear();

        if (snake1.isAlive()) snake1.draw(window);
        if (snake2.isAlive()) snake2.draw(window);

        sf::RectangleShape foodShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        foodShape.setFillColor(sf::Color::Red);
        foodShape.setPosition(sf::Vector2f(food.x * CELL_SIZE, food.y * CELL_SIZE));
        window.draw(foodShape);

        window.display();
    }
    return 0;
}