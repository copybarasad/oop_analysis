#include "gamerenderer.h"
#include "game.h"
#include "field.h"
#include "player.h"
#include "enemymanager.h"
#include "buildingmanager.h"
#include "ally.h"
#include <iostream>

GameRenderer::GameRenderer(const Game& game) : game(game) {}

void GameRenderer::displayGameStats(int turnCounter) const {
	const Player& player = game.getPlayer();
	const EnemyManager& enemyManager = game.getEnemyManager();
	const BuildingManager& buildingManager = game.getBuildingManager();
	const std::vector<Ally>& allies = game.getAllies();
	
	std::cout << "\nTurn " << turnCounter;
	std::cout << " | HP: " << player.getHealth() << "/" << player.getMaxHealth();
	std::cout << " | Mana: " << player.getMana() << "/" << player.getMaxMana();
	std::cout << " | Score: " << player.getScore();
	std::cout << " | Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "Melee" : "Ranged");
	std::cout << " | Enhancement: " << player.getCurrentEnhancementLevel();
	std::cout << " | Slowed: " << (player.isSlowed() ? "Yes" : "No");
	std::cout << " | Spells: " << player.getHand().getSpellCount() << "/" << player.getHand().getMaximumSize();
	std::cout << " | Enemies: " << enemyManager.getEnemies().size();
	std::cout << " | Allies: " << allies.size();
	std::cout << " | Towers: " << buildingManager.getTowers().size();
	std::cout << "\n";
	
	for (size_t i = 0; i < allies.size(); ++i) {
		if (allies[i].isAlive()) {
			std::cout << "Ally " << i << " HP: " << allies[i].getHealth() << "  ";
		}
	}
	if (!allies.empty()) {
		std::cout << "\n";
	}
}

void GameRenderer::displayField(int turnCounter) const {
	displayGameStats(turnCounter);
	
	const Field& field = game.getField();
	const Player& player = game.getPlayer();
	const EnemyManager& enemyManager = game.getEnemyManager();
	const BuildingManager& buildingManager = game.getBuildingManager();
	const std::vector<Ally>& allies = game.getAllies();
	
	int playerX = player.getX();
	int playerY = player.getY();
	
	for (int y = 0; y < field.getHeight(); ++y) {
		for (int x = 0; x < field.getWidth(); ++x) {
			if (x == playerX && y == playerY) {
				std::cout << "P ";
				continue;
			}
			
			bool hasAlly = false;
			for (const auto& ally : allies) {
				if (ally.isAlive() && ally.getPositionX() == x && ally.getPositionY() == y) {
					std::cout << "A ";
					hasAlly = true;
					break;
				}
			}
			if (hasAlly) continue;
			
			bool hasEnemy = false;
			for (const auto& enemy : enemyManager.getEnemies()) {
				if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
					std::cout << "E ";
					hasEnemy = true;
					break;
				}
			}
			if (hasEnemy) continue;
			
			bool hasTower = false;
			for (const auto& tower : buildingManager.getTowers()) {
				if (tower.getX() == x && tower.getY() == y) {
					std::cout << "T ";
					hasTower = true;
					break;
				}
			}
			if (hasTower) continue;
			
			bool hasBuilding = false;
			for (const auto& building : buildingManager.getBuildings()) {
				if (building.getX() == x && building.getY() == y) {
					std::cout << "B ";
					hasBuilding = true;
					break;
				}
			}
			if (hasBuilding) continue;
			
			switch (field.getCellType(x, y)) {
				case CellType::WALL: std::cout << "# "; break;
				case CellType::SLOW: std::cout << "~ "; break;
				case CellType::EMPTY: std::cout << ". "; break;
			}
		}
		std::cout << "\n";
	}
}