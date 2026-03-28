#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
protected:
	std::string location;
	
public:
	GameException(const std::string& message, const std::string& loc);
	const char* what() const noexcept override;
	const std::string& getLocation() const;
	
private:
	mutable std::string full_message;
};

class SaveException : public GameException {
public:
	SaveException(const std::string& message, const std::string& loc);
};

class FileWriteException : public SaveException {
public:
	FileWriteException(const std::string& filename, const std::string& loc);
};

class LoadException : public GameException {
public:
	LoadException(const std::string& message, const std::string& loc);
};

class FileNotFoundException : public LoadException {
public:
	FileNotFoundException(const std::string& filename, const std::string& loc);
};

class CorruptedDataException : public LoadException {
public:
	CorruptedDataException(const std::string& details, const std::string& loc);
};

#endif
