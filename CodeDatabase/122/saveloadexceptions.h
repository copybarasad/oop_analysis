#ifndef SAVELOADEXCEPTIONS_H
#define SAVELOADEXCEPTIONS_H

#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error
{
public:
    explicit SaveLoadException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class SaveException : public SaveLoadException
{
public:
    explicit SaveException(const std::string& msg)
        : SaveLoadException("Save error: " + msg) {}
};

class LoadException : public SaveLoadException
{
public:
    explicit LoadException(const std::string& msg)
        : SaveLoadException("Load error: " + msg) {}
};

class FileNotFoundException : public SaveLoadException
{
public:
    explicit FileNotFoundException(const std::string& path)
        : SaveLoadException("File not found: " + path) {}
};

class InvalidDataException : public SaveLoadException
{
public:
    explicit InvalidDataException(const std::string& msg)
        : SaveLoadException("Invalid data: " + msg) {}
};


#endif // SAVELOADEXCEPTIONS_H
