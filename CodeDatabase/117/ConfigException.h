#ifndef CONFIGEXCEPTION_H
#define CONFIGEXCEPTION_H

#include <stdexcept>
#include <string>

class ConfigFileException : public std::runtime_error {
public:
    ConfigFileException(const std::string& message) : std::runtime_error("Keyboard configuration error: " + message + 
        ". Default configuration will be set") {}
};

class NoKeyInConfigException : public std::runtime_error {
public:
    NoKeyInConfigException() : std::runtime_error("Key not found in config"){}
};

#endif