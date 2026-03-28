#pragma once

class TurnManager {
private:
    bool playerTurn;

public:
    TurnManager(bool playerStarts);
    
    bool isPlayerTurn() const { return playerTurn; }
    void switchTurn() { playerTurn = !playerTurn; }
};