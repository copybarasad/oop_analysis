#ifndef RPG_PLAYER_H_
#define RPG_PLAYER_H_

#include "Entity.h"
#include "Hand.h"
#include "Position.h"
#include <memory>
#include <string>
#include <vector>

enum class CombatMode { melee, ranged };

class Player : public Entity {
private:
  CombatMode combatMode;
  Hand hand;
  int meleeDamage;
  int rangedDamage;
  bool slowed;

public:
  Player(int meleeDamage, int rangedDamage, int health,
         Position position = Position(0, 0));
  CombatMode getCombatMode() const;
  void switchCombatMode();
  int getCurrentDamage() const;
  std::string getCombatModeString() const;
  void castSpell();
  void attack(Entity &target) const;
  template <typename T>
  void attackInRange(const std::vector<std::unique_ptr<T>> &entities,
                     int range) const;
  bool isSlowed() const;
  void setSlowed(bool value);
  Hand &getHand() { return hand; }
  const Hand &getHand() const { return hand; }
};

#endif // RPG_PLAYER_H_
