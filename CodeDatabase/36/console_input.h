#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include <memory>
#include "command.h"
#include "world.h"
#include "key_config.h" // Подключаем конфиг

class ConsoleInputHandler {
public:
    ConsoleInputHandler(); // Конструктор для загрузки конфига
    std::unique_ptr<Command> GetCommand(const World& context);

    int GetUpgradeChoice();
    int GetSaveLoadChoice();
    std::string GetFilename(bool save);

private:
    std::pair<int, int> GetTarget();
    int GetSpellIndex(const Player& player);
    
    KeyConfig key_config_; // Объект конфигурации
};

#endif