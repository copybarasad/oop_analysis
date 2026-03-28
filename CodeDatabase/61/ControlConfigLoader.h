#ifndef CONTROL_CONFIG_LOADER_H
#define CONTROL_CONFIG_LOADER_H

#include "ControlConfig.h"
#include <string>
#include <fstream>

// Класс для загрузки конфигурации управления из файла
class ControlConfigLoader {
private:
	// Парсинг строки конфигурации (формат: "COMMAND=key" или "key=COMMAND")
	static bool parseLine(const std::string& line, std::string& commandStr, char& key);
	
	// Очистка строки от пробелов и комментариев
	static std::string cleanLine(const std::string& line);
	
public:
	// Загрузка конфигурации из файла
	// Если файл не найден или конфигурация невалидна, возвращает конфигурацию по умолчанию
	static ControlConfig loadFromFile(const std::string& filename);
	
	// Сохранение конфигурации в файл (для создания примера)
	static bool saveToFile(const ControlConfig& config, const std::string& filename);
};

#endif // CONTROL_CONFIG_LOADER_H

