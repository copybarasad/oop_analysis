//
// Created by bob on 10/5/25.
//

#ifndef GAME_TERM_FIELD_H
#define GAME_TERM_FIELD_H

#include <list>

#include "Cell.h"
#include "Enemy.h"
#include "EnemyBuild.h"
#include "EnemyTower.h"
#include "Player.h"
#include "Trap.h"
#include "../Direction.h"

#define MIN_SIZE 10
#define MAX_SIZE 25

class Cell;

class Field: public ISerializable {

    int steps;
    Cell ** map;
    std::shared_ptr<Player> player;
    std::list<Enemy> enemies;
    std::list<EnemyBuild> enemy_builds;
    std::list<Trap> traps;
    std::list<EnemyTower> towers;
    int height;
    int width;

    void generateObjects(const double & obstacleChance, const double & slowChance) const;

    void generatePlayerPosition() const;

    void generatePositionEnemiesBuild();

    void generateEnemyTower();

public:

    Field(const int &height , const int &width, std::shared_ptr<Player> player, std::list<EnemyBuild> enemy_builds,
          std::list<EnemyTower> enemy_towers);

    Field();

    Field(const int & level, std::shared_ptr<Player> player);

    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other);
    Field& operator=(Field&& other);

    ~Field();

    bool isFree(const Position &position) const;
    bool isSlow(const Position &position) const;
    bool inField(const Position &position) const;
    bool isObstacle(const Position &position) const;
    bool isEnemy(const Position &position, const bool &isTeammate);
    bool isPlayer(const Position &position, const int &damage);
    bool isEnemyBuild(const Position &position, const bool &isTeammate);
    bool isTower(const Position &position, bool isTeammate);

    Position generatePositionEnemy(const Position &position);

    void checkTrap(const Position &position, std::list<Enemy>::iterator &enemy);
    void addEnemy(const Enemy &enemy);
    void addTrap(const Trap &trap);

    void incrementSteps();
    int getSteps() const;

    int getWidth() const;
    int getHeight() const;

    std::shared_ptr<Player> getPlayer() const;

    const std::list<Enemy>& getEnemies() const;
    std::list<Enemy>& getEnemiesMutable();

    const std::list<EnemyBuild>& getEnemiesBuild() const;
    std::list<EnemyBuild>& getEnemiesBuildMutable();

    const std::list<Trap>& getTraps() const;
    std::list<Trap>& getTrapsMutable();

    const std::list<EnemyTower>& getEnemyTowers() const;
    std::list<EnemyTower>& getEnemyTowersMutable();

    const Cell getCell(const Position &position) const;

    void setPlayer(std::shared_ptr<Player> player);

    void save(Saver &saver) const override;

    void load(Reader &reader) override;


};


#endif //GAME_TERM_FIELD_H