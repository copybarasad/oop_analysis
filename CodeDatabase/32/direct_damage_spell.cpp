#include "direct_damage_spell.hpp"
#include "enhancementspell.hpp"
#include "game.hpp"
#include "enemy.hpp"
#include "enemy_building.hpp"
#include "player.hpp"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int damage, int radius)
    : damage_(damage), radius_(radius) {}

std::string DirectDamageSpell::GetName() const noexcept {
    return "Огненный шар";
}

bool DirectDamageSpell::UseSpell(Game& game) {
    auto& player = game.GetPlayer();
    auto& enemies = game.GetEnemies();
    auto& buildings = game.GetBuildings();

    int effectiveRadius = EnhancementSpell::ApplyRadiusBonus(player, radius_);

    const int px = player.GetX();
    const int py = player.GetY();
    bool hit = false;

    for (auto& e : enemies) {
        if (!e.IsAlive()) continue;

        int dx = std::abs(e.GetX() - px);
        int dy = std::abs(e.GetY() - py);

        if (dx + dy <= effectiveRadius) {
            std::cout << "🔥 " << GetName() << " поражает врага "
                      << e.GetName() << " и наносит "
                      << damage_ << " урона!\n";
            e.TakeDamage(damage_);
            hit = true;
            break;
        }
    }

    if (!hit) {
        for (auto& b : buildings) {
            if (b.IsDestroyed()) continue;
            int dx = std::abs(b.GetX() - px);
            int dy = std::abs(b.GetY() - py);
            if ((dx == 0 || dy == 0) && dx + dy <= effectiveRadius) {
                std::cout << "🔥 " << GetName() << " поражает здание врага!\n";
                b.TakeDamage(damage_, game.GetField());
                hit = true;
                break;
            }
        }
    }

    if (!hit)
        std::cout << "💨 " << GetName() << " не нашёл цели в радиусе "
                  << effectiveRadius << ".\n";

    return hit;
}

bool DirectDamageSpell::UseSpell(Game& game, int originX, int originY) {
    auto& player = game.GetPlayer();
    int dx = std::abs(player.GetX() - originX);
    int dy = std::abs(player.GetY() - originY);

    if (dx + dy <= radius_) {
        std::cout << "🔥 " << GetName() << " поражает игрока и наносит "
                  << damage_ << " урона!\n";
        player.TakeDamage(damage_);
        return true;
    }

    std::cout << "💨 " << GetName() << " не нашёл цели в радиусе "
              << radius_ << ".\n";
    return false;
}

void DirectDamageSpell::Upgrade() {
    damage_ += 5;
    radius_ += 1;

    std::cout << "✨ Заклинание \"" << GetName()
              << "\" улучшено! Урон = " << damage_
              << ", радиус = " << radius_ << "\n";
}