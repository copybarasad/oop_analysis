#ifndef COMMAND_H
#define COMMAND_H

#include "world.h"

// Абстрактный базовый класс команды
class Command {
public:
    virtual ~Command() = default;
    
    // Метод выполнения команды над миром
    // Возвращает true, если действие совершено (тратит ход), false если это было системное действие (сейв) или ошибка
    virtual bool Execute(World& world) = 0;
    
    // Нужно ли завершать игру после этой команды
    virtual bool IsQuit() const { return false; }
};

#endif