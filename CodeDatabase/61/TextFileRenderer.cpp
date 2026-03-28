#include "TextFileRenderer.h"
#include <iostream>

TextFileRenderer::TextFileRenderer(const std::string& outputFilename)
	: GameRenderer(fileStream), filename(outputFilename) {
	// Открываем файл после инициализации базового класса
	fileStream.open(filename, std::ios::out | std::ios::trunc);
	if (!fileStream.is_open()) {
		std::cerr << "ERROR: Failed to open file " << filename << " for writing." << std::endl;
		std::cerr << "Check file permissions and disk space." << std::endl;
	} else {
		// Обновляем указатель в GameRenderer после открытия файла
		// (хотя это не обязательно, так как указатель уже указывает на fileStream)
		outputStream = &fileStream;
		std::cerr << "File " << filename << " opened successfully." << std::endl;
		// Принудительно сбрасываем буфер после открытия
		fileStream.flush();
	}
}

TextFileRenderer::~TextFileRenderer() {
	// Принудительно сбрасываем буфер перед закрытием
	if (fileStream.is_open()) {
		fileStream.flush();
	}
	close();
}

const std::string& TextFileRenderer::getFilename() const {
	return filename;
}

void TextFileRenderer::close() {
	if (fileStream.is_open()) {
		fileStream.flush();  // Принудительно сбрасываем буфер перед закрытием
		fileStream.close();
	}
}

void TextFileRenderer::open(const std::string& newFilename) {
	close();
	filename = newFilename;
	fileStream.open(filename, std::ios::out | std::ios::trunc);
	if (!fileStream.is_open()) {
		std::cerr << "Warning: Failed to open file " << filename << " for writing." << std::endl;
	}
}

void TextFileRenderer::flush() {
	if (fileStream.is_open()) {
		fileStream.flush();
	}
}

