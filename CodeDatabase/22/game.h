#pragma once
#include "field/field.h"
#include "characters/player.h"
#include "spells/hand.h"
#include "spells/improveSpell.h"
#include "configs/gameConf.h"
#include <vector>
#include <random>
#include <iostream>
#include "saveLoadManager.h"
#include "eventManager.h"


class IGameView;
class IInputHandler;
struct Command;


class Game
{
private:
    std::unique_ptr<Player> player_;
    Field field_;
    bool isRunning_ = true;
    int level_;
    EnemyManager enemyManager_;
    SpellManager spellManager_;
    conf_t gameConf;
    SaveLoadManager saveManager_;
    EventManager eventManager_;

public:
    Game();

    void gameOver();
    void gameLoop();
    void printSettings();
    void newLevel(char improvementOpt, IGameView &view, IInputHandler &inputHandler);
    void improvePlayer(char opt, IGameView &view, IInputHandler &inputHandler);
    void handleCommand(const Command &cmd, IGameView &view, IInputHandler &inputHandler);
    bool isRunning() const;
    Player *getPlayer() const;
    Field &getField();
    const Field &getField() const ;
    const EnemyManager &getEnemyManager() const;

    EnemyManager &getEnemyManager();
    SpellManager &getSpellManager();
    int getLevel() const;
    void update(Command upOpt, IGameView &view, IInputHandler &inputHandler);
    void loadGame(IGameView &view, IInputHandler &inputHandler);
    bool levelUp();
    std::string selectLoad(IGameView &view, IInputHandler &inputHandler);

    void setLogger(std::shared_ptr<ILogger> logger);
    void movePlayer(Position newPos);
    void attack(int damage, int attackRadius);
    void receiveSpell(const std::string &spellName);
};
