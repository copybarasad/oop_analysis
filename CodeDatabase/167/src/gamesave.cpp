#include "gamesave.h"
#include "player.h"
#include "enemy.h"
#include "gamefield.h"
#include "playerhand.h"
#include "spellcard.h"
#include "directdamagespell.h"
#include "aoespell.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir _mkdir
#else
#include <unistd.h>
#endif

GameSave::GameSave() : saveDirectory("saves/")
{
  createSaveDirectory();
}

bool GameSave::createSaveDirectory() const
{
#ifdef _WIN32
  return mkdir(saveDirectory.c_str()) == 0 || GetLastError() == ERROR_ALREADY_EXISTS;
#else
  return mkdir(saveDirectory.c_str(), 0777) == 0 || errno == EEXIST;
#endif
}

std::string GameSave::getSaveFilePath(const std::string &saveName) const
{
  return saveDirectory + saveName + ".sav";
}

std::string GameSave::getCurrentTimestamp() const
{
  auto now = std::time(nullptr);
  auto tm = *std::localtime(&now);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

bool GameSave::saveGame(const std::string &saveName,
                        const Player &player,
                        const GameField &field,
                        int level)
{
  if (saveName.empty())
  {
    std::cerr << "Save name cannot be empty!" << std::endl;
    return false;
  }

  std::string filePath = getSaveFilePath(saveName);

  try
  {
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
      throw std::runtime_error("Cannot open file for writing: " + filePath);
    }

    std::string header = "RPGGAME_SAVE_V2.0";
    file.write(header.c_str(), header.length());
    file.put('\0');

    file.write(reinterpret_cast<const char *>(&level), sizeof(level));

    std::string timestamp = getCurrentTimestamp();
    file.write(timestamp.c_str(), timestamp.length());
    file.put('\0');

    std::string playerName = player.getName();
    file.write(playerName.c_str(), playerName.length());
    file.put('\0');

    int playerHealth = player.getHealth();
    int playerMaxHealth = player.getMaxHealth();
    int playerDamage = player.getDamage();
    int playerScore = player.getScore();
    int playerPosX = player.getPosX();
    int playerPosY = player.getPosY();

    file.write(reinterpret_cast<const char *>(&playerHealth), sizeof(playerHealth));
    file.write(reinterpret_cast<const char *>(&playerMaxHealth), sizeof(playerMaxHealth));
    file.write(reinterpret_cast<const char *>(&playerDamage), sizeof(playerDamage));
    file.write(reinterpret_cast<const char *>(&playerScore), sizeof(playerScore));
    file.write(reinterpret_cast<const char *>(&playerPosX), sizeof(playerPosX));
    file.write(reinterpret_cast<const char *>(&playerPosY), sizeof(playerPosY));

    int width = field.getWidth();
    int height = field.getHeight();
    file.write(reinterpret_cast<const char *>(&width), sizeof(width));
    file.write(reinterpret_cast<const char *>(&height), sizeof(height));

    const auto &enemies = field.getEnemies();
    size_t enemyCount = enemies.size();
    file.write(reinterpret_cast<const char *>(&enemyCount), sizeof(enemyCount));

    for (const auto &enemy : enemies)
    {
      std::string enemyType = enemy->getType();
      file.write(enemyType.c_str(), enemyType.length());
      file.put('\0');

      int enemyHealth = enemy->getHealth();
      int enemyMaxHealth = enemy->getMaxHealth();
      int enemyDamage = enemy->getDamage();
      int enemyPosX = enemy->getPosX();
      int enemyPosY = enemy->getPosY();
      int enemyId = enemy->getId();

      file.write(reinterpret_cast<const char *>(&enemyHealth), sizeof(enemyHealth));
      file.write(reinterpret_cast<const char *>(&enemyMaxHealth), sizeof(enemyMaxHealth));
      file.write(reinterpret_cast<const char *>(&enemyDamage), sizeof(enemyDamage));
      file.write(reinterpret_cast<const char *>(&enemyPosX), sizeof(enemyPosX));
      file.write(reinterpret_cast<const char *>(&enemyPosY), sizeof(enemyPosY));
      file.write(reinterpret_cast<const char *>(&enemyId), sizeof(enemyId));
    }

    file.close();

    if (file.fail())
    {
      throw std::runtime_error("Error writing to file");
    }

    std::cout << "Game saved successfully as: " << saveName << std::endl;
    return true;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error saving game: " << e.what() << std::endl;
    return false;
  }
}

