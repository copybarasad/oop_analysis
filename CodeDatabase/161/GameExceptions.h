// Exceptions/GameExceptions.h
#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include "BaseException.h"

class GameException : public BaseException {
public:
	using BaseException::BaseException;
};

// Исключения для файловых операций
class FileException : public BaseException {
public:
	using BaseException::BaseException;
};

class FileNotFoundError : public FileException {
public:
	explicit FileNotFoundError(const std::string& filename)
		: FileException("File not found: " + filename) {}
};

class FileOpenError : public FileException {
public:
	explicit FileOpenError(const std::string& filename)
		: FileException("Cannot open file: " + filename) {}
};

class FileWriteError : public FileException {
public:
	explicit FileWriteError(const std::string& filename)
		: FileException("Cannot write to file: " + filename) {}
};

// Исключения для загрузки данных
class LoadException : public BaseException {
public:
	using BaseException::BaseException;
};

class InvalidDataError : public LoadException {
public:
	explicit InvalidDataError(const std::string& details)
		: LoadException("Invalid data format: " + details) {}
};

class CorruptedSaveError : public LoadException {
public:
	explicit CorruptedSaveError(const std::string& filename)
		: LoadException("Corrupted save file: " + filename) {}
};

// Исключения для сохранения
class SaveException : public BaseException {
public:
	using BaseException::BaseException;
};

class SerializationError : public SaveException {
public:
	explicit SerializationError(const std::string& details)
		: SaveException("Serialization error: " + details) {}
};

#endif //GAMEEXCEPTIONS_H