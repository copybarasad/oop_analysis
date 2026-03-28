#ifndef RPG_SAVE_SYSTEM_H_
#define RPG_SAVE_SYSTEM_H_

#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Field.h"
#include "FileRAII.h"
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "Random.h"
#include "SpellDirectDamage.h"
#include "SpellSquareDamage.h"
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class SaveSystem {
public:
  static void saveGame(const Game &game, const std::string &filename);
  static void loadGame(Game &game, const std::string &filename);
  static bool saveFileExists(const std::string &filename);
  static std::vector<std::string> getSaveFiles();
  static bool isValidSaveFile(const std::string &filename);

private:
  static void writeField(FileRAII &file, const Field &field);
  static void writePlayer(FileRAII &file, const Player &player);
  static void writeHand(FileRAII &file, const Hand &hand);
  static void writeEnemies(FileRAII &file,
                           const EntityManager<Enemy> &enemyManager);
  static void
  writeBuildings(FileRAII &file,
                 const EntityManager<EnemyBuilding> &buildingManager);
  static void readField(FileRAII &file, Field &field, int width, int height);
  static void readPlayer(FileRAII &file, Player &player, Hand &hand);
  static void readEnemies(FileRAII &file, EntityManager<Enemy> &enemyManager);
  static void readBuildings(FileRAII &file,
                            EntityManager<EnemyBuilding> &buildingManager,
                            EntityManager<Enemy> *enemyManager = nullptr);
  static void cleanupFailedSave(const std::string &filename);
};

#endif // RPG_SAVE_SYSTEM_H_