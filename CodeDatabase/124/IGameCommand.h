#ifndef IGAME_COMMAND_H
#define IGAME_COMMAND_H

#include <string>
#include <memory>

class Player;
class GameField;
class Enemy;
class EnemyManager;
class LevelManager;

class IGameCommand {
public:
    virtual ~IGameCommand() = default;
    
    virtual bool execute(Player& player, GameField& field, Enemy& enemy, 
                        EnemyManager& enemyManager, LevelManager& levelManager) = 0;
    virtual std::string getDescription() const = 0;
    virtual bool consumesTurn() const = 0;
    virtual std::string getName() const = 0;
     
    virtual std::unique_ptr<IGameCommand> clone() const = 0;
};

#endif 