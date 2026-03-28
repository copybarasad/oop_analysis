//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_GAMEMASTER_H
#define LABA1_GAMEMASTER_H


#include <string>
#include "../GameField/Field.h"
#include "../Entities/Creatures/Player.h"
#include "Controllers/AllyController.h"
#include "Controllers/EnemyController.h"
#include "Controllers/Building/EnemySpawnerBuildingController.h"
#include "Controllers/Building/EnemyDefenceTowerController.h"
#include "Controllers/PlayerController.h"
#include "../Model/EnemyDefenceTowerModel.h"
#include "../Model/EnemySpawnerBuildingModel.h"
#include "../GameRender/ConsoleRenderer.h"
#include "Controllers/ExternalComandController/GamerConsoleInputSpotter.h"
#include "GameMasterBase.h"
#include "../GameRender/GameVisualizer.h"

template<typename InputHandler, typename DrawStrategy>
class GameMaster : public GameMasterBase{
public:
    GameMaster() {};

    bool startGame(const std::string &json, const std::string& keysJson);

    void gameCycle();

    void redraw();

    void addEnemyController(EnemyController *controller, std::shared_ptr<Entity> entity);

    void addAllyController(AllyController *controller, std::shared_ptr<Entity> entity);

    void saveGame();
private:
    int level;
    Field *field;
    std::shared_ptr<Player> player;
    bool isSaved = false;

    std::vector<std::shared_ptr<EnemyController>> enemyControllers;
    std::vector<std::shared_ptr<AllyController>> allyControllers;
    std::vector<std::shared_ptr<EnemySpawnerBuildingController>> enemySpawnerBuildingControllers;
    std::vector<std::shared_ptr<EnemyDefenceTowerController>> defenceTowerControllers;
    std::shared_ptr<PlayerController> playerController;
    std::unique_ptr<InputHandler> inputHandler;

    std::vector<std::shared_ptr<Entity>> entities;

    std::unique_ptr<EnemySpawner> enemySpawner;
    std::unique_ptr<AllySpawner> allySpawner;
    std::unique_ptr<SpellFactory> spellFactory;
    std::shared_ptr<PlayerView> playerView;
    GameVisualizer<DrawStrategy>* visualizer;

    const CompControlledCreatureModel enemyModel{3, 2, 2, 70};
    const CompControlledCreatureModel allyModel{3, 2, 2, 70};
//    const EnemyDefenceTowerModel defenceTowerModel{3,3,3};
//    const EnemySpawnerBuildingModel spawnerBuildingModel{2,2};

    void checkEntitiesAfterMove();
};

#include "GameMaster.tpp"

#endif //LABA1_GAMEMASTER_H
