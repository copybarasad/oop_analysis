#include "game_state.h"

GameState::GameState() = default;

GameState::GameState(const GameState& other)
    : currentLevel(other.currentLevel),
      playerHealthMax(other.playerHealthMax),
      playerHealth(other.playerHealth),
      playerMeleeDamage(other.playerMeleeDamage),
      playerRangedDamage(other.playerRangedDamage),
      playerScore(other.playerScore),
      fieldRows(other.fieldRows),
      fieldCols(other.fieldCols),
      playerSpells(other.playerSpells),
      entityIds(other.entityIds),
      entityHealths(other.entityHealths),
      entityTypes(other.entityTypes),
      entityPositionsRow(other.entityPositionsRow),
      entityPositionsCols(other.entityPositionsCols),
      version(other.version) {}

GameState& GameState::operator=(const GameState& other) {
    if (this != &other) {
        currentLevel = other.currentLevel;
        playerHealthMax = other.playerHealthMax;
        playerHealth = other.playerHealth;
        playerMeleeDamage = other.playerMeleeDamage;
        playerRangedDamage = other.playerRangedDamage;
        playerScore = other.playerScore;
        fieldRows = other.fieldRows;
        fieldCols = other.fieldCols;
        playerSpells = other.playerSpells;
        entityIds = other.entityIds;
        entityHealths = other.entityHealths;
        entityTypes = other.entityTypes;
        entityPositionsRow = other.entityPositionsRow;
        entityPositionsCols = other.entityPositionsCols;
        version = other.version;
    }
    return *this;
}
