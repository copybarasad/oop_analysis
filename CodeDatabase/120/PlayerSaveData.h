#pragma once

#include "Hand.h"
enum class SavedCombatMode { Melee, Ranged };

struct PlayerSaveData {
  int hp;
  int max_hp;
  int damage;
  int score;
  SavedCombatMode mode;
  int pendingBuffs;
};
