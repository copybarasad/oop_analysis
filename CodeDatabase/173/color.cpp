#include "color.hpp"

Color::Color(const std::string &code) : m_AnsiEscapeCode(code) {}
std::string Color::ansiEscapeCode() { return m_AnsiEscapeCode; }

Color Color::red() { return Color("\033[31m"); }
Color Color::green() { return Color("\033[32m"); }
Color Color::white() { return Color("\033[37m"); }

Color Color::lime() { return Color("\033[92m"); }
Color Color::magenta() { return Color("\033[95m"); }

Color Color::reset() { return Color("\033[0m"); }
