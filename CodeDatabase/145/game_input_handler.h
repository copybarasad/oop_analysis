#ifndef GAME_INPUT_HANDLER_H
#define GAME_INPUT_HANDLER_H

#include <string>

class Game;

class GameInputHandler {
public:
    static char getPlayerCommand();
    static std::pair<int, int> getSpellTarget();
    static bool getYesNoChoice(const std::string& prompt);
    static std::string getFilename(const std::string& prompt);
};

#endif