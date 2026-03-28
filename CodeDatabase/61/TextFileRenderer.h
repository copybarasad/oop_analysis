#ifndef TEXT_FILE_RENDERER_H
#define TEXT_FILE_RENDERER_H

#include "GameRenderer.h"
#include <fstream>
#include <string>

// Простой класс отрисовки в текстовый файл
// Наследуется от GameRenderer и перенаправляет вывод в файл
class TextFileRenderer : public GameRenderer {
private:
	std::ofstream fileStream;
	std::string filename;
	
public:
	// Конструктор - создает файл для записи
	TextFileRenderer(const std::string& outputFilename = "game_output.txt");
	
	// Деструктор - закрывает файл
	~TextFileRenderer();
	
	// Получение имени файла
	const std::string& getFilename() const;
	
	// Закрытие файла (можно вызвать вручную)
	void close();
	
	// Открытие нового файла
	void open(const std::string& newFilename);
	
	// Принудительный сброс буфера
	void flush();
};

#endif // TEXT_FILE_RENDERER_H

