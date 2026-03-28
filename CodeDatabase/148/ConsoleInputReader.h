#ifndef CONSOLEINPUTREADER_H
#define CONSOLEINPUTREADER_H

#include "IInputReader.h"
#include <iostream>
#include <string>

/**
 * @file ConsoleInputReader.h
 * @brief Считывание ввода из консоли
 */

/**
 * @class ConsoleInputReader
 * @brief Реализация считывания из стандартного ввода
 */
class ConsoleInputReader : public IInputReader {
public:
    /**
     * @brief Считать строку из консоли
     * @return Строка с командой пользователя
     */
    std::string readInput() override {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        return input;
    }
    
    /**
     * @brief Проверить доступность консольного ввода
     * @return true если cin в рабочем состоянии
     */
    bool isAvailable() const override {
        return std::cin.good();
    }
};

#endif // CONSOLEINPUTREADER_H
#ifndef CONSOLEINPUTREADER_H
#define CONSOLEINPUTREADER_H

#include "IInputReader.h"
#include <iostream>
#include <string>

/**
 * @file ConsoleInputReader.h
 * @brief Считывание ввода из консоли
 */

/**
 * @class ConsoleInputReader
 * @brief Реализация считывания из стандартного ввода
 */
class ConsoleInputReader : public IInputReader {
public:
    /**
     * @brief Считать строку из консоли
     * @return Строка с командой пользователя
     */
    std::string readInput() override {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        return input;
    }
    
    /**
     * @brief Проверить доступность консольного ввода
     * @return true если cin в рабочем состоянии
     */
    bool isAvailable() const override {
        return std::cin.good();
    }
};

#endif // CONSOLEINPUTREADER_H


