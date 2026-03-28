#ifndef COMBATTABLE_HPP
#define COMBATTABLE_HPP

#include "weapon/weapon.hpp"

class Combattable {
  public:
    Combattable(int hp, int maxHP);
    Combattable(const Combattable &combattable);
    void dealDamage(Combattable *damageTaker);
    void takeDamage(int damage);
    int getHP();
    int getMaxHP();
    void healFullHP();
    void increaseMaxHP(int amount);
    void setWeapon(Weapon weapon);
    int damage();
    bool isAlive();

  protected:
    int m_HP;
    int m_MaxHP;
    Weapon m_Weapon;
};

#endif /* COMBATTABLE_HPP */
