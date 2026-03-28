#include "SaveSystem.h"
#include <iostream>
#include <sstream>

void SaveSystem::saveGame(const Game &game, const std::string &filename) {
  std::string fullFilename = filename + ".save";

  try {
    FileRAII file(fullFilename, std::ios::out | std::ios::binary);
    file.writeLine("SAVE_VERSION 1.0");
    writeField(file, game.getField());
    writePlayer(file, game.getPlayer());
    writeHand(file, game.getPlayer().getHand());
    writeEnemies(file, game.getEnemyManager());
    writeBuildings(file, game.getBuildingManager());
    file.writeLine("END_SAVE");
  } catch (const std::exception &e) {
    cleanupFailedSave(fullFilename);
    throw std::runtime_error(std::string("Save failed: ") + e.what());
  }
}

void SaveSystem::loadGame(Game &game, const std::string &filename) {
  try {
    FileRAII file(filename, std::ios::in);
    std::string versionLine = file.readLine();
    if (versionLine != "SAVE_VERSION 1.0") {
      throw std::runtime_error("Invalid or corrupted save file format");
    }

    std::string fieldLine = file.readLine();
    std::istringstream dimStream(fieldLine);
    std::string token;
    int width, height;

    if (!(dimStream >> token >> width >> height) || token != "FIELD") {
      throw std::runtime_error("Invalid field data in save file");
    }

    game.getEnemyManager().clear();
    game.getBuildingManager().clear();

    Hand emptyHand(5);
    game.getPlayer().getHand() = std::move(emptyHand);
    game.getPlayer().setPosition(Position(0, 0));
    game.getPlayer().setSlowed(false);
    game.setGameRunning(true);

    readField(file, game.getField(), width, height);
    readPlayer(file, game.getPlayer(), game.getPlayer().getHand());
    readEnemies(file, game.getEnemyManager());
    readBuildings(file, game.getBuildingManager(), &game.getEnemyManager());

    game.getCombatSystem() = std::make_unique<CombatSystem>(
        game.getPlayer(), game.getEnemyManager(), game.getBuildingManager(),
        game.getField());

    game.getRenderer() = std::make_unique<Renderer>(
        game.getField(), game.getPlayer(), game.getEnemyManager(),
        game.getBuildingManager());
    if (game.getInputHandler()) {
      game.getRenderer()->setTargetingStatePtr(
          &game.getInputHandler()->getTargetingState());
      game.getRenderer()->setMessagePtr(
          &game.getInputHandler()->getStatusMessage());
    }

    std::string endMarker = file.readLine();
    if (endMarker != "END_SAVE") {
      throw std::runtime_error("Save file is corrupted (missing END marker)");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("Load failed: ") + e.what());
  }
}

