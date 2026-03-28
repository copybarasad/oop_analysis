#include "validator.hpp"

void Validator::CheckRange(const std::string& name, int value, int min, int max) {
    if (value < min || value > max)
        throw ValidationError(name + " = " + std::to_string(value) +
                              " вне диапазона [" + std::to_string(min) +
                              ", " + std::to_string(max) + "]");
}

void Validator::CheckCoord(const std::string& name, int value, int limit) {
    if (value < 0 || value >= limit)
        throw ValidationError(name + " = " + std::to_string(value) +
                              " вне координат [0.." +
                              std::to_string(limit - 1) + "]");
}

void Validator::CheckFlag(const std::string& name, int v) {
    if (v != 0 && v != 1)
        throw ValidationError(name + " должен быть 0 или 1");
}

void Validator::CheckCellType(int t) {
    if (t < 0 || t > 10)
        throw ValidationError("CellType = " + std::to_string(t) +
                              " вне диапазона 0..10");
}