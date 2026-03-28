#pragma once
#include "Cell.h"
#include "Gamefield.h"
#include <SFML/Graphics.hpp>
#include <string>

class Entity {
protected:
    int hp;
    int x_pos;
    int y_pos;
    sf::Sprite sprite;
    sf::Texture texture;

public:
    virtual ~Entity() = default;

    virtual void move(const std::string& direction, Gamefield* gf);
    
    virtual void get_damaged(int dmg);

    virtual void onDeath() {
        sprite.setColor(sf::Color(255, 255, 255, 0));
    }

    sf::Sprite& getSprite() { return sprite; }
    const sf::Sprite& getSprite() const { return sprite; }

    int getX() const { return x_pos; }
    int getY() const { return y_pos; }
    int getHP() const { return hp; }
    bool isAlive() const { return hp > 0; }

    void setPosition(int x, int y) { 
        x_pos = x; 
        y_pos = y; 
        syncSprite(); 
    }

    void syncSprite();
};
