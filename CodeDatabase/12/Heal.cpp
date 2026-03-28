#include "Heal.h"
#include "Player.h"
#include <iostream>

Heal::Heal(int amount) : HealAmount(amount) {}

void Heal::cast(Player& player, std::vector<Enemy>& enemies, Field& field) {
    player.heal(HealAmount);
    std::cout << "Игрок восстановил " << HealAmount << " HP!\n";
}

std::string Heal::getName() const {
    return "Лечение";
}
