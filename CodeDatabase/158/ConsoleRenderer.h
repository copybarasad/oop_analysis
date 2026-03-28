#pragma once

#include "Core/Map.h"
#include "Entities/Player.h"
#include "Input/KeyBinder.h"
#include <iostream>
#include <vector>

class ConsoleRenderer 
{
public:
    void render(const Map& map) const 
    {
        map.draw();
    }
    
    void renderUI(const Player& player, int level, const std::vector<std::pair<int, int>>& spells) const 
    {
        std::cout << "Level: " << level << " | HP: " << player.getHP() 
                  << "/" << player.getMaxHP() << " | Souls: " << player.getSouls() 
                  << " | Points: " << player.checkPoints() 
                  << " | Attack: " << (player.getAttackType() == ATKtype::melee ? "MELEE" : "RANGED") 
                  << " (Range: " << player.getAttackRange() << ", Damage: " << player.getAttackDamage() << ")\n";
        
        if(!spells.empty()) 
        {
            std::cout << "Spells: ";
            for(size_t i = 0; i < spells.size(); i++) 
            {
                std::cout << (i+1) << "." << spells[i].first << "(LvL" << spells[i].second << ") ";
            }
            std::cout << "\n";
        }
    }
    
    void renderControls(const KeyBinder& binder) const 
    {
        std::cout << "Controls: ";
        std::cout << binder.getKey(KeyBinder::MOVE_UP) << "-Up, ";
        std::cout << binder.getKey(KeyBinder::MOVE_DOWN) << "-Down, ";
        std::cout << binder.getKey(KeyBinder::MOVE_LEFT) << "-Left, ";
        std::cout << binder.getKey(KeyBinder::MOVE_RIGHT) << "-Right, ";
        std::cout << binder.getKey(KeyBinder::ATTACK) << "-Attack, ";
        std::cout << binder.getKey(KeyBinder::SWITCH_ATTACK) << "-Switch, ";
        std::cout << binder.getKey(KeyBinder::SHOP) << "-Shop, ";
        std::cout << binder.getKey(KeyBinder::UPGRADE) << "-Upgrade, ";
        std::cout << binder.getKey(KeyBinder::KEY_CONFIG) << "-Key Config\n";
        std::cout << "Save/Load: " << binder.getKey(KeyBinder::SAVE_MENU) << "-Save, ";
        std::cout << binder.getKey(KeyBinder::LOAD_MENU) << "-Load, ";
        std::cout << "Animations: " << binder.getKey(KeyBinder::TOGGLE_ANIMATIONS) << "-Toggle (currently " 
                  << (enable_animations ? "ON" : "OFF") << ")\n";
        std::cout << "Menu: " << binder.getKey(KeyBinder::BACK_TO_MENU) << "-Back to Menu\n";
    }
    
    void renderMessage(const std::string& message) const 
    {
        std::cout << message << "\n";
    }
};

extern bool enable_animations;