#include "GameRenderer.h"

template<typename TOutputHandler>
GameRenderer<TOutputHandler>::GameRenderer(Game* gameInstance)
    : game(gameInstance), 
      outputHandler(std::make_unique<TOutputHandler>()),
      isRunning(true) {
}

template<typename TOutputHandler>
void GameRenderer<TOutputHandler>::displayField(GameField& field) {
    outputHandler->displayField(field);
}

template<typename TOutputHandler>
void GameRenderer<TOutputHandler>::displayPlayer(Player& player) {
    outputHandler->displayPlayer(player);
}

template<typename TOutputHandler>
void GameRenderer<TOutputHandler>::displaySpells(Player& player) {
    outputHandler->displaySpells(player);
}

template<typename TOutputHandler>
void GameRenderer<TOutputHandler>::displayVictory(Game& game) {
    outputHandler->displayVictory(game);
}

template<typename TOutputHandler>
void GameRenderer<TOutputHandler>::displayDefeat(Game& game) {
    outputHandler->displayDefeat(game);
}

template<typename TOutputHandler>
void GameRenderer<TOutputHandler>::displayFinishLevel(Game& game) {
    outputHandler->displayFinishLevel(game);
}

template<typename TOutputHandler>
void GameRenderer<TOutputHandler>::displayUpgradeChoice(Game& game) {
    outputHandler->displayUpgradeChoice(game);
}

template class GameRenderer<OutputTerminal>;