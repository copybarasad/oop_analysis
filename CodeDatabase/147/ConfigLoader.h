#pragma once

#include <unordered_map>
#include <string>
#include <set>

// Загружает настройку управления из файла.
// Формат: одна команда на строку в виде KEY=ACTION, например:
// W=UP
// A=LEFT
// S=DOWN
// D=RIGHT
// M=TOGGLE_MODE
// F=RANGED_ATTACK
// C=CAST_SPELL
// B=BUY_SPELL
// L=LOAD
// V=SAVE
// Q=EXIT
class ConfigLoader {
public:
    std::unordered_map<char, std::string> loadBindings(const std::string& path) const;

private:
    bool validate(const std::unordered_map<char, std::string>& bindings) const;
};
