#pragma once
#include <stdexcept>
#include <string>

class GameException : public std::exception {
public:
	explicit GameException(const std::string& message, const std::string& location = "")
		: msg(message), loc(location) {}
	
	const char* what() const noexcept override {
		return msg.c_str();
	}
	
	const std::string& getLocation() const noexcept {
		return loc;
	}

protected:
	std::string msg;
	std::string loc;
};

class SaveException : public GameException {
public:
	explicit SaveException(const std::string& message, const std::string& location = "")
		: GameException("Ошибка сохранения: " + message, location) {}
};

class LoadException : public GameException {
public:
	explicit LoadException(const std::string& message, const std::string& location = "")
		: GameException("Ошибка загрузки: " + message, location) {}
};

class FileException : public GameException {
public:
	explicit FileException(const std::string& message, const std::string& location = "")
		: GameException("Ошибка файла: " + message, location) {}
};

class DataException : public GameException {
public:
	explicit DataException(const std::string& message, const std::string& location = "")
		: GameException("Ошибка данных: " + message, location) {}
};

