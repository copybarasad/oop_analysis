#ifndef GAME
#define GAME

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Building.h"
#include "GameColleague.h"

#include <SFML/Graphics.hpp>

// #include "Screens/IScreen.h"
using namespace sf;

class Game : public GameColleague
{
protected:
    shared_ptr<Map> mMap;
    shared_ptr<Player> mPlayer;
    shared_ptr<vector<shared_ptr<Enemy>>> mEnemies;
    shared_ptr<Building> mBuilding;
    shared_ptr<size_t> mLevel;

public:
    Game();

    bool gameOver() const;

    bool isWin() const;

    void enemyMove();

    Vec2 getSize() const;

    void setNewLevel();

    void newGame();

    size_t getLevel() const;

    shared_ptr<Player> getPlayer();

    Map *getMap() const;

    Cell getCell(Vec2 pos);

    void moveCharacterTo(shared_ptr<Character>, Vec2 dest);

    void setMediator(GameSaveMediator *mediator);

    void putEverythingOnTheMap();

private:
    void randomEnemyAction(size_t);

    Vec2 getRandomDirection(bool isCross) const;
};

#endif