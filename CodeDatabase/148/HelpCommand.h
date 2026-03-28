#ifndef HELPCOMMAND_H
#define HELPCOMMAND_H

#include "Command.h"
#include "KeyBindingConfig.h"

/**
 * @file HelpCommand.h
 * @brief Команда отображения справки
 */

/**
 * @class HelpCommand
 * @brief Команда для отображения справки по командам
 */
class HelpCommand : public Command {
private:
    const KeyBindingConfig* keyConfig;
    
public:
    explicit HelpCommand(const KeyBindingConfig* config = nullptr) 
        : keyConfig(config) {}
    
    bool execute(Game* game) override;
    bool isUndoable() const override { return false; }
    std::string getDescription() const override;
};

#endif // HELPCOMMAND_H


