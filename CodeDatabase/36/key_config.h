#ifndef KEY_CONFIG_H
#define KEY_CONFIG_H

#include <map>
#include <string>
#include "game_action.h"

class KeyConfig {
public:
    KeyConfig();
    GameAction GetAction(char key) const;
    
    // НОВЫЙ МЕТОД: Возвращает отформатированную строку с подсказкой
    std::string GetHelpString() const;

private:
    void LoadFromFile(const std::string& filename);
    void SetDefaults();
    
    static std::map<char, GameAction> key_map_;
};

#endif