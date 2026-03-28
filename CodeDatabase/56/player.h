
#ifndef PLAYER_H
#define PLAYER_H

#include "game_objects.h"
#include "hand_spells.h"
#include <nlohmann/json.hpp>
#include "random.h"
class Player : public GameObject {
private:
    int damage;
    int Points;
    int mana;
    Hand hand;
    int max_hp;
public:
    Player(int X, int Y, int hp, int dam, int point, int m, int max_HP);
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    nlohmann::json toJson();
    void fromJson(const nlohmann::json& j);

    void upPlayer();
    int getMaxHP();
    int getDamage() ;
    int getPoints();
    void setDamage(int new_damage);
    void setRange(int new_range);
    void reduceMana(int amount);
    void setMana(int new_mana);
    Hand& getHand();
    void addKill();
    int getMana();
    void setHP(int hp);
    void setHand(Hand&& hands);
};

#endif