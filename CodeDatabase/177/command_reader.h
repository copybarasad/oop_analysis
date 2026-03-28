#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include "command_input_interface.h"
#include "command_factory.h"
#include <istream>
#include <ostream>
#include <memory>

class GameController;

class CommandReader : public ICommandInput {
private:
    std::istream& in;
    std::ostream& out;
    CommandFactory commandFactory;
    
    bool readLine(std::string& line); 

public:
    CommandReader(std::istream& input, std::ostream& output);

    // Реализация интерфейса ICommandInput
    CommandPtr readMainMenuCommand() override;
    CommandPtr readLoadMenuCommand(int saveFilesCount) override;
    CommandPtr readInGameCommand(const GameController& game) override;
    CommandPtr readInGameMenuCommand() override;
};

#endif