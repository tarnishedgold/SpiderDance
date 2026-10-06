#include "Player.h"

Player::Player(float speed, float lineCooldown, float minX, float maxX, int lineStep, int maxLine, sf::Vector2f startPos)
        : speed(speed), lineCooldown(lineCooldown), minX(minX), maxX(maxX), lineStep(lineStep), maxLine(maxLine) {
        sprite.setPosition(startPos);
    }

    void Player::setDirection(int currentDirection) {
        direction = currentDirection;
    }

    int Player::getDirection() {
        return direction;
    }

    void Player::setTexture(const sf::Texture& texture, int direction) {
        const float SPRITE_SCALE = 2.5;

        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setScale(SPRITE_SCALE * direction, SPRITE_SCALE);
    }

    void Player::setColor(sf::Uint8 color1, sf::Uint8 color2, sf::Uint8 color3, sf::Uint8 alpha) {
        sprite.setColor(sf::Color{ color1, color2, color3, alpha });
    }

    void Player::moveUp() {
        if (lineTimer.getElapsedTime().asSeconds() >= lineCooldown) {
            if (currentLine > 0) { currentLine--; sprite.move(0, -lineStep); }
            lineTimer.restart();
        }
    }

    void Player::moveDown() {
        if (lineTimer.getElapsedTime().asSeconds() >= lineCooldown) {
            if (currentLine < maxLine) { currentLine++; sprite.move(0, lineStep); }
            lineTimer.restart();
        }
    }

    void Player::moveLeft(float deltaTime) {
        if (sprite.getPosition().x > minX) {
            sprite.move(-speed * deltaTime, 0);
        }
    }

    void Player::moveRight(float deltaTime) {
        if (sprite.getPosition().x < maxX) {
            sprite.move(speed * deltaTime, 0);
        }
    }

    void Player::draw(sf::RenderWindow& window) { window.draw(sprite); }

    float Player::getSpritePosX() { return sprite.getPosition().x; }

    int Player::getCurrentLine() { return currentLine; }

    int Player::getHealth() { return health; }

    float Player::gethealthTimer() {
        return healthTimer.getElapsedTime().asSeconds();
    }

    float Player::gethealthCooldown() {
        return healthCooldown;
    }

    bool Player::getcanTakeDamage() {
        return canTakeDamage;
    }

    bool Player::getIsRestoring() {
        return isRestoring;
    }

    float Player::getRestoreProgress() {
        if (!isRestoring) { return 1.0; }
        if (restoreHealthClock.getElapsedTime().asSeconds() >= restoreHealthTimer) {
            return 1.0;
        }
        return restoreHealthClock.getElapsedTime().asSeconds() / restoreHealthTimer;
    }

    void Player::damage() {
        if (canTakeDamage) {
            health = std::max(0, health - 1);
            canTakeDamage = false;
            healthTimer.restart();
        }
    }

    bool Player::isDead() const { return health <= 0; }

    void Player::startHealthRestore() {
        isRestoring = true;
        restoreHealthClock.restart();
    }

    void Player::updateHealthRestore() {
        if (isRestoring) {
            if (restoreHealthClock.getElapsedTime().asSeconds() >= restoreHealthTimer) {
                if (health < MAX_HEALTH) health++;
                isRestoring = false;
            }
        }
    }

    void Player::updateCanTakeDamage() {
        if (health > 0 && healthTimer.getElapsedTime().asSeconds() >= healthCooldown) {
            canTakeDamage = true;
        }
    }