#ifndef VALIFATOR_HPP
#define VALIFATOR_HPP
#include <stdexcept>
#include <string>

class ValidationError : public std::runtime_error {
public:
    explicit ValidationError(const std::string& msg)
        : std::runtime_error(msg) {}
};

class Validator {
public:
    static void CheckRange(const std::string& name, int value, int min, int max);
    static void CheckCoord(const std::string& name, int value, int limit);
    static void CheckFlag(const std::string& name, int v);
    static void CheckCellType(int t);
};

#endif