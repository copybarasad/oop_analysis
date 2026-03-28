#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <set>

#include "creature.hpp"
#include "spell/hand.hpp"
#include "weapon/weapon.hpp"

class Hand;

class Player : public Creature {
  public:
    Player(int hp, int maxHP, int spellHandCap);
    Player(const Player &player);
    void rangedAttack(Position pos);
    void setSecondaryWeapon(Weapon weapon);
    void swapWeapon();
    bool canCastSpell(int index, Field *target);
    void castSpell(int index, Field *target);
    Hand *spellHand();

    std::string chr() override;
    Color baseColor() override;

    bytedump dump() override;
    Player(bytedump bytes);

    std::string toString();

  private:
    Weapon m_SecondaryWeapon;
    int m_Score;
    std::unique_ptr<Hand> m_Hand;
};

#endif /* PLAYER_HPP */
