#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include "Command.h"
#include "GameStatus.h"
#include <unordered_map>
#include <vector>
#include <string>

// Класс для парсинга команд
class CommandParser {
private:
    struct CommandMapping {
        char key;
        CmdType type;
        std::string description;
    };
    
    std::unordered_map<GameStatus, std::vector<CommandMapping>> mappings;
    
    void initMappings() {
        // Меню
        mappings[GameStatus::MENU] = {
            {'1', CmdType::MENU_SELECT_1, "Новая игра"},
            {'2', CmdType::MENU_SELECT_2, "Загрузить игру"},
            {'3', CmdType::MENU_SELECT_3, "Выход"}
        };
        
        // Игровой процесс
        mappings[GameStatus::PLAYING] = {
            {'w', CmdType::MOVE_UP, "Движение вверх"},
            {'W', CmdType::MOVE_UP, "Движение вверх"},
            {'s', CmdType::MOVE_DOWN, "Движение вниз"},
            {'S', CmdType::MOVE_DOWN, "Движение вниз"},
            {'a', CmdType::MOVE_LEFT, "Движение влево"},
            {'A', CmdType::MOVE_LEFT, "Движение влево"},
            {'d', CmdType::MOVE_RIGHT, "Движение вправо"},
            {'D', CmdType::MOVE_RIGHT, "Движение вправо"},
            {'m', CmdType::SWITCH_MODE, "Смена режима"},
            {'M', CmdType::SWITCH_MODE, "Смена режима"},
            {'1', CmdType::USE_SPELL_1, "Заклинание 1"},
            {'2', CmdType::USE_SPELL_2, "Заклинание 2"},
            {'5', CmdType::SAVE_GAME, "Сохранить"},
            {'6', CmdType::LOAD_GAME, "Загрузить"},
            {'q', CmdType::QUIT_GAME, "Меню"},
            {'Q', CmdType::QUIT_GAME, "Меню"}
        };
        
        // Game Over
        mappings[GameStatus::GAME_OVER] = {
            {'1', CmdType::MENU_SELECT_1, "Повторить"},
            {'2', CmdType::MENU_SELECT_2, "Меню"}
        };
        
        // Уровень пройден
        mappings[GameStatus::LEVEL_COMPLETE] = {
            {'1', CmdType::MENU_SELECT_1, "Продолжить"},
            {'2', CmdType::MENU_SELECT_2, "Сохранить и выйти"}
        };
    }
    
public:
    CommandParser() {
        initMappings();
    }
    
    Command parse(GameStatus status, char input) const {
        auto it = mappings.find(status);
        if (it != mappings.end()) {
            for (const auto& mapping : it->second) {
                if (mapping.key == input) {
                    return Command(mapping.type, mapping.description);
                }
            }
        }
        return Command(CmdType::UNKNOWN, "Неизвестная команда");
    }
};

#endif