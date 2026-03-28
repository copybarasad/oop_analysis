#include "TurnOrchestrator.h"
#include "TurnCoordinator.h"
#include "GameRenderer.h"
#include "GameField.h"
#include "EntityManager.h"
#include "IPlayerCharacter.h"
#include "TurnResults.h"
#include <iostream>

TurnOrchestrator::TurnOrchestrator(TurnCoordinator& tc, GameRenderer& rend, GameField& f, 
                                   EntityManager& em, int& player_idx)
	: turn_coordinator(tc), renderer(rend), field(f), entity_manager(em), player_index(player_idx) {}

void TurnOrchestrator::showPlayerTurnStart() {
	int player_x, player_y;
	field.indexToCoord(player_index, player_x, player_y);
	
	auto* entity = entity_manager.getEntity(player_index);
	bool is_slowed = false;
	
	if (entity) {
		auto* player_char = dynamic_cast<IPlayerCharacter*>(entity);
		if (player_char) {
			is_slowed = player_char->isSlowed();
		}
	}
	
	renderer.showPlayerTurnStart(player_x, player_y, is_slowed);
	
	if (!is_slowed) {
		std::cout << "\nКоманда: ";
	}
}

void TurnOrchestrator::executeFullTurn() {
	PlayerTurnResult player_result = turn_coordinator.processPlayerTurn();
	displayPlayerTurnResult(player_result);
	
	AllyTurnResult ally_result = turn_coordinator.processAllyTurn();
	displayAllyTurnResult(ally_result);
	
	EnemyTurnResult enemy_result = turn_coordinator.processEnemyTurn();
	displayEnemyTurnResult(enemy_result);
	
	TowerTurnResult tower_result = turn_coordinator.processTowerAttacks();
	displayTowerTurnResult(tower_result);
	
	UpdateResult update_result = turn_coordinator.update();
	displayUpdateResult(update_result);
}

bool TurnOrchestrator::isPlayerAlive() const {
	return turn_coordinator.isPlayerAlive();
}

void TurnOrchestrator::displayPlayerTurnResult(const PlayerTurnResult& result) {
	switch (result.action) {
		case TurnAction::SLOWED:
			renderer.showPlayerTurnStart(result.player_x, result.player_y, true);
			break;
		case TurnAction::MOVE:
			renderer.showMoveResult(result.move);
			break;
		case TurnAction::ATTACK:
			renderer.showAttackResult(result.attack);
			break;
		case TurnAction::WEAPON_CHANGE:
			renderer.showWeaponChange(result.weapon);
			break;
		case TurnAction::SPELL_CAST:
			renderer.showSpellCastResult(result.spell);
			break;
		case TurnAction::SHOP_VISIT:
			break;
		default:
			break;
	}
}

void TurnOrchestrator::displayEnemyTurnResult(const EnemyTurnResult& result) {
	if (result.actions.empty()) return;
	
	renderer.showEnemyTurnHeader();
	for (const auto& action : result.actions) {
		renderer.showEnemyAction(action);
	}
}

void TurnOrchestrator::displayAllyTurnResult(const AllyTurnResult& result) {
	if (result.actions.empty()) return;
	
	renderer.showAllyTurnHeader();
	for (const auto& action : result.actions) {
		renderer.showAllyAction(action);
	}
}

void TurnOrchestrator::displayTowerTurnResult(const TowerTurnResult& result) {
	if (result.actions.empty()) return;
	
	for (const auto& action : result.actions) {
		renderer.showTowerAction(action);
	}
}

void TurnOrchestrator::displayUpdateResult(const UpdateResult& result) {
	renderer.showUpdateEvents(result);
}
