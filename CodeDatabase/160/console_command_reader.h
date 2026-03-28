#ifndef INCLUDE_IO_CONSOLE_COMMAND_READER
#define INCLUDE_IO_CONSOLE_COMMAND_READER

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "command.h"

class ConsoleCommandReader {
private:
    static void ClearBadInputLine();
    static char ReadChar();
    static int ReadInt();

public:
    Command ReadStartCommand();
    Command ReadMainCommand(bool inventoryShown);
    Command ReadLoadCommand(const std::vector<std::string>& saves);
    Command ReadShopCommand();
    Command ReadTargetSelectionCommand();
    Command ReadDirectionSelectionCommand();
    Command ReadConfirmAttackCommand();
    Command ReadUpgradeChoiceCommand();
    void WaitForEnter();
};

#endif
