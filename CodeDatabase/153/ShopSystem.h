#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include "Player.h"           
#include "Spell.h"            
#include "InputHandler.h"     
#include <vector>
#include <memory>

class ShopSystem {
private:
    std::vector<std::unique_ptr<Spell>> availableSpells;

    std::unique_ptr<Spell> createSpellCopy(Spell* baseSpell) const;
    bool processPlayerChoice(Player& player, int choice) const;

public:
    ShopSystem();

    bool buySpell(Player& player, int spellIndex) const;
    const std::vector<std::unique_ptr<Spell>>& getAvailableSpells() const;
    int calculateCost(Spell* spell) const;
    bool interactWithPlayer(Player& player, InputHandler& inputHandler) const;
};

#endif