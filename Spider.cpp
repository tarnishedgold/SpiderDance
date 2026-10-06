#include "Spider.h"

    Spider::Spider(int currentLine, int direction, float speed, sf::Vector2f pos, float minX, float maxX)
        : currentLine(currentLine), direction(direction), speed(speed), minX(minX), maxX(maxX) {
        sprite.setPosition(pos);
    }

    void Spider::setTexture(const sf::Texture& texture) {
        const float SPRITE_SCALE = 2.8;
        const sf::Color SPRITE_TINT = sf::Color(255, 250, 250, 255);

        sprite.setTexture(texture);
        sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setColor(SPRITE_TINT);
    }

    void Spider::move(float deltaTime) {
        if (!isOutOfBounds()) {
            sprite.move(speed * deltaTime * direction, 0);
        }
    }

    void Spider::draw(sf::RenderWindow& window) const { window.draw(sprite); }

    float Spider::getSpritePosX() const { return sprite.getPosition().x; }

    int Spider::getCurrentLine() const { return currentLine; }

    bool Spider::isOutOfBounds() const  {
        const int DESPAWN_MARGIN = 42; // how far outside the screen a spider is removed

        return (sprite.getPosition().x < minX - DESPAWN_MARGIN || sprite.getPosition().x > maxX + DESPAWN_MARGIN);
    }
