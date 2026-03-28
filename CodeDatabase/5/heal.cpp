#include "heal.hpp"
#include "player.hpp"
#include <iostream>

HealSpell::HealSpell() 
    : Spell("Heal", "Restores 20 HP to player"), 
      healAmount(20) {}

void HealSpell::cast_spell(Player& player, int cordX, int cordY, 
        std::vector<Enemy>& enemies, const GameField& field) {
    int currentHP = player.get_hp();
    if (currentHP >= 100) {
        std::cout << "Already at full health!" << std::endl;
        return;
    }
    int healAmount = 20;
    int newHP = currentHP + healAmount;

    if (newHP > 100) {
        healAmount = 100 - currentHP; 
    }
    player.take_damage(-healAmount);
    std::cout << "Healed for " << healAmount << " HP!" << std::endl;
    std::cout << "HP: " << currentHP << " -> " << player.get_hp() << std::endl;
}

std::string HealSpell::getType() const {
    return "HEAL";
}