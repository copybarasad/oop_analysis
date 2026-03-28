#ifndef GAME_KEYCONFIG_H
#define GAME_KEYCONFIG_H

#include <map>
#include <string>

namespace Game {

enum class CommandType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    SwitchCombat,
    CastSpell,
    SaveQuit,
    Quit
};

class KeyConfig {
public:
    KeyConfig(); 
    void loadFromFile(const std::string& filename); // Метод загрузки
    
    CommandType getCommandType(char key) const;
    bool isKeyBound(char key) const;

private:
    void setDefault();
    
    std::map<char, CommandType> keyToCommand;
};

}

#endif