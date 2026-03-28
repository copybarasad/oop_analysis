#ifndef COMMAND_MAPPER_H
#define COMMAND_MAPPER_H

#include "control_config.h"
#include <string>
#include <memory>
#include <iostream>

class CommandMapper {
private:
    std::unique_ptr<ControlConfig> config;
    
public:
    CommandMapper() : config(std::make_unique<ControlConfig>()) {
        config->setDefaultConfig();
    }
    
    bool initialize(const std::string& configFile = "controls.config") {
        return config->loadFromFile(configFile);
    }
    
    std::string mapInputToCommand(const std::string& input) const {
        return config->getCommandForKey(input);
    }
    
    std::string getKeyForCommand(const std::string& command) const {
        return config->getKeyForCommand(command);
    }
    
    void displayMappings() const {
        std::cout << "\n=== НАСТРОЙКИ УПРАВЛЕНИЯ ===" << std::endl;
        std::cout << "w / UP: Вверх" << std::endl;
        std::cout << "s / DOWN: Вниз" << std::endl;
        std::cout << "a / LEFT: Влево" << std::endl;
        std::cout << "d / RIGHT: Вправо" << std::endl;
        std::cout << "k / ATTACK: Атака" << std::endl;
        std::cout << "t / SWITCH_COMBAT: Смена режима боя" << std::endl;
        std::cout << "c / CAST_SPELL: Применить заклинание" << std::endl;
        std::cout << "b / BUY_SPELL: Купить заклинание" << std::endl;
        std::cout << "save / SAVE: Сохранить игру" << std::endl;
        std::cout << "load / LOAD: Загрузить игру" << std::endl;
        std::cout << "status / SHOW_STATUS: Показать статус" << std::endl;
        std::cout << "q / QUIT: Выйти" << std::endl;
        std::cout << "===========================" << std::endl;
    }
};

#endif // COMMAND_MAPPER_H