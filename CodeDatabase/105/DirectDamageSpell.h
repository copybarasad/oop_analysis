#pragma once

#include "Spell.h"
#include "GameVisualize.h"
#include <string>

class GameField;
class Player;

class DirectDamageSpell : public Spell {
private:
    std::string name;
    std::string description;
    int manaCost;
    int damage;
    int range;
    bool enhanced;
    ConsoleGameVisualizer visualizer;

public:
    DirectDamageSpell(int damage = 30, int range = 3, 
                     const std::string& name = "Fireball", 
                     const std::string& description = "Deals direct damage to a single target",
                     int manaCost = 20);
    
    bool cast(GameField& field, Player& player, int targetX, int targetY) override;
    bool requiresTarget() const override { return true; }
    
    const std::string& getName() const override { return name; }
    const std::string& getDescription() const override { return description; }
    int getManaCost() const override { return manaCost; }
    
    void setDamage(int newDamage);
    void setRange(int newRange);
    void enhance(int damageBonus = 10, int rangeBonus = 2);

     // Методы для улучшения
    void increaseDamage(int amount);
    void increaseRange(int amount);
    int getCurrentDamage() const { return damage; }
    int getCurrentRange() const { return range; }
};
