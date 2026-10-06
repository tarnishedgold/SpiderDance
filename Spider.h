#pragma once
#include <SFML/Graphics.hpp>

class Spider {
private:
    sf::Sprite sprite;
    int direction;
    int currentLine;
    float speed;
    float minX, maxX;

public:
    Spider(int currentLine, int direction, float speed,
           sf::Vector2f pos, float minX, float maxX);

    void setTexture(const sf::Texture& texture);
    void move(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    float getSpritePosX() const;
    int getCurrentLine() const;
    bool isOutOfBounds() const;

};