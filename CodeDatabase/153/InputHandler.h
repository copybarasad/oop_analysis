#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Command.h"
#include "CommandType.h"
#include <utility>
#include <memory>

class ShopSystem;
class GameSaveSystem;

class InputHandler {
public:
    virtual ~InputHandler() = default;

    // НОВЫЕ МЕТОДЫ:
    virtual CommandType getCommandType() = 0;                     
    virtual std::unique_ptr<Command> createCommand(CommandType type) = 0; 

    // Старые методы остаются:
    virtual std::pair<int, int> getSpellTarget() = 0;
    virtual int getSpellChoice(int maxSpells) = 0;
    virtual int getShopChoice(int maxSpells) = 0;

    virtual void setShopSystem(ShopSystem& shopSystem) = 0;
    virtual void setSaveSystem(GameSaveSystem& saveSystem) = 0;
};

#endif 