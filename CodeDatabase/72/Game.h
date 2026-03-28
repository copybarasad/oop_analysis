#ifndef GAME_H
#define GAME_H

#include <memory>
#include "LevelManager.h"
#include "SpellShop.h"
#include "controllers/CombatController.h"
#include "controllers/MovementController.h"
#include "controllers/SpellController.h"
#include "field_objects/EnemySpawner.h"


class Game {
    std::shared_ptr<Field> field;
    LevelManager level_manager;
    SpellShop shop;

    std::unique_ptr<MovementController> movement;
    std::unique_ptr<CombatController> combat;
    std::unique_ptr<EnemySpawner> spawner;
    std::unique_ptr<SpellController> spellController;

    bool shouldExitGame = false;
    bool shouldSaveGame = false;

    void initializeGame(std::shared_ptr<Field> field, std::shared_ptr<Player> player);
    void createControllers();

public:
    Game(std::shared_ptr<Field> field,
         std::shared_ptr<Player> player,
         LevelManager& level_manager);

    std::shared_ptr<Player> getPlayer();
    bool isPlayerAlive() const;
    bool isLevelComplete() const;
    void updateGameState();
    void advanceToNextLevel();
    void handleSkipStep();
    bool switchFightMode();
    std::shared_ptr<Field> getField() const { return field; }
    LevelManager& getLevelManager() { return level_manager; }
    const LevelManager& getLevelManager() const { return level_manager; }
    SpellShop& getSpellShop() { return shop; }
    const SpellShop& getSpellShop() const { return shop; }

    bool shouldExit() const { return shouldExitGame; }
    bool shouldSave() const { return shouldSaveGame; }
    void requestExit(bool save = false) {
        shouldExitGame = true;
        shouldSaveGame = save;
    }
};

#endif // GAME_H