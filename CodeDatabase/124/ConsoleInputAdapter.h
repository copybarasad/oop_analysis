#ifndef CONSOLE_INPUT_ADAPTER_H
#define CONSOLE_INPUT_ADAPTER_H

#include "IInputAdapter.h"
#include "CommandProcessor.h"
#include <memory>

class ConsoleInputAdapter : public IInputAdapter {
private:
    CommandProcessor commandProcessor;
    
public:
    ConsoleInputAdapter() = default;
     
    bool handleInput(char input, Player& player, GameField& field, 
                    Enemy& enemy, EnemyManager& enemyManager, LevelManager& levelManager) override;
    
    void printHelp() override;
    bool reloadConfig() override;
};

#endif