#ifndef KEY_BINDINGS_INFO
#define KEY_BINDINGS_INFO

#include <string>
#include <unordered_map>

struct KeyBindingsInfo{
    std::unordered_map <std::string, std::string> mainMenuBindings;
    std::unordered_map <std::string, std::string> playingBindings;
    std::unordered_map <std::string, std::string> levelCompleteBindings;
    std::unordered_map <std::string, std::string> gameOverBindings;
    std::unordered_map <std::string, std::string> upgradingBindings;
    std::unordered_map <std::string, std::string> buyingSpellBindings;
};

#endif