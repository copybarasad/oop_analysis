#pragma once
#include "Entity.h"
#include "Cell.h"
#include "Enemy.h"
#include "Spell.h"
#include <memory>
#include <stack>

class Hand;

class Player : public Entity {
private:
    enum class Weapon{melee, range, hand};
    Weapon current_weapon;
    int damage;
    Hand* hand;
    int moves;
    int max_hp = hp;



public:
    sf::Clock moveClock;
    float moveDelay = 0.15f; 

    Player(const Player& other);
    Player& operator=(const Player& other);


    int score;

    Player(int hp, int damage, int x, int y);
    Player();

    void move(const std::string& direction, Gamefield* gf) override;

    Hand* getHand() const { return hand; }
    void setHP(int new_hp) { hp = new_hp; }
    void weapon_change();
    void print_weapon();
    void cast_spell(std::string direction, Gamefield* gf);
    int getMaxHP() const { return max_hp; }
};
