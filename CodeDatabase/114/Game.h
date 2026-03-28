#ifndef GAME_H
#define GAME_H

#include "LevelManager.h"
#include "SaveLoadManager.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "EnemyBuildingController.h"
#include "PetController.h"
#include "HandController.h"
#include "../spells/SpellFactory.h"
#include <memory>
#include <string>

class Game {
private:
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<SaveLoadManager> saveLoadManager;
    
    std::unique_ptr<PlayerController> playerController;
    std::unique_ptr<EnemyController> enemyController;
    std::unique_ptr<EnemyBuildingController> buildingController;
    std::unique_ptr<PetController> petController;
    std::unique_ptr<HandController> handController;
    
    std::unique_ptr<SpellFactory> spellFactory;
    
    void initializeControllers();
    void handleStartChoice();
    
public:
    Game();
    ~Game() = default;
    
    // Методы для управления игрой
    void startGame();  // Главный метод запуска игры (обрабатывает выбор)
    void startNewGame();
    void restartGame();
    void nextLevel(bool upgradeHealth, bool upgradeDamage);
    void saveCurrentGame(const std::string& filename);
    void loadGameFromFile(const std::string& filename);
    
    // Методы для получения контроллеров (для GameManager)
    PlayerController& getPlayerController() { return *playerController; }
    EnemyController& getEnemyController() { return *enemyController; }
    EnemyBuildingController& getBuildingController() { return *buildingController; }
    PetController& getPetController() { return *petController; }
    HandController& getHandController() { return *handController; }
    
    // Методы для получения состояния игры
    int getCurrentLevel() const { return levelManager->getCurrentLevel(); }
    GameField& getField() { return levelManager->getField(); }
    const GameField& getField() const { return levelManager->getField(); }
    
};

#endif

