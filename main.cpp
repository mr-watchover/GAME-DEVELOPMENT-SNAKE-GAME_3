#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int TILE_SIZE = 20;
const int WIDTH = 600;
const int HEIGHT = 400;

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
public:
    SnakeGame() {
        window.create(sf::VideoMode(WIDTH, HEIGHT), "Snake Game");
        srand(time(0));  // Initialize random seed
        reset();
    }

    void reset() {
        snake.clear();
        snake.push_front(sf::Vector2i(WIDTH / 2 / TILE_SIZE * TILE_SIZE, HEIGHT / 2 / TILE_SIZE * TILE_SIZE));
        direction = RIGHT;
        food = sf::Vector2i(rand() % (WIDTH / TILE_SIZE) * TILE_SIZE, rand() % (HEIGHT / TILE_SIZE) * TILE_SIZE);
        score = 0;
        gameOver = false;
    }

    void update() {
        if (gameOver) return;

        // Moving the snake
        sf::Vector2i head = snake.front();
        switch (direction) {
            case UP: head.y -= TILE_SIZE; break;
            case DOWN: head.y += TILE_SIZE; break;
            case LEFT: head.x -= TILE_SIZE; break;
            case RIGHT: head.x += TILE_SIZE; break;
        }

        // Check for collisions with walls or itself
        if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT || std::find(snake.begin(), snake.end(), head) != snake.end()) {
            gameOver = true;
            return;
        }

        snake.push_front(head);
        if (head == food) {
            // Eating food (increase score)
            score++;
            food = sf::Vector2i(rand() % (WIDTH / TILE_SIZE) * TILE_SIZE, rand() % (HEIGHT / TILE_SIZE) * TILE_SIZE);
        } else {
            snake.pop_back();
        }
    }

    void render() {
        window.clear(sf::Color::Black);

        // Draw snake
        for (const auto& segment : snake) {
            sf::RectangleShape rectangle(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            rectangle.setPosition(segment.x, segment.y);
            rectangle.setFillColor(sf::Color::Green);
            window.draw(rectangle);
        }

        // Draw food
        sf::RectangleShape foodRect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        foodRect.setPosition(food.x, food.y);
        foodRect.setFillColor(sf::Color::Red);
        window.draw(foodRect);

        // Draw score
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font\n";
        }
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        window.draw(scoreText);

        window.display();
    }

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && direction != DOWN) {
                    direction = UP;
                } else if (event.key.code == sf::Keyboard::Down && direction != UP) {
                    direction = DOWN;
                } else if (event.key.code == sf::Keyboard::Left && direction != RIGHT) {
                    direction = LEFT;
                } else if (event.key.code == sf::Keyboard::Right && direction != LEFT) {
                    direction = RIGHT;
                } else if (event.key.code == sf::Keyboard::Space && gameOver) {
                    reset();  // Restart the game
                }
            }
        }
    }

    bool isGameOver() {
        return gameOver;
    }

private:
    sf::RenderWindow window;
    std::deque<sf::Vector2i> snake;
    Direction direction;
    sf::Vector2i food;
    int score;
    bool gameOver;
};

int main() {
    SnakeGame game;

    sf::Clock clock;
    const float frameRate = 10.f;  // Snake speed (frames per second)

    while (game.isGameOver() == false) {
        game.handleInput();

        if (clock.getElapsedTime().asSeconds() > 1.f / frameRate) {
            game.update();
            clock.restart();
        }

        game.render();
    }

    return 0;
}
