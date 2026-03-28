#include "spells/direct_damage_spell.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"

bool DirectDamageSpell::cast(Hero& hero,
                             Field& field,
                             std::vector<Enemy>& enemies,
                             std::vector<Spawner>& spawners,
                             BoostState& boosts,
                             const Position& target)
{
    (void)field;    // не используется в этой реализации
    (void)spawners; // не используется

    // радиус применения с учётом буста
    int r = baseRad_ + boosts.directRadiusPlus;

    // проверяем, что цель в радиусе манхэттеном
    int dx = std::abs(hero.getPosition().getX() - target.getX());
    int dy = std::abs(hero.getPosition().getY() - target.getY());
    if (dx + dy > r) {
        // вне радиуса — не применилось
        return false;
    }

    // пытаемся нанести урон существу в точке
    bool hit = false;
    for (auto& e: enemies) {
        if (!e.isDead() && e.getPosition() == target) {
            e.takeDamage(dmg_);
            hit = true;
            break;
        }
    }

    // важное изменение: direct расходуется даже по пустой клетке (если цель в радиусе)
    // поэтому возвращаем true независимо от того, попали или нет
    boosts.clear();
    (void)hit;
    return true;
}