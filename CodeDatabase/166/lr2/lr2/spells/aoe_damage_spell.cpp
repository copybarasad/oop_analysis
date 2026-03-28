#include "spells/aoe_damage_spell.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"

bool AoEDamageSpell::cast(Hero& hero,
                          Field& field,
                          std::vector<Enemy>& enemies,
                          std::vector<Spawner>& spawners,
                          BoostState& boosts,
                          const Position& target)
{
    (void)field;    // параметр не используется в этой реализации
    (void)spawners; // тоже не нужен

    // сторона квадрата: 2 + буст
    int side = 2 + boosts.aoeSizePlus;
    // радиус до левого-верхнего угла области
    int r = baseRad_ + boosts.directRadiusPlus;

    // проверяем расстояние манхэттеном
    int dx = std::abs(hero.getPosition().getX() - target.getX());
    int dy = std::abs(hero.getPosition().getY() - target.getY());
    if (dx + dy > r) return false;

    // наносим урон всем врагам в квадрате side x side
    for (auto& e: enemies) {
        if (e.isDead()) continue;
        const auto pos = e.getPosition();
        if (pos.getX() >= target.getX() && pos.getX() < target.getX() + side &&
            pos.getY() >= target.getY() && pos.getY() < target.getY() + side) {
            e.takeDamage(dmg_);
        }
    }
    boosts.clear();
    return true;
}