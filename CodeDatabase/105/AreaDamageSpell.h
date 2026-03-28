#pragma once

#include "Spell.h"
#include "GameVisualize.h"
#include <string>

class GameField;
class Player;

class AreaDamageSpell : public Spell {
private:
    std::string name;
    std::string description;
    int manaCost;
    int damage;
    int range;
    int areaSize;
    bool enhanced;
    ConsoleGameVisualizer visualizer;

public:
    AreaDamageSpell(int damage = 25, int range = 2, int areaSize = 2,
                   const std::string& name = "Firestorm", 
                   const std::string& description = "Deals damage in an area",
                   int manaCost = 30);
    
    bool cast(GameField& field, Player& player, int targetX, int targetY) override;
    bool requiresTarget() const override { return true; }
    
    const std::string& getName() const override { return name; }
    const std::string& getDescription() const override { return description; }
    int getManaCost() const override { return manaCost; }
    
    void enhance(int damageBonus = 5, int areaBonus = 1);

    // Методы для улучшения
    void increaseDamage(int amount);
    void increaseArea(int amount);
    int getCurrentDamage() const { return damage; }
    int getCurrentAreaSize() const { return areaSize; }
}; 