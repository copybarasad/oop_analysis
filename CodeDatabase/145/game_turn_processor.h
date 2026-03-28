#ifndef GAME_TURN_PROCESSOR_H
#define GAME_TURN_PROCESSOR_H

class Game;

class GameTurnProcessor {
public:
    static void processPlayerTurn(Game& game, char command);
    static void processAlliesTurn(Game& game);
    static void processEnemyTurns(Game& game);
    static void processBuildingTurns(Game& game);
    static void processTowerTurns(Game& game);
    static bool processFullTurn(Game& game, char command);
};

#endif