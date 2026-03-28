#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include "Command.h"
#include <memory>
#include <istream>
#include <ostream>
#include <iostream>
#include <limits>
#include <string>

// Класс для считывания ввода пользователя и преобразования в объект команды
class CommandReader {
private:
	std::istream* inputStream;
	std::ostream* outputStream;
	
	// Валидация ввода
	bool isValidInput(const std::string& input) const;
	
public:
	CommandReader(std::istream& input = std::cin, std::ostream& output = std::cout);
	
	// Считывает ввод пользователя и преобразует в команду
	std::unique_ptr<Command> readCommand();
	
	// Получает строку ввода от пользователя
	std::string readInputLine(const std::string& prompt = "");
	
	// Получает число от пользователя
	int readInteger(const std::string& prompt = "", int minValue = std::numeric_limits<int>::min(), 
	                int maxValue = std::numeric_limits<int>::max());
	
	// Очистка потока ввода
	void clearInput();
};

#endif // COMMAND_READER_H

