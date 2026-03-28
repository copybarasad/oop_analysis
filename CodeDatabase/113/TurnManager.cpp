#include "TurnManager.h"
#include "GameSession.h"
#include <iostream>

void TurnManager::processPlayerTurn(GameSession* session) {
    if (!session) {
        std::cout << "TurnManager: null session" << std::endl;
        return;
    }

    try {
        auto& gameState = session->getGameState();
        gameState.turnCount++;
        m_currentTurn = gameState.turnCount;

        std::cout << "Turn " << m_currentTurn << " processed." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in processPlayerTurn: " << e.what() << std::endl;
    }
}