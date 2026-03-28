#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include "player.h"
#include "enemy.h"
#include "game_field.h"
#include "book_of_spells.h"
#include "player_hand.h"
#include "event_dispatcher.h"

enum class Direction { Up, Down, Left, Right };

class Game {
public:
    Game();
    ~Game();

    const std::vector<Enemy>& getEnemies() const;
    void castSpell();
    bool allEnemiesDead() const;

    bool isRunning() const;
    bool isInGame() const;

    void startNewGame();
    bool loadGame();
    bool saveGame() const;
    void returnToMenu();
    void gameOver();

    void movePlayer(Direction dir);
    void castSpellAt(int x, int y);
    void chooseSpellByIndex(int index);
    void nextLevel();

    void enemiesMove();
    void towersUpdate();
    

    Player* getPlayerPtr();
    GameField* getFieldPtr();
    std::vector<Enemy>& getEnemies();
    std::vector<std::string> getAvailableSpells() const;

    void setEventDispatcher(EventDispatcher* d);

private:
    void cleanup();

    BookOfSpells book;
    std::unique_ptr<HandSpell> handSpell;
    std::unique_ptr<Player> player;
    GameField* field;
    std::vector<Enemy> enemies;

    bool running;
    bool inGame;
    int currentLevel;

    EventDispatcher* dispatcher;
};

#endif
