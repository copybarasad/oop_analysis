#include "GameState.h"
#include "Constants.h"

GameState::GameState() 
    : currentLevel(1), playerHp(PLAYER_DEFAULT_HP), playerBaseDmg(PLAYER_DEFAULT_BASE_DMG),
      playerHandCapacity(PLAYER_HAND_CAPACITY), playerScore(0),
      fieldWidth(15), fieldHeight(15) {}

GameState::GameState(int level, Player* player, GameField* field)
    : currentLevel(level), fieldWidth(field->getWidth()), fieldHeight(field->getHeight()) {
    if (player) {
        playerHp = player->getHp();
        playerBaseDmg = player->getBaseDamage();
        playerScore = player->getScore();
        if (player->getHand()) {
            playerHandCapacity = player->getHand()->getCapacity();
            const auto& spells = player->getHand()->getSpells();
            playerSpells.clear();
            for (const auto* spell : spells) {
                playerSpells.push_back(spell->name());
            }
        } else {
            playerHandCapacity = PLAYER_HAND_CAPACITY;
        }
    } else {
        playerHp = PLAYER_DEFAULT_HP;
        playerBaseDmg = PLAYER_DEFAULT_BASE_DMG;
        playerScore = 0;
        playerHandCapacity = PLAYER_HAND_CAPACITY;
    }
}

void GameState::updateFromGame(int level, Player* player, GameField* field) {
    currentLevel = level;
    fieldWidth = field->getWidth();
    fieldHeight = field->getHeight();
    if (player) {
        playerHp = player->getHp();
        playerBaseDmg = player->getBaseDamage();
        playerScore = player->getScore();
        if (player->getHand()) {
            playerHandCapacity = player->getHand()->getCapacity();
            const auto& spells = player->getHand()->getSpells();
            playerSpells.clear();
            for (const auto* spell : spells) {
                playerSpells.push_back(spell->name());
            }
        }
    }
}

