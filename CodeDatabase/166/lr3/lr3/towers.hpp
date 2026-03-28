#ifndef TOWERS_HPP_INCLUDED
#define TOWERS_HPP_INCLUDED

#include "position.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "ally.hpp"
#include "spells/direct_damage_spell.hpp"

#include <vector>
#include <cmath>

class Tower {
public:
    Tower() = default;
    Tower(Position p, int dmg, int radius)
        : pos_(p), damage_(dmg), attackRadius_(radius), cooldown_(0) {}

    const Position& getPos() const noexcept { return pos_; }
    void setPos(const Position& p) noexcept { pos_ = p; }

    int  getDamage() const noexcept { return damage_; }
    void setDamage(int v) noexcept { damage_ = v; }

    int  getAttackRadius() const noexcept { return attackRadius_; }
    void setAttackRadius(int v) noexcept { attackRadius_ = v; }

    int  getCooldown() const noexcept { return cooldown_; }
    void setCooldown(int v) noexcept { cooldown_ = v; }

    void onTurn() noexcept { if (cooldown_ > 0) --cooldown_; } //если на перезарядке, то --кд 

    // Башня атакует: приоритет Ally в радиусе, затем герой. Урон наносится DirectDamageSpell'ом (как источник эффекта),
    // а затем применяем к цели (ally/hero) вручную, т.к. DirectDamageSpell по своей природе бьёт только врагов.
    bool tryAttack(
        Hero& hero,
        Field& field,
        std::vector<Enemy>& enemies,
        std::vector<Spawner>& spawners,
        std::vector<Ally>& allies, std::vector<Position>* to_redraw = nullptr
    ) {
        if (cooldown_ > 0) return false;

        auto manh = [](const Position& a, const Position& b){
            return std::abs(a.getX() - b.getX()) + std::abs(a.getY() - b.getY());
        };

        // 1) Ally в радиусе
        int bestIdx = -1, bestD = 1e9;
        for (int i = 0; i < (int)allies.size(); ++i) {
            if (allies[i].isDead()) continue;
            int d = manh(pos_, allies[i].getPos());
            if (d <= attackRadius_ && d < bestD) { bestD = d; bestIdx = i; } //бьет ближнего А
        }

        if (bestIdx >= 0) {
            Position target = allies[bestIdx].getPos();
            DirectDamageSpell spell(std::max(1, damage_), attackRadius_);
            BoostState neutral{};
            Hero caster(1, std::max(1, damage_), pos_); // кастер в позиции башни, чтобы прошла проверка радиуса
            (void)spell.cast(caster, field, enemies, spawners, neutral, target);

            allies[bestIdx].takeDamage(damage_);
            if (to_redraw) to_redraw->push_back(target);
            cooldown_ = 4;
            return true;
        }

        // 2) Герой в радиусе
        if (manh(pos_, hero.getPosition()) <= attackRadius_) {
            Position target = hero.getPosition();
            DirectDamageSpell spell(std::max(1, damage_), attackRadius_);
            BoostState neutral{}; //бусты героя не должны переходить в бусты башни
            Hero caster(1, std::max(1, damage_), pos_);
            (void)spell.cast(caster, field, enemies, spawners, neutral, target);

            hero.takeDamage(damage_);
            if (to_redraw) to_redraw->push_back(target);
            cooldown_ = 4;
            return true;
        }

        return false;
    }

private:
    Position pos_{};
    int damage_{0};
    int attackRadius_{0};
    int cooldown_{0};
};

#endif // TOWERS_HPP_INCLUDED
