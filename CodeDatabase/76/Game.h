#pragma once
#include "Player.h"
#include "Enemy.h"
#include "PlayingField.h"
#include "SpellHand.h"
#include "GameSaveManager.h"
#include "InputHandler.h"
#include "Renderer.h"
#include <memory>

class ExtendedGame;

class Game {
private:
    friend class ExtendedGame;


    PlayingField* field;
    Player player;
    Enemy enemy;
    SpellHand spellHand;
    bool isRunning;
    int currentLevel;

    void InitializePlayer();
    void StartLevel(int level);
    
    int GetFieldSize();
    int GetBarriersCount();
    int GetCombatRange();
    char GetPlayerInput();
    
    void ProcessPlayerInput(char input);
    void HandleCombatRangeChange(int combatRange);
    
    void ExecuteGameTurn(bool playerMoved, bool spellCasted);
    bool CheckPlayerDefeat();
    
    void HandleGameOver();
    bool OfferRestart();
    void HandleEnemyDefeat();
    void RemoveRandomSpells(int count);

public:
    Game();
    ~Game();

    void HandleLevelCompletion();

    void HandleSpellPurchase();
    bool HandleSpellCasting(int spellIndex = -1);
    
    void StartGame();
    void GameLoop();
    void DebugFieldState();

    PlayingField* GetField() const { return field; }
    Player& GetPlayer() { return player; }
    const Player& GetPlayer() const { return player; }
    Enemy& GetEnemy() { return enemy; }
    const Enemy& GetEnemy() const { return enemy; }
    SpellHand& GetSpellHand() { return spellHand; }
    const SpellHand& GetSpellHand() const { return spellHand; }
    int GetCurrentLevel() const { return currentLevel; }
    bool IsRunning() const { return isRunning; }
    void SetRunning(bool running) { isRunning = running; }

    void SetField(PlayingField* newField) { field = newField; }
    void SetCurrentLevel(int level) { currentLevel = level; }

    friend class GameSaveManager;
};

