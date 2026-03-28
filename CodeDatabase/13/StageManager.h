#ifndef GAME_STAGEMANAGER_H
#define GAME_STAGEMANAGER_H
#include <random>
#include "GameLogic.h"
#include "ObjectFactory.h"
#include  "PlayerUpgrade.h"
#include "GameVisualizer.h"
#include "GameController.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/InputHandler.h"

class StageManager {
private:
    PlayerUpgrade upgrade_;
    std::vector<std::string> upgrades_ =
        {"More health",
        "Refresh mana",
        "Add two random spells",
        "Add two levels to next spell"
    };
    ObjectFactory factory;
    int difficulty_ = 1;
    bool show_info_ = false;
    GameLogic *gl_ = nullptr;
    GameVisualizer<Renderer> *visualizer_ = nullptr;
    GameController<InputHandler> *controller_ = nullptr;

public:
    StageManager();

    ~StageManager();

    void save() const;

    int load();

    void startGame();

    void endGame() const;

    void nextLevel();

    void generateLevel();

    void mainMenu();

    void placeUnits(const std::string &objName, float edge, int layer = 1);
};


#endif
