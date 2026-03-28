#include "GameField.h"
#include "EnemyBuilding.h"
#include "Ally.h"
#include "MathUtils.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

GameField::GameField(int width, int height) 
	: grid(width, height), entities(), playerController() {
	if (width < MIN_FIELD_SIZE || width > MAX_FIELD_SIZE || 
		height < MIN_FIELD_SIZE || height > MAX_FIELD_SIZE) {
		throw std::invalid_argument("Field size must be between 10x10 and 25x25");
	}
	grid.Initialize();
	grid.EnsureSpawnArea(0, 0);
	playerController.SetPosition(0, 0);
	validateInvariant();
}

GameField::GameField(const GameField& other) 
	: grid(other.grid), entities(other.entities), playerController(other.playerController) {
}

GameField::GameField(GameField&& other) noexcept 
	: grid(std::move(other.grid)), entities(std::move(other.entities)), 
	  playerController(std::move(other.playerController)) {
}

GameField& GameField::operator=(const GameField& other) {
	if (this != &other) {
		copyFrom(other);
	}
	return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
	if (this != &other) {
		moveFrom(std::move(other));
	}
	return *this;
}

GameField::~GameField() = default;

int GameField::GetWidth() const {
	return grid.GetWidth();
}

int GameField::GetHeight() const {
	return grid.GetHeight();
}

int GameField::GetPlayerX() const {
	return playerController.GetX();
}

int GameField::GetPlayerY() const {
	return playerController.GetY();
}

bool GameField::IsValidPosition(int x, int y) const {
	return grid.IsValidPosition(x, y);
}

bool GameField::IsCellPassable(int x, int y) const {
	return grid.IsCellPassable(x, y);
}

bool GameField::IsCellSlowing(int x, int y) const {
	return grid.IsCellSlowing(x, y);
}

void GameField::SetPlayerPosition(int x, int y) {
	if (!IsValidPosition(x, y)) {
		throw std::invalid_argument("Invalid player position");
	}
	playerController.SetPosition(x, y);
}

void GameField::SetCellType(int x, int y, CellType type) {
	grid.SetCellType(x, y, type);
}

bool GameField::MovePlayer(int deltaX, int deltaY) {
	return playerController.Move(deltaX, deltaY, grid);
}

std::vector<int> GameField::MoveEnemies() {
	return entities.MoveEnemies(grid, GetPlayerX(), GetPlayerY());
}

void GameField::UpdateBuildings() {
	entities.UpdateBuildings(grid, GetPlayerX(), GetPlayerY());
}

void GameField::AddEnemy(const Enemy& enemy) {
	entities.AddEnemy(enemy);
}

void GameField::AddEnemyBuilding(const EnemyBuilding& building) {
	entities.AddEnemyBuilding(building);
}

void GameField::AddEnemyTower(const EnemyTower& tower) {
	entities.AddEnemyTower(tower);
}

void GameField::RemoveDeadEnemies() {
	entities.RemoveDeadEnemies();
}

void GameField::RemoveDeadBuildings() {
	entities.RemoveDeadBuildings();
}

void GameField::AttackEnemyAt(int x, int y, int damage) {
	entities.AttackEnemyAt(x, y, damage);
}

void GameField::AttackBuildingAt(int x, int y, int damage) {
	entities.AttackBuildingAt(x, y, damage);
}

void GameField::AttackArea(int startX, int startY, int areaSize, int damage) {
	entities.AttackArea(startX, startY, areaSize, damage);
}

int GameField::GetTowerDamageAt(int x, int y) const {
	int distance = MathUtils::CalculateDistance(GetPlayerX(), GetPlayerY(), x, y);
	
	for (size_t i = 0; i < entities.GetTowerCount(); ++i) {
		const EnemyTower& tower = entities.GetTowerAt(i);
		if (tower.GetX() == x && tower.GetY() == y) {
			if (distance <= tower.GetAttackRadius() && tower.CanAttack()) {
				return tower.GetDamage();
			}
		}
	}
	return 0;
}

bool GameField::IsEnemyAt(int x, int y) const {
	return entities.IsEnemyAt(x, y);
}

bool GameField::IsBuildingAt(int x, int y) const {
	return entities.IsBuildingAt(x, y);
}

bool GameField::IsTowerAt(int x, int y) const {
	return entities.IsTowerAt(x, y);
}

bool GameField::IsTrapAt(int x, int y) const {
	return entities.IsTrapAt(x, y);
}

bool GameField::IsAllyAt(int x, int y) const {
	return entities.IsAllyAt(x, y);
}

void GameField::PlaceTrap(int x, int y, int damage) {
	entities.PlaceTrap(x, y, damage);
}

void GameField::CheckTraps() {
	entities.CheckTraps(grid);
}

