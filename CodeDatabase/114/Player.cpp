#include "Player.h"
#include <array>
#include <random>

namespace {
SpellType getRandomSpell() {
    static std::mt19937 gen(std::random_device{}());
    constexpr std::array<SpellType, 5> available{
        SpellType::DIRECT_DAMAGE,
        SpellType::AREA_DAMAGE,
        SpellType::TRAP,
        SpellType::IMPROVE,
        SpellType::SUMMON
    };
    std::uniform_int_distribution<std::size_t> dist(0, available.size() - 1);
    return available[dist(gen)];
}
}

Player::Player(int startHealth, int meleeDmg, int rangedDmg) 
    : Entity(startHealth, startHealth), meleeDamage(meleeDmg), 
      rangedDamage(rangedDmg),
      combatMode(CombatMode::MELEE), canMove(true), hand(5) {
    hand.addSpell(getRandomSpell());
}

int Player::getDamage() const {
    return (combatMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

void Player::setMoney(int money) {
    this->money = money;
}

void Player::switchCombatMode() {
    combatMode = (combatMode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}
