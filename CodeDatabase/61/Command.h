#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// Базовый класс для всех команд игры
class Command {
public:
	Command();
	virtual ~Command() = default;
	
	// Выполнение команды
	virtual void execute() = 0;
	
	// Получение типа команды
	virtual std::string getType() const = 0;
	
	// Проверка валидности команды
	virtual bool isValid() const;

protected:
	bool valid;
};

#endif // COMMAND_H

