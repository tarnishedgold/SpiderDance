#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    // Player state
    sf::Sprite sprite;
    int currentLine = 1;        // starts on the middle line
    int direction = 1;          // starts facing right
    int health = 2;
    int MAX_HEALTH = 2;

    // Rules provided by Game
    float speed;
    float lineCooldown;
    float minX, maxX;
    int lineStep;               // vertical distance between lines
    int maxLine;                // index of the lowest line
    float healthCooldown = 3;   // invulnerability time after a hit, seconds
    float restoreHealthTimer = 10; // time to regenerate one health point, seconds
    bool isRestoring = false;

    // Helpers
    sf::Clock lineTimer;
    sf::Clock healthTimer;
    sf::Clock restoreHealthClock;
    bool canTakeDamage = true;

public:

    Player(float speed, float lineCooldown, float minX, float maxX, int lineStep, int maxLine, sf::Vector2f startPos);

    void setDirection(int currentDirection);

    int getDirection();

    void setTexture(const sf::Texture& texture, int direction);

    void setColor(sf::Uint8 color1, sf::Uint8 color2, sf::Uint8 color3, sf::Uint8 alpha);

    void moveUp();

    void moveDown();

    void moveLeft(float deltaTime);

    void moveRight(float deltaTime);

    void draw(sf::RenderWindow& window);

    float getSpritePosX();

    int getCurrentLine();

    int getHealth();

    float gethealthTimer();

    float gethealthCooldown();

    bool getcanTakeDamage();

    bool getIsRestoring();

    float getRestoreProgress();

    void damage();

    bool isDead() const;

    void startHealthRestore();

    void updateHealthRestore();

    void updateCanTakeDamage();
};