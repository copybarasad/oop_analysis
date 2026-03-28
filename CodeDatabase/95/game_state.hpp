#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "entity_manager.hpp"
#include <string>
#include <iostream>

enum class GameStatus {
    Running,
    Paused,
    Win,
    Lose
};

class GameState {
private:
    eManager entityManager;
    GameStatus status;
    int turn_counter;
    
public:
    GameState();
    
    eManager& getEntityManager();
    const eManager& getEntityManager() const;
    GameStatus getStatus() const;
    int getTurnCounter() const;

    bool isRunning() const;
    void setStatus(GameStatus s);
    void setTurnCounter(int turns);
    void incrementTurnCounter();

    friend class SaveManager;
};

#endif
