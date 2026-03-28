#ifndef GAME_PLAYERUPGREDE_H
#define GAME_PLAYERUPGREDE_H

#include <unordered_map>
#include <functional>
#include "../Entities/Player.h"

class PlayerUpgrade {
private:
    Player *pl_ = nullptr;
public:
    std::unordered_map<std::string, std::function<void()>> upgrades_;

    explicit PlayerUpgrade() {
        upgrades_ = {
            {"More health", [this]() { this->addHealth(); }},
            {"Refresh mana", [this]() { this->refreshMana(); }},
            {"Add two random spells", [this]() { this->addSpells(); }},
            {"Add two levels to next spell", [this]() { this->addLevels(); }},
        };
    }

    void setPlayer(Player *pl);

    void addHealth() const;

    void refreshMana() const;

    void addSpells() const;

    void addLevels() const;

    void applyUpgrade(const std::string& name);

};


#endif
