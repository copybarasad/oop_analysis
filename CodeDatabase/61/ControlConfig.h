#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

#include <string>
#include <map>
#include <unordered_set>

// Класс для хранения конфигурации управления игрой
// Хранит маппинг клавиш на команды
class ControlConfig {
public:
	// Типы команд
	enum class CommandType {
		MOVE_UP,      // Движение вверх
		MOVE_DOWN,    // Движение вниз
		MOVE_LEFT,    // Движение влево
		MOVE_RIGHT,   // Движение вправо
		ATTACK,       // Атака
		CAST_SPELL,   // Применение заклинания
		SHOW_HAND,    // Показать руку (заклинания)
		SWITCH_MODE,  // Переключить режим боя
		SAVE,         // Сохранить игру
		LOAD,         // Загрузить игру
		EXIT          // Выход из игры
	};
	
private:
	// Маппинг: клавиша -> тип команды
	std::map<char, CommandType> keyToCommand;
	
	// Маппинг: тип команды -> клавиша (для обратного поиска)
	std::map<CommandType, char> commandToKey;
	
	// Валидность конфигурации
	bool isValid;
	
	// Сообщение об ошибке (если конфигурация невалидна)
	std::string errorMessage;
	
public:
	// Инициализация значений по умолчанию
	void initializeDefaults();
	// Конструктор - создает конфигурацию по умолчанию
	ControlConfig();
	
	// Установка маппинга клавиши на команду
	void setKeyForCommand(char key, CommandType command);
	
	// Получение команды по клавише
	CommandType getCommandForKey(char key) const;
	
	// Получение клавиши для команды
	char getKeyForCommand(CommandType command) const;
	
	// Проверка, назначена ли клавиша какой-либо команде
	bool isKeyMapped(char key) const;
	
	// Проверка, назначена ли команда какой-либо клавише
	bool isCommandMapped(CommandType command) const;
	
	// Валидация конфигурации
	bool validate();
	
	// Получение статуса валидности
	bool getIsValid() const;
	
	// Получение сообщения об ошибке
	const std::string& getErrorMessage() const;
	
	// Получение всех обязательных команд
	static std::unordered_set<CommandType> getRequiredCommands();
	
	// Преобразование CommandType в строку (для отладки)
	static std::string commandTypeToString(CommandType type);
	
	// Преобразование строки в CommandType
	static CommandType stringToCommandType(const std::string& str);
};

#endif // CONTROL_CONFIG_H

