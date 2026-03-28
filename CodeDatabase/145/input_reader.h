#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "game_command.h"
#include "command.h"
#include <memory>
#include <string>
#include <map>

class InputReader {
public:
    virtual ~InputReader() = default;

    // Получить следующую команду
    virtual GameCommand getNextGameCommand() = 0;

    // Получить целевые координаты для заклинания
    virtual std::pair<int, int> getSpellTarget() = 0;

    // Получить ответ да/нет
    virtual bool getYesNoChoice(const std::string& prompt) = 0;

    // Получить имя файла
    virtual std::string getFilename(const std::string& prompt) = 0;

    // Получить отображение клавиш команд
    virtual std::map<char, std::string> getKeyMapping() const = 0;
};

#endif