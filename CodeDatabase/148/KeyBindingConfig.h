#ifndef KEYBINDINGCONFIG_H
#define KEYBINDINGCONFIG_H

#include <string>
#include <unordered_map>

/**
 * @file KeyBindingConfig.h
 * @brief Конфигурация привязки клавиш к командам
 */

/**
 * @enum CommandType
 * @brief Типы игровых команд
 */
enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    CAST_SPELL,
    SWITCH_MODE,
    SHOW_INFO,
    SHOW_HELP,
    QUIT,
    SAVE_GAME,
    LOAD_GAME
};

/**
 * @class KeyBindingConfig
 * @brief Управление привязками клавиш
 * 
 * Загружает конфигурацию из файла, валидирует её,
 * и предоставляет маппинг клавиша → команда.
 */
class KeyBindingConfig {
private:
    std::unordered_map<std::string, CommandType> keyToCommand;  ///< Клавиша → Команда
    std::unordered_map<CommandType, std::string> commandToKey;  ///< Команда → Клавиша
    
    bool isValid;  ///< Флаг валидности конфигурации
    
    /**
     * @brief Загрузить дефолтные настройки
     */
    void loadDefaults();
    
    /**
     * @brief Валидировать конфигурацию
     * @return true если конфигурация корректна
     * 
     * Проверяет:
     * - Все команды присутствуют
     * - Нет дублей клавиш
     * - Нет дублей команд
     */
    bool validate() const;
    
public:
    /**
     * @brief Конструктор с дефолтными настройками
     */
    KeyBindingConfig();
    
    /**
     * @brief Загрузить конфигурацию из файла
     * @param filepath Путь к файлу конфигурации
     * @return true если загрузка успешна
     * 
     * Если загрузка не удалась или конфигурация невалидна,
     * устанавливаются дефолтные настройки.
     */
    bool loadFromFile(const std::string& filepath);
    
    /**
     * @brief Получить тип команды по клавише
     * @param key Клавиша (строка, например "W" или "SAVE")
     * @return Тип команды или nullptr если клавиша не привязана
     */
    const CommandType* getCommand(const std::string& key) const;
    
    /**
     * @brief Получить клавишу по команде
     * @param command Тип команды
     * @return Строка с клавишей
     */
    std::string getKey(CommandType command) const;
    
    /**
     * @brief Проверить валидность конфигурации
     * @return true если конфигурация корректна
     */
    bool isConfigValid() const { return isValid; }
    
    /**
     * @brief Вывести все привязки (debug)
     */
    void printBindings() const;
};

#endif // KEYBINDINGCONFIG_H


