#ifndef GAME_SERIALIZER_H
#define GAME_SERIALIZER_H

#include <string>
#include <vector>

class Board;
class Player;
class Enemy;
class Ally;
class EnemyBase;
class EnemyTower;
class LevelManager;
enum class Difficulty;

class GameSerializer {
public:
    static void Save( Board& board, Player& player, std::vector<Enemy>& enemies, std::vector<Ally>& allies,
                     EnemyBase* base, std::vector<EnemyTower>& towers, LevelManager& levelManager,
                     Difficulty difficulty,
                     int hpBonus, int meleeBonus,
                     int directSpellBonus, const std::string& filename);
 
    static void Load(Board& board,
                     Player& player,
                     std::vector<Enemy>& enemies,
                     std::vector<Ally>& allies,
                     EnemyBase*& base,
                     std::vector<EnemyTower>& towers,
                     LevelManager& levelManager,
                     Difficulty& difficulty,
                     int& hpBonus,
                     int& meleeBonus,
                     int& directSpellBonus,
                     const std::string& filename);
};

#endif