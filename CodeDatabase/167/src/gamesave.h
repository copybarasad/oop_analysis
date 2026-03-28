#ifndef GAMESAVE_H
#define GAMESAVE_H

#include <string>
#include <memory>
#include <vector>
#include <fstream>

class Player;
class Enemy;
class GameField;
class SpellCard;

struct SaveData
{
  struct PlayerData
  {
    std::string name;
    int health;
    int maxHealth;
    int damage;
    int score;
    int posX;
    int posY;
  };

  struct EnemyData
  {
    std::string type;
    int health;
    int maxHealth;
    int damage;
    int posX;
    int posY;
    int id;
  };

  struct GameFieldData
  {
    int width;
    int height;
  };

  PlayerData player;
  std::vector<EnemyData> enemies;
  GameFieldData field;
  std::string timestamp;
  int level;
};

class GameSave
{
private:
  std::string saveDirectory;

  bool createSaveDirectory() const;
  std::string getSaveFilePath(const std::string &saveName) const;
  std::string getCurrentTimestamp() const;

public:
  GameSave();

  bool saveGame(const std::string &saveName,
                const Player &player,
                const GameField &field,
                int level);

  bool loadGame(const std::string &saveName,
                Player &player,
                GameField &field,
                int &level);

  bool deleteSave(const std::string &saveName);
  std::vector<std::string> listSaves() const;
  bool saveExists(const std::string &saveName) const;

  void displaySaveInfo(const std::string &saveName) const;

private:
  bool validateSaveFile(const std::string &filePath) const;
};

#endif