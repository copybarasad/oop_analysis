#include "gamestate.h"

GameState::GameState()
    : player(),
      playerX(0),
      playerY(0),
      enemies(),
      enemyPositions(),
      hand(3),
      currentLevel(0),
      fieldWidth(10),
      fieldHeight(10),
      blockChance(0.1),
      baseEnemyCount(1)
{}


Player& GameState::GetPlayer() { return player; }
const Player& GameState::GetPlayer() const { return player; }

int GameState::GetPlayerX() const { return playerX; }
int GameState::GetPlayerY() const { return playerY; }

std::vector<Enemy>& GameState::GetEnemies() { return enemies; }
const std::vector<Enemy>& GameState::GetEnemies() const { return enemies; }

std::vector<std::pair<int,int>>& GameState::GetEnemyPositions() { return enemyPositions; }
const std::vector<std::pair<int,int>>& GameState::GetEnemyPositions() const { return enemyPositions; }

Hand& GameState::GetHand() { return hand; }
const Hand& GameState::GetHand() const { return hand; }

int GameState::GetCurrentLevel() const { return currentLevel; }

int GameState::GetFieldWidth() const { return fieldWidth; }
int GameState::GetFieldHeight() const { return fieldHeight; }
double GameState::GetBlockChance() const { return blockChance; }
int GameState::GetBaseEnemyCount() const { return baseEnemyCount; }


void GameState::SetPlayer(const Player& newPlayer) { player = newPlayer; }
void GameState::SetPlayerPosition(int x, int y) { playerX = x; playerY = y; }

void GameState::SetEnemies(const std::vector<Enemy>& newEnemies) { enemies = newEnemies; }
void GameState::SetEnemyPositions(const std::vector<std::pair<int,int>>& positions) { enemyPositions = positions; }

void GameState::SetHand(const Hand& newHand) { hand = newHand; }

void GameState::SetCurrentLevel(int level) { currentLevel = level; }

void GameState::SetFieldSize(int w, int h) { fieldWidth = w; fieldHeight = h; }
void GameState::SetBlockChance(double chance) { blockChance = chance; }
void GameState::SetBaseEnemyCount(int value) { baseEnemyCount = value; }
