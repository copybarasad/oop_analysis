#ifndef IINPUT_ADAPTER_H
#define IINPUT_ADAPTER_H
 
class Player;
class GameField;
class Enemy;
class EnemyManager;
class LevelManager;

class IInputAdapter {
public:
    virtual ~IInputAdapter() = default;
    
    virtual bool handleInput(char input, Player& player, GameField& field, 
                           Enemy& enemy, EnemyManager& enemyManager, LevelManager& levelManager) = 0;
    virtual void printHelp() = 0;
    virtual bool reloadConfig() = 0;
};

#endif