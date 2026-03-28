#ifndef CONSOLE_INPUT_READER_H
#define CONSOLE_INPUT_READER_H

#include "input_reader.h"
#include "game_config.h"
#include "command.h"
#include <iostream>
#include <memory>
#include <map>

class ConsoleInputReader : public InputReader {
private:
    GameConfig config;
    
public:
    explicit ConsoleInputReader(const std::string& configFile = "controls.cfg");
    
    // Получить отображение клавиш команд
    std::map<char, std::string> getKeyMapping() const;
    
    std::unique_ptr<Command> getNextCommand();
    GameCommand getNextGameCommand() override;
    std::pair<int, int> getSpellTarget() override;
    bool getYesNoChoice(const std::string& prompt) override;
    std::string getFilename(const std::string& prompt) override;
    const GameConfig& getConfig() const { return config; }
};

#endif