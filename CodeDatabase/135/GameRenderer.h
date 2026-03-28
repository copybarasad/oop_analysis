#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "GameField.h"
#include "EntityManager.h"
#include "TurnResults.h"

class GameRenderer {
private:
	void clearScreen() const;
	void showPlayerInfo(const GameField& field, const EntityManager& entities, 
	                    int player_index) const;
	void showFieldGrid(const GameField& field, const EntityManager& entities) const;

public:
	GameRenderer() = default;
	
	void render(const GameField& field, const EntityManager& entities, 
	            int player_index, int game_level, int game_turn) const;
	
	void showLegend() const;
	
	void showPlayerTurnStart(int x, int y, bool is_slowed) const;
	void showMoveResult(const MoveResult& result) const;
	void showAttackResult(const AttackResult& result) const;
	void showWeaponChange(const WeaponChangeResult& result) const;
	void showSpellCastResult(const SpellCastResult& result) const;
	
	void showEnemyTurnHeader() const;
	void showEnemyAction(const EnemyAction& action) const;
	
	void showAllyTurnHeader() const;
	void showAllyAction(const AllyAction& action) const;
	
	void showTowerAction(const TowerAction& action) const;
	
	void showUpdateEvents(const UpdateResult& result) const;
	
	void showGameOverScreen() const;
	void showLevelCompleteScreen(int score, int threshold) const;
	void showWelcomeScreen() const;
};

#endif 
