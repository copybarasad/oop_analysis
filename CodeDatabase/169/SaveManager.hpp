#pragma once
#include <fstream>
#include <string>
#include "GameException.hpp"

class SaveManager {
public:
	explicit SaveManager(const std::string& filename);
	
	SaveManager(const std::string& filename, bool readMode);
	
	SaveManager(const SaveManager&) = delete;
	SaveManager& operator=(const SaveManager&) = delete;
	
	SaveManager(SaveManager&& other) noexcept;
	SaveManager& operator=(SaveManager&& other) noexcept;
	
	~SaveManager();
	
	bool isOpen() const noexcept;
	bool good() const noexcept;
	
	template<typename T>
	SaveManager& operator<<(const T& value) {
		if (!file.is_open() || !file.good()) {
			throw SaveException("Не удалось записать в файл", "SaveManager::operator<<");
		}
		file.write(reinterpret_cast<const char*>(&value), sizeof(T));
		if (file.fail()) {
			throw SaveException("Ошибка записи данных", "SaveManager::operator<<");
		}
		return *this;
	}
	
	template<typename T>
	SaveManager& operator>>(T& value) {
		if (!file.is_open() || !file.good()) {
			throw LoadException("Не удалось прочитать из файла", "SaveManager::operator>>");
		}
		file.read(reinterpret_cast<char*>(&value), sizeof(T));
		if (file.fail() || file.eof()) {
			throw DataException("Некорректные данные в файле", "SaveManager::operator>>");
		}
		return *this;
	}
	
	void writeString(const std::string& str);
	std::string readString();
	
	bool eof() const noexcept;
	
	void close();

private:
	std::fstream file;
	std::string filename;
	bool isReadMode;
	
	void openForWrite();
	void openForRead();
};

