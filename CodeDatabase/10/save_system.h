#pragma once

#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include "type.h"
#include "spell.h"

class Game;
class Field;
class Player;
class Enemy;
class Tower;
class DamageTower;
class DirectDamageSpell;
class AreaDamageSpell;
class TrapSpell;

class SaveSystem {
private:
    std::string saveFileName = "game_save.txt";
    
    void saveField(std::ofstream& file, const Field& field);
    void savePlayer(std::ofstream& file, const Player& player);
    void saveEnemies(std::ofstream& file, const std::vector<Enemy*>& enemies);
    void saveTowers(std::ofstream& file, const Tower* tower, const DamageTower* damageTower);
    
    void loadField(std::ifstream& file, Game& game);
    void loadPlayer(std::ifstream& file, Game& game);
    void loadEnemies(std::ifstream& file, Game& game);
    void loadTowers(std::ifstream& file, Game& game);
    
    Spell* createSpellByName(const std::string& name, int power = 0);

public:

    void saveGame(const Game& game);
    bool loadGame(Game& game);
};
