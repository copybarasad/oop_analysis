#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <vector>
#include <memory>

class GameGrid;
class Player;
class Enemy;
class EnemyTower;
enum class Direction;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool cast(Player& player, Direction direction,
                      std::vector<std::unique_ptr<Enemy>>& enemies,
                      std::vector<std::unique_ptr<EnemyTower>>& towers,
                      GameGrid& grid) = 0;
    virtual std::string getName() const = 0;
    virtual int getCost() const { return 0; }
};

#endif