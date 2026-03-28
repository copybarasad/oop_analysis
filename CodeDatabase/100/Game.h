#pragma once
#include "GameState.h"
#include "CombatSystem.h"
//#include "../interface/GameVisualizer.h"
#include "../interface/ConsoleRenderer.h"
#include "../game_spells/SpellManager.h"
#include "SaveManager.h"
#include "../game_spells/DirectDamageSpell.h"
#include "../game_spells/AreaDamageSpell.h"
#include "../game_spells/TrapSpell.h"
#include "../game_objects/PlayerHand.h"
#include "../game_spells/SpawnAllySpell.h"
#include "../game_spells/UpgradeSpell.h"
#include "../commands/ConsoleInputProcessor.h"

class Game {
private:
    GameState state;
    CombatSystem combat;
    SpellManager spellManager;
    SaveManager saveManager;

    bool exitRequested = false;
    bool pauseRequested = false;
    bool saveRequested = false;
    bool loadRequested = false;

    void requestExit();
    void requestSave();
    void requestLoad();

    bool isExitRequested() const;
    bool isPauseRequested() const;
    bool isSaveRequested() const;
    bool isLoadRequested() const;

    void clearFlags();

    // Инциализация всех сущностей
    void initializeGame();
    void initializeCharacters(); 
    void spawnEnemyTower();
    void spawnEnemyBuilding();   

    // Метод для выдачи заклинаний
    void tryGiveRandomSpell(); 

    // Меню выбора
    bool showGameOver(); 
    void showPauseMenu();
    void levelUpMenu();

    // Функция запроса названия файла для сохранения / загрузки
    std::string askFilename();

    // Переход на следующий уровень
    void nextLevel();
    
    void clearScreen();

    std::mt19937 rng;

public:
    Game(int fieldWidth, int fieldHeight, int playerHealth, int playerDamage);
    ~Game();

    void run();
    void startLevel();
    void showMainMenu();
    void restartGameState();
    void requestPause();
    GameState& getGameState();

    void movePlayer(int dx, int dy);
    void switchCombatMode();
    void castSpell();
    void pauseGame();
};