// Exceptions/BaseException.h
#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <exception>
#include <string>

class BaseException : public std::exception {
public:
	explicit BaseException(const std::string& message) : message_(message) {}

	const char* what() const noexcept override {
		return message_.c_str();
	}

private:
	std::string message_;
};

#endif //BASEEXCEPTION_H