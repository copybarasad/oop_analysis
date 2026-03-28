#ifndef OOP_BASE_COMMAND_H
#define OOP_BASE_COMMAND_H
#include <string>


// базовый класс - интерфейс для других команд
// по тз - ввод преобразуется в объект команды

class Command {
public:
    virtual ~Command() = default;
    virtual std::string get_name() = 0;
};

#endif //OOP_BASE_COMMAND_H