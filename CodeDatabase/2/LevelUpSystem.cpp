#include "LevelUpSystem.hpp"

LevelUpSystem::LevelUpSystem() : availablePoints(0) {}

void LevelUpSystem::addLevelUpPoints(int points) { 
    availablePoints += points; 
    std::cout << "Received " << points << " level up points! Total: " << availablePoints << std::endl;
}

bool LevelUpSystem::improvePlayerHealth(Player& player) {
    if (availablePoints > 0) {
        player.set_max_hp(player.get_max_hp() + 5);
        player.set_hp(player.get_max_hp()); // Полное восстановление
        availablePoints--;
        std::cout << "Health improved! Max HP: " << player.get_max_hp() << std::endl;
        return true;
    }
    return false;
}

bool LevelUpSystem::improvePlayerMana(Player& player) {
    if (availablePoints > 0) {
        player.set_max_mana(player.get_max_mana() + 5);
        player.set_mana(player.get_max_mana()); // Полное восстановление
        availablePoints--;
        std::cout << "Mana improved! Max Mana: " << player.get_max_mana() << std::endl;
        return true;
    }
    return false;
}

bool LevelUpSystem::improvePlayerDamage(Player& player) {
    if (availablePoints > 0) {
        player.increase_damage(1);
        availablePoints--;
        std::cout << "Damage improved!" << std::endl;
        return true;
    }
    return false;
}

int LevelUpSystem::getAvailablePoints() const { 
    return availablePoints; 
}

void LevelUpSystem::reset() {
    availablePoints = 0;
}