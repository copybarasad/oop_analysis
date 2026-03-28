#pragma once
#include "spell.h"
#include <string>

class Field;
class Player;
class EnemyManager;

class BaseSpell : public Spell {
protected:
    std::string name;
    int damage;
    int cost;
    int range;
    
    bool checkResources(int x, int y, Field& field, Player& player, std::string& message);
    void removeCash(Player& player);
    
public:
    BaseSpell(int dmg, int cost, int radius, std::string spellName);
    
    std::string getName() const override;
    int getCost() const override;
    int getRange() const override;
    int getDamage() const;
};