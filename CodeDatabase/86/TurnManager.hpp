#pragma once

enum class TurnStatus { Continue, PlayerWon, PlayerDied, SaveAndExit };

class GameWorld;
class GameContext;

class TurnManager {
    GameWorld& world_;
    GameContext& ctx_;
    int turnCount_ = 1;
    bool playerSkip_ = false;

public:
    TurnManager(GameWorld& world, GameContext& ctx,int startTurn);
    
    TurnStatus runOneTurn(); 
    
    int getTurnCount() const { return turnCount_; }
    void setTurnCount(int t) { turnCount_ = t; }

private:
    bool playerTurn(); 
    void mainEnemyTurn();
    void enemiesTurn();
    void constructionTurn();
};
