#include <algorithm>
#include <stdexcept>

#include "combattable.hpp"

Combattable::Combattable(int hp, int maxHP) {
    m_HP = hp;
    m_MaxHP = maxHP;
    m_Weapon = Weapon{
        WeaponType::Melee,
        2,
    };
}

Combattable::Combattable(const Combattable &combattable) {
    m_HP = combattable.m_HP;
    m_MaxHP = combattable.m_MaxHP;
    m_Weapon = combattable.m_Weapon;
}

void Combattable::takeDamage(int damage) {
    if (damage < 0) {
        return;
    }

    m_HP -= std::min(damage, m_HP);
}

void Combattable::dealDamage(Combattable *damageTaker) {
    damageTaker->takeDamage(m_Weapon.damage);
}

void Combattable::setWeapon(Weapon weapon) { m_Weapon = weapon; }

int Combattable::getHP() { return m_HP; }
int Combattable::getMaxHP() { return m_MaxHP; }
void Combattable::healFullHP() { m_HP = m_MaxHP; }
bool Combattable::isAlive() { return m_HP != 0; }
int Combattable::damage() { return m_Weapon.damage; }

void Combattable::increaseMaxHP(int amount) {
    if (amount <= 0) {
        throw std::logic_error("amount must be positive");
    }
    m_MaxHP += amount;
}
