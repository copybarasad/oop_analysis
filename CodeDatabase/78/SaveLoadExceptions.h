#ifndef SAVELOADEXCEPTIONS_H
#define SAVELOADEXCEPTIONS_H

#include <stdexcept>
#include <string>

class SaveGameException : public std::runtime_error {
public:
    SaveGameException(const std::string& details)
        : std::runtime_error("Save failed: " + details) {}
};

class LoadGameException : public std::runtime_error {
public:
    LoadGameException(const std::string& details)
        : std::runtime_error("Load failed: " + details) {}
};

#endif