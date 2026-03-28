#ifndef GAMEPHASE_H
#define GAMEPHASE_H

enum class GamePhase {
    MainMenu,
    PlayerTurn,
    AlliesTurn, 
    EnemiesTurn,
    BuildingsTurn,
    LevelTransition,
    UpgradePhase,
    GameOver,
    Victory,
    SaveGame,
    LoadGame
};

#endif