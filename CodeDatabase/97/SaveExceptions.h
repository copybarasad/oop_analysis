#ifndef UNTITLED_SAVEEXCEPTIONS_H
#define UNTITLED_SAVEEXCEPTIONS_H

#include <stdexcept>
#include <string>

class SaveError : public std::runtime_error {
public:
    explicit SaveError(const std::string &msg) : std::runtime_error(msg) {}
};

class LoadError : public std::runtime_error {
public:
    explicit LoadError(const std::string &msg) : std::runtime_error(msg) {}
};

class FileError : public LoadError {
public:
    explicit FileError(const std::string &msg) : LoadError(msg) {}
};

class ParseError : public LoadError {
public:
    explicit ParseError(const std::string &msg) : LoadError(msg) {}
};

#endif //UNTITLED_SAVEEXCEPTIONS_H
