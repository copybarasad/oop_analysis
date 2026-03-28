#pragma once
#include "field.h"
#include "player.h"
#include "spell.h"
class DirectDamage : public Spell {
public:
  DirectDamage(unsigned int damage = 20) {
    this->damage = damage;
    this->type = DIRECT;
  }
  bool doDamage(Player &p) {
    if (!p.isAlive())
      return false;
    p.takeDamage(this->damage);
    return true;
  }

  bool doDamage(Player &p, std::vector<Enemy> &enemies, Field &f) {
    int px = p.getX();
    int py = p.getY();
    bool didDamage = false;

    std::vector<Enemy *> EnemiesDMG;

    // 1️⃣ Collect all enemies adjacent to player
    for (auto &enemy : enemies) {
      int ex = enemy.getX();
      int ey = enemy.getY();

      if (std::abs(ex - px) <= 1 && std::abs(ey - py) <= 1) {
        EnemiesDMG.push_back(&enemy);
      }
    }

    if (EnemiesDMG.empty())
      return false;
    for (auto *enemy : EnemiesDMG) {
      enemy->takeDamage(damage);
      didDamage = true;
      break;
    }

    f.updateField(p, enemies);

    return didDamage;
  }
};
