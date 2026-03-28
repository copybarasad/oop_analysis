#ifndef CONSOLEINPUTHANDLER_H
#define CONSOLEINPUTHANDLER_H

#include "InputHandler.h"
#include <memory>
#include <limits>

class ConsoleInputHandler : public InputHandler {
private:
    ShopSystem* shopSystem;
    GameSaveSystem* saveSystem;

    char getRawInput();

public:
    ConsoleInputHandler();

    CommandType getCommandType() override;
    std::unique_ptr<Command> createCommand(CommandType type) override;


    std::pair<int, int> getSpellTarget() override;
    int getSpellChoice(int maxSpells) override;
    int getShopChoice(int maxSpells) override;

    void setShopSystem(ShopSystem& shopSystem) override;
    void setSaveSystem(GameSaveSystem& saveSystem) override;
};

#endif