void SaveSystem::cleanupFailedSave(const std::string &filename) {
  try {
    if (fs::exists(filename)) {
      fs::remove(filename);
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Warning: Could not cleanup failed save: " << e.what()
              << std::endl;
  }
}

void SaveSystem::writeField(FileRAII &file, const Field &field) {
  std::ostringstream oss;
  oss << "FIELD " << field.getWidth() << " " << field.getHeight();
  file.writeLine(oss.str());

  for (int y = 0; y < field.getHeight(); ++y) {
    std::string line;
    for (int x = 0; x < field.getWidth(); ++x) {
      Position pos(x, y);
      CellType type = field.getCell(pos).getType();
      line += std::to_string(static_cast<int>(type)) + " ";
    }
    file.writeLine(line);
  }
}

void SaveSystem::writePlayer(FileRAII &file, const Player &player) {
  std::ostringstream oss;
  oss << "PLAYER " << player.getPosition().getX() << " "
      << player.getPosition().getY() << " " << player.getHealth() << " "
      << (player.getCombatMode() == CombatMode::melee ? 0 : 1) << " "
      << (player.isSlowed() ? 1 : 0) << " " << player.getCurrentDamage();
  file.writeLine(oss.str());
}

void SaveSystem::writeHand(FileRAII &file, const Hand &hand) {
  std::ostringstream oss;
  oss << "HAND " << hand.size();
  file.writeLine(oss.str());

  for (const auto &spell : hand.getAll()) {
    if (spell) {
      if (spell->name() == "Direct Damage") {
        oss.str("");
        oss << "DIRECT " << spell->radius();
        file.writeLine(oss.str());
      } else if (spell->name() == "Square Damage") {
        oss.str("");
        oss << "SQUARE " << spell->radius();
        file.writeLine(oss.str());
      }
    }
  }
}
void SaveSystem::writeEnemies(FileRAII &file,
                              const EntityManager<Enemy> &enemyManager) {
  std::ostringstream oss;
  oss << "ENEMIES " << enemyManager.size();
  file.writeLine(oss.str());

  for (const auto &enemy : enemyManager.getAll()) {
    if (enemy) {
      oss.str("");
      oss << enemy->getPosition().getX() << " " << enemy->getPosition().getY()
          << " " << enemy->getHealth() << " " << enemy->getDamage() << " "
          << (enemy->isSlowed() ? 1 : 0) << " "
          << (enemy->wasAdjacentToPlayer() ? 1 : 0);
      file.writeLine(oss.str());
    }
  }
}

void SaveSystem::writeBuildings(
    FileRAII &file, const EntityManager<EnemyBuilding> &buildingManager) {
  std::ostringstream oss;
  oss << "BUILDINGS " << buildingManager.size();
  file.writeLine(oss.str());

  for (const auto &building : buildingManager.getAll()) {
    if (building && building->isAlive()) {
      oss.str("");
      oss << building->getPosition().getX() << " "
          << building->getPosition().getY() << " " << building->getHealth()
          << " " << building->getSpawnCooldown() << " "
          << building->getMaxCooldown();
      file.writeLine(oss.str());
    }
  }
}

void SaveSystem::readField(FileRAII &file, Field &field, int width,
                           int height) {
  Field loadedField(width, height);

  for (int y = 0; y < height; ++y) {
    try {
      std::string line = file.readLine();
      std::istringstream cellStream(line);

      for (int x = 0; x < width; ++x) {
        int typeInt;
        if (!(cellStream >> typeInt)) {
          throw std::runtime_error("Error reading cell type at position (" +
                                   std::to_string(x) + "," + std::to_string(y) +
                                   ")");
        }

        if (typeInt < 0 || typeInt > 2) {
          throw std::runtime_error("Invalid cell type value: " +
                                   std::to_string(typeInt));
        }

        CellType type = static_cast<CellType>(typeInt);
        loadedField.getCell(Position(x, y)).setType(type);
      }
    } catch (const std::exception &e) {
      throw std::runtime_error("Error reading cell data row " +
                               std::to_string(y) + ": " + e.what());
    }
  }

  field = std::move(loadedField);
}

void SaveSystem::readPlayer(FileRAII &file, Player &player, Hand &hand) {
  std::string line = file.readLine();
  std::istringstream iss(line);
  std::string token;

  if (!(iss >> token) || token != "PLAYER") {
    throw std::runtime_error("Expected player data, got: " + token);
  }

  int x, y, health, combatMode, slowed, damage;
  if (!(iss >> x >> y >> health >> combatMode >> slowed >> damage)) {
    throw std::runtime_error("Invalid player data format");
  }
  if (health < 0)
    throw std::runtime_error("Invalid player health: " +
                             std::to_string(health));
  if (combatMode < 0 || combatMode > 1)
    throw std::runtime_error("Invalid combat mode");
  if (slowed < 0 || slowed > 1)
    throw std::runtime_error("Invalid slowed flag");
  player.setPosition(Position(x, y));
  int currentHealth = player.getHealth();
  if (currentHealth > health) {
    player.takeDamage(currentHealth - health);
  }

  CombatMode targetMode =
      (combatMode == 0) ? CombatMode::melee : CombatMode::ranged;
  if (player.getCombatMode() != targetMode) {
    player.switchCombatMode();
  }

  player.setSlowed(slowed == 1);
  line = file.readLine();
  iss.clear();
  iss.str(line);

  if (!(iss >> token) || token != "HAND") {
    throw std::runtime_error("Expected hand data, got: " + token);
  }

  int handSize;
  if (!(iss >> handSize)) {
    throw std::runtime_error("Invalid hand size");
  }

  if (handSize < 0 || (size_t)handSize > hand.maxSize()) {
    throw std::runtime_error("Invalid hand size: " + std::to_string(handSize));
  }
  hand = Hand(5);

  for (int i = 0; i < handSize; ++i) {
    line = file.readLine();
    iss.clear();
    iss.str(line);

    std::string spellType;
    int radius;
    if (!(iss >> spellType >> radius)) {
      throw std::runtime_error("Invalid spell data format");
    }

    if (radius <= 0) {
      throw std::runtime_error("Invalid spell radius: " +
                               std::to_string(radius));
    }

    if (spellType == "DIRECT") {
      hand.add(std::make_unique<SpellDirectDamage>(radius, 20));
    } else if (spellType == "SQUARE") {
      hand.add(std::make_unique<SpellSquareDamage>(radius, 12));
    } else {
      throw std::runtime_error("Unknown spell type: " + spellType);
    }
  }
}

void SaveSystem::readEnemies(FileRAII &file,
                             EntityManager<Enemy> &enemyManager) {
  std::string line = file.readLine();
  std::istringstream iss(line);
  std::string token;

  if (!(iss >> token) || token != "ENEMIES") {
    throw std::runtime_error("Expected enemy data, got: " + token);
  }

  int enemyCount;
  if (!(iss >> enemyCount)) {
    throw std::runtime_error("Invalid enemy count");
  }

  if (enemyCount < 0) {
    throw std::runtime_error("Invalid enemy count: " +
                             std::to_string(enemyCount));
  }

  enemyManager.clear();

  for (int i = 0; i < enemyCount; ++i) {
    line = file.readLine();
    iss.clear();
    iss.str(line);

    int x, y, health, damage, slowed, adjacent;
    if (!(iss >> x >> y >> health >> damage >> slowed >> adjacent)) {
      throw std::runtime_error("Invalid enemy data format at entry " +
                               std::to_string(i));
    }

    if (health < 0)
      throw std::runtime_error("Invalid enemy health");
    if (damage < 0)
      throw std::runtime_error("Invalid enemy damage");
    if (slowed < 0 || slowed > 1)
      throw std::runtime_error("Invalid slowed flag");
    if (adjacent < 0 || adjacent > 1)
      throw std::runtime_error("Invalid adjacent flag");

    Position exactPos(x, y);
    auto enemy = std::make_unique<Enemy>(damage, health, exactPos);

    int currentHealth = enemy->getHealth();
    if (currentHealth > health) {
      enemy->takeDamage(currentHealth - health);
    }

    if (slowed == 1) {
      enemy->setSlowed(true);
    }

    if (adjacent == 1) {
      enemy->setAdjacentToPlayer(true);
    }

    enemyManager.add(std::move(enemy));
  }
}

void SaveSystem::readBuildings(FileRAII &file,
                               EntityManager<EnemyBuilding> &buildingManager,
                               EntityManager<Enemy> *enemyManager) {
  std::string line = file.readLine();
  std::istringstream iss(line);
  std::string token;

  if (!(iss >> token) || token != "BUILDINGS") {
    throw std::runtime_error("Expected building data, got: " + token);
  }

  int buildingCount;
  if (!(iss >> buildingCount)) {
    throw std::runtime_error("Invalid building count");
  }

  if (buildingCount < 0) {
    throw std::runtime_error("Invalid building count: " +
                             std::to_string(buildingCount));
  }

  buildingManager.clear();

  for (int i = 0; i < buildingCount; ++i) {
    line = file.readLine();
    iss.clear();
    iss.str(line);

    int x, y, health, cooldown, maxCooldown;
    if (!(iss >> x >> y >> health >> cooldown >> maxCooldown)) {
      throw std::runtime_error("Invalid building data format at entry " +
                               std::to_string(i));
    }

    if (health < 0)
      throw std::runtime_error("Invalid building health");
    if (cooldown < 0)
      throw std::runtime_error("Invalid cooldown");
    if (maxCooldown <= 0)
      throw std::runtime_error("Invalid max cooldown");

    auto building = std::make_unique<EnemyBuilding>(health, Position(x, y),
                                                    maxCooldown, enemyManager);
    int currentHealth = building->getHealth();
    if (currentHealth > health) {
      building->takeDamage(currentHealth - health);
    }

    building->setSpawnCooldown(cooldown);
    buildingManager.add(std::move(building));
  }
}

bool SaveSystem::saveFileExists(const std::string &filename) {
  std::string fullFilename = filename;
  if (filename.size() < 6 || filename.substr(filename.size() - 5) != ".save") {
    fullFilename = filename + ".save";
  }
  try {
    FileRAII file(fullFilename, std::ios::in);
    return file.isOpen() && file;
  } catch (const std::exception &) {
    return false;
  }
}

std::vector<std::string> SaveSystem::getSaveFiles() {
  std::vector<std::string> saveFiles;
  try {
    for (const auto &entry : fs::directory_iterator(".")) {
      if (entry.is_regular_file()) {
        std::string filename = entry.path().filename().string();
        if (filename.size() > 5 &&
            filename.substr(filename.size() - 5) == ".save") {
          try {
            if (isValidSaveFile(filename)) {
              saveFiles.push_back(filename);
            }
          } catch (const std::exception &) {
            continue;
          }
        }
      }
    }
    std::sort(saveFiles.begin(), saveFiles.end(),
              [](const std::string &a, const std::string &b) {
                try {
                  auto timeA = fs::last_write_time(a);
                  auto timeB = fs::last_write_time(b);
                  return timeA > timeB;
                } catch (...) {
                  return a < b;
                }
              });

  } catch (const fs::filesystem_error &e) {
    std::cerr << "Warning: Could not read directory: " << e.what() << std::endl;
  }

  return saveFiles;
}

bool SaveSystem::isValidSaveFile(const std::string &filename) {
  std::string fullFilename = filename;
  if (filename.size() < 6 || filename.substr(filename.size() - 5) != ".save") {
    fullFilename = filename + ".save";
  }

  try {
    FileRAII file(fullFilename, std::ios::in);
    std::string versionLine = file.readLine();
    if (versionLine != "SAVE_VERSION 1.0") {
      return false;
    }
    std::string fieldLine = file.readLine();
    std::istringstream iss(fieldLine);
    std::string token;
    int width, height;

    if (!(iss >> token >> width >> height) || token != "FIELD") {
      return false;
    }
    if (width < 10 || width > 25 || height < 10 || height > 25) {
      return false;
    }
    return true;
  } catch (const std::exception &) {
    return false;
  }
}