void GameField::UpdateTowers() {
	for (size_t i = 0; i < entities.GetTowerCount(); ++i) {
		EnemyTower& tower = entities.GetTowerAt(i);
		tower.Update(*this);
	}
}

void GameField::AddAlly(const Ally& ally) {
	entities.AddAlly(ally);
}

std::vector<std::pair<int, int>> GameField::UpdateAllies() {
	std::vector<std::pair<int, int>> attacks;
	for (size_t i = 0; i < entities.GetAllyCount(); ++i) {
		Ally& ally = entities.GetAllyAt(i);
		if (!ally.IsAlive()) continue;
		ally.Move(*this);
		if (ally.Attack(*this)) {
			attacks.push_back(std::make_pair(ally.GetX(), ally.GetY()));
		}
	}
	return attacks;
}

void GameField::RemoveDeadAllies() {
	entities.RemoveDeadAllies();
}

void GameField::Display() const {
	std::cout << "\n";
	
	for (int y = 0; y < GetHeight(); ++y) {
		if (y < 10) {
			std::cout << " " << y << " ";
		} else {
			std::cout << y << " ";
		}
		for (int x = 0; x < GetWidth(); ++x) {
			if (x == GetPlayerX() && y == GetPlayerY()) {
				std::cout << "P ";
			} else if (IsEnemyAt(x, y)) {
				std::cout << "E ";
			} else if (IsTowerAt(x, y)) {
				std::cout << "T ";
			} else if (IsAllyAt(x, y)) {
				std::cout << "A ";
			} else if (IsBuildingAt(x, y)) {
				std::cout << "B ";
			} else if (IsTrapAt(x, y)) {
				std::cout << "X ";
			} else {
				std::cout << grid.GetCellSymbol(x, y) << " ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\nLegend: P=Player, E=Enemy, A=Ally, B=Building, T=Tower, X=Trap, .=Normal, #=Wall, ~=Slow\n";
	std::cout << "Coordinates: Y (left) and X (count from left, starting from 0)\n";
}

size_t GameField::GetEnemyCount() const {
	return entities.GetEnemyCount();
}

size_t GameField::GetBuildingCount() const {
	return entities.GetBuildingCount();
}

size_t GameField::GetTowerCount() const {
	return entities.GetTowerCount();
}

size_t GameField::GetAllyCount() const {
	return entities.GetAllyCount();
}

size_t GameField::GetTrapCount() const {
	return entities.GetTrapCount();
}

const Enemy& GameField::GetEnemyByIndex(size_t index) const {
	return entities.GetEnemyAt(index);
}

const EnemyBuilding& GameField::GetBuildingByIndex(size_t index) const {
	return entities.GetBuildingAt(index);
}

const EnemyTower& GameField::GetTowerByIndex(size_t index) const {
	return entities.GetTowerAt(index);
}

const Ally& GameField::GetAllyByIndex(size_t index) const {
	return entities.GetAllyAt(index);
}

const Trap& GameField::GetTrapByIndex(size_t index) const {
	return entities.GetTrapAt(index);
}

int GameField::GetEnemyDamageAt(size_t index) const {
	return entities.GetEnemyAt(index).GetCurrentDamage();
}

int GameField::GetEnemyX(size_t index) const {
	return entities.GetEnemyAt(index).GetX();
}

int GameField::GetEnemyY(size_t index) const {
	return entities.GetEnemyAt(index).GetY();
}

bool GameField::IsEnemyAliveAt(size_t index) const {
	return entities.GetEnemyAt(index).IsAlive();
}

void GameField::AttackEnemyByIndex(size_t index, int damage) {
	entities.GetEnemyAt(index).TakeDamage(damage);
}

int GameField::GetTowerX(size_t index) const {
	return entities.GetTowerAt(index).GetX();
}

int GameField::GetTowerY(size_t index) const {
	return entities.GetTowerAt(index).GetY();
}

void GameField::copyFrom(const GameField& other) {
	grid = other.grid;
	entities = other.entities;
	playerController = other.playerController;
}

void GameField::moveFrom(GameField&& other) noexcept {
	grid = std::move(other.grid);
	entities = std::move(other.entities);
	playerController = std::move(other.playerController);
}

void GameField::validateInvariant() const {
	if (GetWidth() < MIN_FIELD_SIZE || GetWidth() > MAX_FIELD_SIZE) {
		throw std::runtime_error("Field width is out of valid range");
	}
	if (GetHeight() < MIN_FIELD_SIZE || GetHeight() > MAX_FIELD_SIZE) {
		throw std::runtime_error("Field height is out of valid range");
	}
	if (!IsValidPosition(GetPlayerX(), GetPlayerY())) {
		throw std::runtime_error("Player position is invalid");
	}
}

