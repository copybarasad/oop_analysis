#ifndef TURNMANAGER_H
#define TURNMANAGER_H

class GameSession;

class TurnManager {
private:
    int m_currentTurn = 0;

public:
    void processPlayerTurn(GameSession* session);
    int getCurrentTurn() const { return m_currentTurn; }
};

#endif // TURNMANAGER_H