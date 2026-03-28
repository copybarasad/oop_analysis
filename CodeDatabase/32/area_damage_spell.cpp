#include "area_damage_spell.hpp"
#include "enhancementspell.hpp"
#include "game.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy_building.hpp"
#include "field.hpp"
#include <iostream>
#include <cmath>
#include "constants.hpp"
AreaDamageSpell::AreaDamageSpell(int damage, int radius)
    : damage_(damage), radius_(radius) {}

std::string AreaDamageSpell::GetName() const noexcept {
    return "Грозовая буря (урон по области)";
}

static bool InRadius(int sx, int sy, int tx, int ty, int radius) {
    return std::abs(sx - tx) + std::abs(sy - ty) <= radius;
}

bool AreaDamageSpell::UseSpell(Game& game) {
    auto& player = game.GetPlayer();
    auto& enemies = game.GetEnemies();
    auto& buildings = game.GetBuildings();
    auto& field = game.GetField();

    int px = player.GetX();
    int py = player.GetY();

    int effectiveArea = EnhancementSpell::ApplyAreaBonus(player, AREA_SIZE);

    std::cout << "Выберите направление удара (w/a/s/d): ";
    char dir;
    std::cin >> dir;

    int tx = px, ty = py;
    switch (dir) {
        case DIR_UP:    ty -= radius_; break;
        case DIR_DOWN:  ty += radius_; break;
        case DIR_LEFT:  tx -= radius_; break;
        case DIR_RIGHT: tx += radius_; break;
        default:
            std::cout << "Некорректное направление!\n";
            return false;
    }

    if (!InRadius(px, py, tx, ty, radius_)) {
        std::cout << "⚡ Цель вне радиуса применения заклинания.\n";
        return false;
    }

    if (tx < FIELD_MIN || ty < FIELD_MIN ||
        tx + OFFSET >= field.width() || ty + OFFSET >= field.height()) {
        std::cout << "⚡ Область выходит за границы поля — заклинание не сработало.\n";
        return false;
    }

    std::cout << "⚡ " << GetName() << " поражает область "
              << effectiveArea << "x" << effectiveArea << "!\n";

    bool hitSomething = false;

    for (int dy = 0; dy < effectiveArea; ++dy) {
        for (int dx = 0; dx < effectiveArea; ++dx) {
            int cx = tx + dx;
            int cy = ty + dy;

            for (auto& e : enemies) {
                if (e.IsAlive() && e.GetX() == cx && e.GetY() == cy) {
                    std::cout << "⚡ Враг " << e.GetName()
                              << " получает " << damage_ << " урона!\n";
                    e.TakeDamage(damage_);
                    hitSomething = true;
                }
            }

            for (auto& b : buildings) {
                if (!b.IsDestroyed() && b.GetX() == cx && b.GetY() == cy) {
                    std::cout << "⚡ Здание врага получает "
                              << damage_ << " урона!\n";
                    b.TakeDamage(damage_, field);
                    hitSomething = true;
                }
            }
        }
    }

    if (!hitSomething)
        std::cout << "💨 В области никого нет — заклинание потрачено впустую.\n";

    return true;
}

void AreaDamageSpell::Upgrade() {
    damage_ += 3;
    radius_ += 1;
    std::cout << "⛈ Область и урон заклинания увеличены!\n";
}