#pragma once
#include <string>
#include <fstream>
#include <stdexcept>
#include "type.h"
#include "spell.h"
#include <string>
class Game;
class Field;
class Player;
class Enemy;
class EnemyTower;

class SaveSystem {
private:
    std::string saveFileName = "game_save.txt";
    //сохранить
    void saveField(std::ofstream& file, const Field& field);
    void savePlayer(std::ofstream& file, const Player& player);
    void saveEnemy(std::ofstream& file, const Enemy& enemy);
    void saveTower(std::ofstream& file, const EnemyTower& tower);
    //загрузить
    void loadField(std::ifstream& file, Game& game);
    void loadPlayer(std::ifstream& file, Game& game);
    void loadPlayerSpells(std::ifstream& file, Game& game);
    void loadEnemy(std::ifstream& file, Game& game);
    void loadTower(std::ifstream& file, Game& game);
    Spell* createSpellByName(const std::string& name);
    bool isNumber(const std::string& str);
public:
    void saveGame(const Game& game);
    bool loadGame(Game& game);
    bool saveExists() const;
    void deleteSave();
};