bool GameSave::loadGame(const std::string &saveName,
                        Player &player,
                        GameField &field,
                        int &level)
{
  if (!saveExists(saveName))
  {
    std::cerr << "Save file does not exist: " << saveName << std::endl;
    return false;
  }

  std::string filePath = getSaveFilePath(saveName);

  try
  {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
      throw std::runtime_error("Cannot open file for reading: " + filePath);
    }

    std::string header;
    char ch;
    while (file.get(ch) && ch != '\0')
    {
      header += ch;
    }

    if (header != "RPGGAME_SAVE_V2.0" && header != "RPGGAME_SAVE_V1.0")
    {
      throw std::runtime_error("Invalid save file format: " + header);
    }

    file.read(reinterpret_cast<char *>(&level), sizeof(level));

    std::string timestamp;
    while (file.get(ch) && ch != '\0')
    {
      timestamp += ch;
    }

    std::string playerName;
    while (file.get(ch) && ch != '\0')
    {
      playerName += ch;
    }

    int playerHealth, playerMaxHealth, playerDamage, playerScore, playerPosX, playerPosY;
    file.read(reinterpret_cast<char *>(&playerHealth), sizeof(playerHealth));
    file.read(reinterpret_cast<char *>(&playerMaxHealth), sizeof(playerMaxHealth));
    file.read(reinterpret_cast<char *>(&playerDamage), sizeof(playerDamage));
    file.read(reinterpret_cast<char *>(&playerScore), sizeof(playerScore));
    file.read(reinterpret_cast<char *>(&playerPosX), sizeof(playerPosX));
    file.read(reinterpret_cast<char *>(&playerPosY), sizeof(playerPosY));

    player.setHealth(playerHealth);
    player.setMaxHealth(playerMaxHealth);
    player.setDamage(playerDamage);
    player.setScore(playerScore);
    player.setPosition(playerPosX, playerPosY);

    int width, height;
    file.read(reinterpret_cast<char *>(&width), sizeof(width));
    file.read(reinterpret_cast<char *>(&height), sizeof(height));

    size_t enemyCount;
    file.read(reinterpret_cast<char *>(&enemyCount), sizeof(enemyCount));

    for (size_t i = 0; i < enemyCount; ++i)
    {
      std::string enemyType;
      while (file.get(ch) && ch != '\0')
      {
        enemyType += ch;
      }

      int enemyHealth, enemyMaxHealth, enemyDamage, enemyPosX, enemyPosY, enemyId;
      file.read(reinterpret_cast<char *>(&enemyHealth), sizeof(enemyHealth));
      file.read(reinterpret_cast<char *>(&enemyMaxHealth), sizeof(enemyMaxHealth));
      file.read(reinterpret_cast<char *>(&enemyDamage), sizeof(enemyDamage));
      file.read(reinterpret_cast<char *>(&enemyPosX), sizeof(enemyPosX));
      file.read(reinterpret_cast<char *>(&enemyPosY), sizeof(enemyPosY));
      file.read(reinterpret_cast<char *>(&enemyId), sizeof(enemyId));

      Enemy *enemy = new Enemy(enemyType, enemyMaxHealth, enemyDamage);
      enemy->setHealth(enemyHealth);
      enemy->setId(enemyId);
      enemy->setPosition(enemyPosX, enemyPosY);

      field.placeEnemy(enemy, enemyPosX, enemyPosY);
    }

    file.close();

    std::cout << "Game loaded successfully: " << saveName << std::endl;
    std::cout << "Timestamp: " << timestamp << std::endl;
    std::cout << "Player restored: " << playerName << " (HP: " << playerHealth << ")" << std::endl;
    std::cout << "Enemies loaded: " << enemyCount << std::endl;
    return true;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error loading game: " << e.what() << std::endl;
    return false;
  }
}

bool GameSave::deleteSave(const std::string &saveName)
{
  try
  {
    std::string filePath = getSaveFilePath(saveName);
    return std::remove(filePath.c_str()) == 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error deleting save: " << e.what() << std::endl;
    return false;
  }
}

std::vector<std::string> GameSave::listSaves() const
{
  std::vector<std::string> saves;

  DIR *dir = opendir(saveDirectory.c_str());
  if (!dir)
  {
    return saves;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != nullptr)
  {
    std::string filename = entry->d_name;
    if (filename.length() > 4 &&
        filename.substr(filename.length() - 4) == ".sav")
    {
      saves.push_back(filename.substr(0, filename.length() - 4));
    }
  }

  closedir(dir);
  std::sort(saves.begin(), saves.end());

  return saves;
}

bool GameSave::saveExists(const std::string &saveName) const
{
  std::string filePath = getSaveFilePath(saveName);
  std::ifstream file(filePath);
  return file.good();
}

bool GameSave::validateSaveFile(const std::string &filePath) const
{
  try
  {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
      return false;
    }

    std::string header;
    char ch;
    while (file.get(ch) && ch != '\0')
    {
      header += ch;
    }

    file.close();
    return header == "RPGGAME_SAVE_V2.0" || header == "RPGGAME_SAVE_V1.0";
  }
  catch (...)
  {
    return false;
  }
}

void GameSave::displaySaveInfo(const std::string &saveName) const
{
  std::string filePath = getSaveFilePath(saveName);

  try
  {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
      std::cout << "Cannot open save file: " << saveName << std::endl;
      return;
    }

    char ch;
    while (file.get(ch) && ch != '\0')
    {
    }

    int level;
    file.read(reinterpret_cast<char *>(&level), sizeof(level));

    std::string timestamp;
    while (file.get(ch) && ch != '\0')
    {
      timestamp += ch;
    }

    std::string playerName;
    while (file.get(ch) && ch != '\0')
    {
      playerName += ch;
    }

    int playerHealth;
    file.read(reinterpret_cast<char *>(&playerHealth), sizeof(playerHealth));

    file.close();

    std::cout << "Save: " << saveName << std::endl;
    std::cout << "  Level: " << level << std::endl;
    std::cout << "  Player: " << playerName << " (HP: " << playerHealth << ")" << std::endl;
    std::cout << "  Saved: " << timestamp << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cout << "Error reading save info: " << e.what() << std::endl;
  }
}