#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Event.h"

// Структура для хранения конфигурации одной команды
struct CommandConfig {
    EventType eventType;
    std::vector<float> floatArgs;
    std::vector<int> intArgs;
    std::vector<std::string> stringArgs;
};

// Класс для хранения конфигурации управления
class InputConfig {
private:
    // Маппинг клавиши -> команда
    std::map<sf::Keyboard::Scancode, CommandConfig> keyToCommand_;
    // Маппинг команды -> клавиша (для проверки дубликатов)
    std::map<EventType, sf::Keyboard::Scancode> commandToKey_;
    
    // Значения по умолчанию
    static std::map<EventType, sf::Keyboard::Scancode> getDefaultConfig();
    
public:
    InputConfig();
    
    // Загружает конфигурацию из файла, возвращает true если успешно
    bool loadFromFile(const std::string& filename);
    
    // Устанавливает конфигурацию по умолчанию
    void setDefault();
    
    // Получает команду для клавиши, возвращает nullptr если команды нет
    const CommandConfig* getCommand(sf::Keyboard::Scancode key) const;
    
    // Проверяет корректность конфигурации
    bool isValid() const;
};

