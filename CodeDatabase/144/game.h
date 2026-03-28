#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "player.h"
#include "enemymanager.h"
#include "buildingmanager.h"
#include "game_command.h"
#include "ally.h"
#include "trap.h"
#include "game_state.h"
#include "game_persistence.h"
#include "spell.h"
#include "control_config.h"
#include "game_logging.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>

class Spell;

class Game {
private:
	struct LevelDefinition {
		int fieldWidth = 0;
		int fieldHeight = 0;
		int initialEnemies = 0;
		int buildingCount = 0;
		int towerCount = 0;
		int enemyHealth = 30;
		int enemyDamage = 10;
		int buildingHealth = 100;
		int towerHealth = 150;
		int towerRange = 4;
		int towerDamage = 8;
	};

    Field field;
    Player player;
    EnemyManager enemyManager;
    BuildingManager buildingManager;
    bool gameRunning;
    int turnCounter;
    bool playerActionTaken;
    std::vector<Ally> allies;
    std::vector<Trap> traps;
    std::vector<std::unique_ptr<Spell>> availableSpells;
    int enemiesKilledSinceLastSpell;
    const int ENEMIES_PER_SPELL = 3;
    const int SPELL_COST = 50;
	int baseFieldWidth;
	int baseFieldHeight;
	int currentLevelIndex;
	bool awaitingUpgradeSelection;
	bool awaitingRestartDecision;
	bool campaignActive;
	bool levelInProgress;
	std::string saveFilePath;
	GamePersistence persistence;
	std::map<SpellType, int> permanentSpellEnhancements;
	int currentEnemyHealth;
	int currentEnemyDamage;
	ControlConfig controls;
	GameEventBus eventBus;
	void applyPermanentEnhancementsToSpell(Spell& spell) const;
    void spawnPlayer();
    void processCellEffects(int x, int y);
    void displayHelp() const;
    void checkTraps();
    void updateAllies();
	void startLevel(int levelIndex, bool fromLoad = false);
	void resetWorldState();
	LevelDefinition buildLevelDefinition(int levelIndex) const;
	bool isLevelCleared() const;
	void handleLevelCompletion();
	void promptUpgradeSelection();
	void applyUpgradeChoice(int choice);
	void removeHalfOfPlayerSpells();
	void onPlayerDefeat();
	void saveGameState();
	void loadGameState();
	GameState buildGameState() const;
	void restoreFromState(const GameState& state);
	std::vector<SpellState> captureAvailableSpellStates() const;
	void restoreAvailableSpells(const std::vector<SpellState>& states);

public:
    Game(int fieldWidth, int fieldHeight, ControlConfig controlConfig = ControlConfig::createDefault(),
		 std::shared_ptr<GameLogSink> sink = nullptr);
    
    bool isGameRunning() const;
    bool isCampaignActive() const { return campaignActive; }
    int getTurnCounter() const { return turnCounter; }
    void displayField() const;
    void processCommand(const GameCommand& command);
    void processInput(char input);
    void update();
    void runGameLoop();
	void saveCampaign();
	void loadCampaign();
	const ControlConfig& getControls() const { return controls; }
	GameEventBus& getEventBus() { return eventBus; }
	
	// Метод для логирования событий (доступен для контекстов и других компонентов)
	void logEvent(GameEventType type, const std::string& message);

    // Геттеры для доступа к приватным полям
    Field& getField() { return field; }
    const Field& getField() const { return field; }
    
    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }
    
    EnemyManager& getEnemyManager() { return enemyManager; }
    const EnemyManager& getEnemyManager() const { return enemyManager; }
    
    BuildingManager& getBuildingManager() { return buildingManager; }
    const BuildingManager& getBuildingManager() const { return buildingManager; }
    
    std::vector<Ally>& getAllies() { return allies; }
    const std::vector<Ally>& getAllies() const { return allies; }
    
    std::vector<Trap>& getTraps() { return traps; }
    const std::vector<Trap>& getTraps() const { return traps; }
    
    void addAlly(const Ally& ally);
    void addTrap(const Trap& trap);
    void removeTrap(int index);
    void initializeSpells();
    void displaySpells() const;
    bool castSpell(int spellIndex);
    void giveRandomSpell();
    void giveSpellForEnemyKill();
    bool buySpell();
	bool isCellOccupiedByAlly(int x, int y) const;
	bool isCellOccupiedByEnemy(int x, int y) const;
	bool damageEntitiesAtPosition(int x, int y, int damageAmount);
};

#endif