#include "player.h"
#include "spell_factory.h"

#include <random>

namespace game {
    // ------------ Attack character (enemy) --------------
    bool Player::attack(Character& enemy, const GameConfig& cfg, std::mt19937& rng)
    {
        if (!enemy.isAlive()) return false;

        int finalDamage = damage_;
        if (isCriticalHit(cfg, rng)) {
            finalDamage *= cfg.critMultiplier;
        }

        enemy.takeDamage(finalDamage);
        return true;
    }

    // --------------- Is critical damage? ----------------
    bool Player::isCriticalHit(const GameConfig& cfg, std::mt19937& rng) const 
    {
        std::uniform_int_distribution<int> chance(0, 99);
        return chance(rng) < cfg.critChance;
    }

    // ------------ Move player at direction --------------
    Pos Player::move(Direction dir, int w, int h) 
    {
        Pos step = Pos::toPos(dir);

        int nx = position_.x + step.x;
        int ny = position_.y + step.y;

        if (nx < 0 || nx >= w || ny < 0 || ny >= h)
            return position_;   // no movement

        position_.x = nx;
        position_.y = ny;

        return position_;
    }

    // ------------ Switch player fight mode --------------
    void Player::switchFightMode() 
    {
        if (getFightMode() == FightMode::Melee) {
            fmode_ = FightMode::Ranged;
            damage_ = rangedDamage_;
        } else {
            fmode_ = FightMode::Melee;
            damage_ = meleeDamage_;
        }
    }
}