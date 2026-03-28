#include "enemy_tower.hpp"
#include "game.hpp"
#include "direct_damage_spell.hpp"
#include <iostream>
#include <cmath>


void EnemyTower::Tick() {
    CooldownTick();
    if (!IsReady()) return;

    Player& player = game_->GetPlayer();

    int px = player.GetX();
    int py = player.GetY();

    // Башня атакует только если игрок ВПЛОТНУЮ
    int dist = std::abs(x_ - px) + std::abs(y_ - py);
    if (dist != 1) return;

    std::cout << "🏰 Башня (" << x_ << "," << y_
              << ") замечает игрока вблизи и готовит ослабленное заклинание!\n";

    int weakenedDamage = std::max(1, damage_ / 2);
    int spellRadius = 1;
    DirectDamageSpell spell(weakenedDamage, spellRadius);

    std::cout << "🔥 Башня выпускает " << spell.GetName()
              << " силой " << weakenedDamage << "!\n";

    spell.UseSpell(*game_, x_, y_);

    cooldown_ = 2;
}