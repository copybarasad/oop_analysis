#include "CommandReader.h"
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

CommandReader::CommandReader(std::istream& input, std::ostream& output)
	: inputStream(&input), outputStream(&output) {
}

std::unique_ptr<Command> CommandReader::readCommand() {
	std::string input = readInputLine();
	
	if (input.empty()) {
		return nullptr;
	}
	
	// Приводим к нижнему регистру для обработки
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);
	
	// Убираем пробелы
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
	
	if (!isValidInput(input)) {
		if (outputStream) {
			*outputStream << "Invalid command: " << input << std::endl;
		}
		return nullptr;
	}
	
	// Создаем команду на основе ввода
	// Пока возвращаем nullptr, так как конкретные классы команд будут добавлены позже
	// Базовая структура готова для расширения
	return nullptr;
}

std::string CommandReader::readInputLine(const std::string& prompt) {
	if (outputStream && !prompt.empty()) {
		*outputStream << prompt;
		outputStream->flush();
	}
	
	std::string line;
	if (inputStream && std::getline(*inputStream, line)) {
		return line;
	}
	
	return "";
}

int CommandReader::readInteger(const std::string& prompt, int minValue, int maxValue) {
	int value = 0;
	
	while (true) {
		if (outputStream && !prompt.empty()) {
			*outputStream << prompt;
			outputStream->flush();
		}
		
		if (inputStream && (*inputStream >> value)) {
			// Проверяем диапазон
			if (value >= minValue && value <= maxValue) {
				clearInput(); // Очищаем остаток строки
				return value;
			} else {
				if (outputStream) {
					*outputStream << "Value must be between " << minValue 
					              << " and " << maxValue << ". Try again." << std::endl;
				}
			}
		} else {
			if (outputStream) {
				*outputStream << "Invalid input. Please enter an integer." << std::endl;
			}
			clearInput();
		}
	}
}

void CommandReader::clearInput() {
	if (inputStream) {
		inputStream->clear();
		inputStream->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

bool CommandReader::isValidInput(const std::string& input) const {
	if (input.empty()) {
		return false;
	}
	
	// Проверяем, что это одна из допустимых команд
	// w, a, s, d - движение
	// f - атака
	// c - заклинание
	// h - показать руку
	// q - переключить режим боя
	// v - сохранить
	// z - загрузить
	// x - выход
	
	const std::string validCommands = "wasdfchqvxz";
	
	if (input.length() == 1) {
		return validCommands.find(input[0]) != std::string::npos;
	}
	
	return false;
}

