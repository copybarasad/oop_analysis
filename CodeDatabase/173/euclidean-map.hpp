#ifndef EUCLIDEAN_MAP_HPP
#define EUCLIDEAN_MAP_HPP

#include <cstddef>
#include <vector>

#include "../ally.hpp"
#include "../enemy.hpp"
#include "map.hpp"

class EuclideanMap : public Map {
  public:
    EuclideanMap(int width, int height);
    EuclideanMap(const EuclideanMap &);
    EuclideanMap(EuclideanMap &&map);
    ~EuclideanMap();
    EuclideanMap &operator=(const EuclideanMap &other);
    EuclideanMap &operator=(EuclideanMap &&other);
    Map *clone() override;

    Field *at(Position position) override;
    Field *randomNearby(Position position) override;
    bool outOfBounds(Position position) override;
    Field *fieldInDirection(Position position, Direction direction) override;
    std::pair<int, int> directionToVec2D(Direction direction) override;
    Position locateField(Field *field) override;
    double distance(Position p1, Position p2) override;
    bool winConditionIsMet() override;
    std::string toString() override;
    void update() override;

    bool canSpawnAt(Position where) override;
    void spawn(Creature *creture, Position where) override;
    std::vector<Ally *> extractSpawnedAllies() override;
    std::vector<Enemy *> extractSpawnedEnemies() override;

    bytedump dump() override;
    EuclideanMap(bytedump bytes);

  private:
    int m_Width;
    int m_Height;
    std::vector<std::vector<Field *>> m_Fields;
    std::vector<Ally *> m_SpawnedAllies;
    std::vector<Enemy *> m_SpawnedEnemies;

    Field *randomField(Position pos) override;
    bool trySpawnAlly(Creature *creture, Position where);
    bool trySpawnEnemy(Creature *creture, Position where);
};

#endif /* EUCLIDEAN_MAP_HPP */
