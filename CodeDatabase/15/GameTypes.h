#ifndef GAME_TYPES_H
#define GAME_TYPES_H

enum class GameState { kNormal, kCasting, kLevelTransition, kGameOver, kWin, kLevelUp };

enum class SpellType { kDirect, kArea, kTrap, kSummon, kEnhance };

enum class EntityType { kPlayer, kEnemy, kAlly, kBuilding, kTower, kTrap };

#endif