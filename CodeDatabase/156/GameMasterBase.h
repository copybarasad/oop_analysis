//
// Created by Artem on 26.12.2025.
//

#ifndef LABAOOP2_GAMEMASTERBASE_H
#define LABAOOP2_GAMEMASTERBASE_H

#include <memory>
#include <vector>
#include <string>

class Field;
class Entity;
class EnemyController;
class AllyController;
class EnemySpawnerBuildingController;
class EnemyDefenceTowerController;
class Player;
class PlayerView;

class GameMasterBase {
public:
    virtual ~GameMasterBase() = default;

    virtual void redraw() = 0;

    virtual void saveGame() = 0;

    virtual void addEnemyController(EnemyController* controller,
                                    std::shared_ptr<Entity> entity) = 0;

    virtual void addAllyController(AllyController* controller,
                                   std::shared_ptr<Entity> entity) = 0;
};


#endif //LABAOOP2_GAMEMASTERBASE_H
