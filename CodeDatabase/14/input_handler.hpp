#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <map>
#include <string>

class Game;   
class Command; 

class ConsoleInputHandler {
private:
    std::map<std::string, std::string> keyBindings;

    void loadDefaultConfig();

public:
    ConsoleInputHandler();
    void loadConfig(const std::string& filename);
    std::string getKey(const std::string& action) const;
    Command* handleInput(Game& game);
};

#endif