#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"
#include "map.hpp"

#include <string>

class Hand;

enum class DamageType { 
    MELEE, 
    RANGE
};

class Player : public Entity {
private:
    DamageType dtype;
    Hand* hand;
    char last_step;
    int max_hp;
public:
    Player(int health, int damage, int max_hp);
    void setDamageType(DamageType newDamageType);
    DamageType getDamageType();
    void step(Map* map, int x, int y) override;
    void set_hand(Hand* hand_ptr);
    char get_last_step();
    virtual std::string getStatus() const override;
    void heal_hp();
    void increase_hp();
    void increase_damage();
};

#endif