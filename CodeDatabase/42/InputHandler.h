#pragma once
#include <string>

class InputHandler {
public:
    static int getValidatedIntegerInput(const std::string& prompt, int minValue, int maxValue);
    static char getGameInput();
    static void clearInputBuffer();
    static void waitForEnter();

private:
    static const int MAX_COMMAND_LENGTH = 1;
    static bool isValidCommandCharacter(char c);
};