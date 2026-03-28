#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "world.h"
#include "key_config.h" // Подключаем конфиг

class ConsoleRenderer {
public:
    ConsoleRenderer(); // Нужен конструктор
    void Render(const World& world);

private:
    void PrintMap(const Field& field);
    void PrintStats(const World& world);
    void PrintControls(); // Новый метод

    KeyConfig key_config_; // Экземпляр конфига для чтения настроек
};

#endif