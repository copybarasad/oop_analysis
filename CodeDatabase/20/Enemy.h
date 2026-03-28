#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Gamefield.h"
#include "Player.h"

class Enemy : public Entity {
private:
    int damage;
    
public:
    Enemy(int hp, int dmg, int x, int y);
    Enemy();

    Enemy(const Enemy& other);

    Enemy& operator=(const Enemy& other);

    const sf::Sprite& getSprite() const { return sprite; }
    void setDamage(int dmg) { damage = dmg; }
    int getDamage() const { return damage; }

    void move(const std::string& direction, Gamefield* gf) override;
};
