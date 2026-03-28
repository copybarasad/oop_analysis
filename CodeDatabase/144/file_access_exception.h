#ifndef FILE_ACCESS_EXCEPTION_H
#define FILE_ACCESS_EXCEPTION_H

#include <stdexcept>
#include <string>

class FileAccessException : public std::runtime_error {
public:
	FileAccessException(const std::string& path, const std::string& operation)
		: std::runtime_error("FileAccessException: Failed to " + operation + " file: " + path) {}
};

#endif





