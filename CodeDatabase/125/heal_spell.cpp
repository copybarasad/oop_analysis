#include "heal_spell.h"
#include "player.h"
#include <algorithm>
#include <iostream>

void HealSpell::Cast(Player& player, EnemyManager&, GameField&) {
    int heal = 30;
    int old_hp = player.GetHealth();
    player.TakeDamage(-heal); 
    int healed = player.GetHealth() - old_hp;
    std::cout << "Вы восстановили " << healed << " HP!\n";
}