#ifndef GAME_INITIALIZER_H
#define GAME_INITIALIZER_H

class Game;

class GameInitializer {
public:
    static void initializeNewGame(Game& game);
    static void spawnInitialEnemies(Game& game, int count);
    static void spawnInitialBuildings(Game& game, int count);
    static void spawnInitialTowers(Game& game, int count);
    static void initializeSpells(Game& game);
    static void displayControls();
};

#endif