#include "SaveManager.hpp"
#include <stdexcept>

SaveManager::SaveManager(const std::string& filename)
	: filename(filename), isReadMode(false) {
	openForWrite();
}

SaveManager::SaveManager(const std::string& filename, bool readMode)
	: filename(filename), isReadMode(readMode) {
	if (readMode) {
		openForRead();
	} else {
		openForWrite();
	}
}

SaveManager::SaveManager(SaveManager&& other) noexcept
	: file(std::move(other.file)), 
	  filename(std::move(other.filename)),
	  isReadMode(other.isReadMode) {
	other.isReadMode = false;
}

SaveManager& SaveManager::operator=(SaveManager&& other) noexcept {
	if (this != &other) {
		if (file.is_open()) {
			file.close();
		}
		file = std::move(other.file);
		filename = std::move(other.filename);
		isReadMode = other.isReadMode;
		other.isReadMode = false;
	}
	return *this;
}

SaveManager::~SaveManager() {
	if (file.is_open()) {
		file.close();
	}
}

void SaveManager::openForWrite() {
	file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open() || !file.good()) {
		throw FileException("Не удалось открыть файл для записи: " + filename, "SaveManager::openForWrite");
	}
}

void SaveManager::openForRead() {
	file.open(filename, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		throw FileException("Файл не существует: " + filename, "SaveManager::openForRead");
	}
	if (!file.good()) {
		throw FileException("Не удалось открыть файл для чтения: " + filename, "SaveManager::openForRead");
	}
}

bool SaveManager::isOpen() const noexcept {
	return file.is_open();
}

bool SaveManager::good() const noexcept {
	return file.good();
}

bool SaveManager::eof() const noexcept {
	return file.eof();
}

void SaveManager::close() {
	if (file.is_open()) {
		file.close();
	}
}

void SaveManager::writeString(const std::string& str) {
	if (!file.is_open() || !file.good()) {
		throw SaveException("Не удалось записать строку в файл", "SaveManager::writeString");
	}
	size_t len = str.length();
	file.write(reinterpret_cast<const char*>(&len), sizeof(len));
	file.write(str.c_str(), len);
	if (file.fail()) {
		throw SaveException("Ошибка записи строки", "SaveManager::writeString");
	}
}

std::string SaveManager::readString() {
	if (!file.is_open() || !file.good()) {
		throw LoadException("Не удалось прочитать строку из файла", "SaveManager::readString");
	}
	size_t len = 0;
	file.read(reinterpret_cast<char*>(&len), sizeof(len));
	if (file.fail() || file.eof() || len > 10000) {
		throw DataException("Некорректная длина строки в файле", "SaveManager::readString");
	}
	std::string str(len, '\0');
	file.read(&str[0], len);
	if (file.fail() || file.eof()) {
		throw DataException("Не удалось прочитать строку из файла", "SaveManager::readString");
	}
	return str;
}

