#include "enhancementspell.hpp"
#include "game.hpp"
#include "player.hpp"
#include <iostream>

bool EnhancementSpell::UseSpell(Game& game) {
    auto& player = game.GetPlayer();
    player.ActivateEnhancement();
    std::cout << "💫 Следующее заклинание будет усилено!\n";
    return true;
}


int EnhancementSpell::ApplyRadiusBonus(Player& player, int baseRadius) {
    if (player.HasEnhancement()) {
        int newRadius = baseRadius + 1;
        std::cout << "💫 Усиление активно! Радиус увеличен до "
                  << newRadius << "!\n";
        player.ConsumeEnhancement();
        return newRadius;
    }
    return baseRadius;
}

int EnhancementSpell::ApplyAreaBonus(Player& player, int baseArea) {
    if (player.HasEnhancement()) {
        int newArea = baseArea + 1;
        std::cout << "⚡ Усиление активно! Размер области увеличен до "
                  << newArea << "x" << newArea << "!\n";
        player.ConsumeEnhancement();
        return newArea;
    }
    return baseArea;
}

int EnhancementSpell::ApplyDamageBonus(Player& player, int baseDamage, int bonus) {
    if (player.HasEnhancement()) {
        int newDamage = baseDamage + bonus;
        std::cout << "💥 Усиление активно! Урон увеличен до "
                  << newDamage << "!\n";
        player.ConsumeEnhancement();
        return newDamage;
    }
    return baseDamage;
}

int EnhancementSpell::ApplySummonBonus(Player& player, int baseCount) {
    if (player.HasEnhancement()) {
        int newCount = baseCount + 1;
        std::cout << "💫 Усиление активно! Будет призвано "
                  << newCount << " союзника!\n";
        player.ConsumeEnhancement();
        return newCount;
    }
    return baseCount;
}

void EnhancementSpell::Upgrade() {
    std::cout << "💫 Усилено: Enhancement усиляет любые заклинания вдвое!\n";
    power_multiplier_ += 1; // нужно добавить это поле
}