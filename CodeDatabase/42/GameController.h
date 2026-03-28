#pragma once
#include "GameField.h"
#include "Player.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SpellFactory.h"
#include "SaveData.h"
#include "SaveManager.h"
#include <random>
#include <memory>

class GameController {
private:
    GameField field;
    Player player;
    bool gameRunning;
    int turnCount;
    std::mt19937 randomGenerator;
    
    SaveManager saveManager;
    
    void initializeGame();
    void spawnEnemies();
    void displaySpellMenu() const;
    int getSpellChoice();
    void processSpellCast(int spellIndex);
    void castDirectDamageSpell(DirectDamageSpell* spell, int spellIndex);
    void castAreaDamageSpell(AreaDamageSpell* spell, int spellIndex);
    void grantInitialSpell();
    void checkAndGrantSpells();
    void castTrapSpell(TrapSpell* spell, int spellIndex);
    
    void showInGameSaveMenu();
    void showInGameLoadMenu();
    
public:
    GameController();
    
    void startNewGame();
    
    void runGame();
    void processPlayerTurn();
    void processEnemyTurns();
    void checkGameState();
    
    bool isPlayerAlive() const { return player.isAlive(); }
    bool isGameRunning() const { return gameRunning; }
    
    int getTurnCount() const { return turnCount; }
    int getFieldWidth() const { return field.getWidth(); }
    int getFieldHeight() const { return field.getHeight(); }

    SaveData getSaveData() const;
    void loadFromSave(const SaveData& data);
    void resetForLoad();
};