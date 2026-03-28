#pragma once

#include "GameObjects/Entities/Player.hpp"
#include "GameClass/EnemyController.hpp"
#include "GameClass/MapController.hpp"
#include "GameClass/TowerController.hpp"
#include "GameClass/CastController.hpp"
#include "InputController/commands.hpp"


class GameClass {
public:
    GameClass();
    void loadLevel(Motion motion);
    GameDTO toDTO();

    std::vector<Motion> getAllowedMotions();
    void gameLoop(Motion motion);
    GameState getState();
    

private:
    GameState state;
    
    StatisticDTO stats;
    bool bonus;

    std::unique_ptr<Player> player;
    std::unique_ptr<MapController> MapC;
    std::unique_ptr<EnemyController> EnemiesC;
    std::unique_ptr<TowerController> TowerC;
    std::unique_ptr<CastController> CastC;

    std::vector<Coordinates> shootedFields;


    void handleMotion(Motion motion);
    void AIsteps();

    void playerStep(Motion Motion);
    void handleRangeStep(Coordinates delta);
    void handleMeleeStep(Coordinates delta);
    void movePlayer(Coordinates oldPos, Coordinates newPos);
    void handlePlayerMotion(Motion Motion);

    void saveGame();

    void improvePlayer(Motion motion);
};
