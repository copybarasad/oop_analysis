
#pragma once

class Game;

class GameObserver {
public:
    virtual ~GameObserver() = default;
    virtual void onGameStateChanged(const Game& game) = 0;  
};