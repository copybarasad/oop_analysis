#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>

#include "../direction.hpp"
#include "../field/field.hpp"
#include "../positionable.hpp"
#include "../save/dumpable.hpp"

class Ally;
class Enemy;

class Map : public Dumpable {
  public:
    virtual ~Map() = 0;
    virtual Map *clone() = 0;

    virtual Field *at(Position position) = 0;
    virtual Field *randomNearby(Position position) = 0;
    virtual bool outOfBounds(Position position) = 0;
    virtual Field *fieldInDirection(Position position, Direction direction) = 0;
    virtual std::pair<int, int> directionToVec2D(Direction direction) = 0;
    virtual Position locateField(Field *field) = 0;
    virtual double distance(Position p1, Position p2) = 0;
    virtual bool winConditionIsMet() = 0;
    virtual std::string toString() = 0;
    virtual void update() = 0;

    virtual bool canSpawnAt(Position where) = 0;
    virtual void spawn(Creature *creture, Position where) = 0;
    virtual std::vector<Ally *> extractSpawnedAllies() = 0;
    virtual std::vector<Enemy *> extractSpawnedEnemies() = 0;

  private:
    virtual Field *randomField(Position pos) = 0;
};

#endif /* MAP_HPP */
