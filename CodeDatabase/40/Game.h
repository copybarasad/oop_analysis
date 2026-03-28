#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "SpellHand.h"
#include "SpellShop.h"
#include "Spell.h"
#include "BOMB.h"
#include "fireball.h"
#include <vector>

class GameRunner;

class Game{
private:
    Player player;
    std::vector<Enemy> enemies;
    Field field;
    SpellShop spellShop;

    GameRunner* gameRunnerRef = nullptr;
    
    void visitShop();
    void showSpellMenu();
    void buySpell(int spellIndex);
    void castSpell();

    void getPlayerInput();
    void handleEnemyTurn();
    void printMap() const;

public:
    Game();
    Game(GameRunner* runner);
    
    void setGameRunnerRef(GameRunner* runner);
    void requestSave();

    void save(std::ostream& out) const;
    void load(std::istream& in);
    bool isPlayerAlive() const;

    void damageEnemyAt(int x, int y, int damage, int radius = 0);
    bool allEnemiesDead() const;
    void run();
};

#endif