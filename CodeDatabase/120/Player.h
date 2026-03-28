#pragma once
#include "PlayerSaveData.h"
#include "Entity.h"

class Player : public Entity {
 public:
  enum class CombatMode { Melee, Ranged };

 private:
  int damage;
  int score;
  bool slowed;
  CombatMode mode;
  Hand hand;
  int pendingBuffs = 0;
  int maxHealth = 100;

 public:
  Player(int hp, int dmg, Position pos);
  void RestoreFullHealth();
  void SwitchCombatMode();
  void ResetStatus();
  int GetDamage() const;
  int GetScore() const;
  void AddScore(int points);
  CombatMode GetMode() const;
  bool IsSlowed() const;
  Hand& GetHand();
  void SetHand(Hand&& h);
  void AddPendingBuff();
  int ConsumePendingBuffs();
  void PrintHand();
  void SetSlowed();
  void LoadFromSave(const PlayerSaveData& data);
  void IncreaseMaxHealth(int amount) { maxHealth += amount; }
  void IncreaseDamage(int amount) { damage += amount; }
  void IncreaseHandSize(int amount) { hand.IncreaseCapacity(amount); }
  PlayerSaveData Save() const;
};
