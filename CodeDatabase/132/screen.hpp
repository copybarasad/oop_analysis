#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "field.hpp"

class Screen {
private:
    static void clearConsole();
    static std::string getColorForSymbol(char symbol);
public:
    static void frame(const Field& field);
    // static void show_result();
};
