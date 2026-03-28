#ifndef IINPUTREADER_H
#define IINPUTREADER_H

#include <string>

/**
 * @file IInputReader.h
 * @brief Интерфейс для считывания пользовательского ввода
 */

/**
 * @interface IInputReader
 * @brief Абстрактный интерфейс для различных источников ввода
 * 
 * Позволяет реализовать:
 * - Консольный ввод
 * - Сетевой ввод
 * - Файловый ввод (replay)
 * - GUI ввод
 */
class IInputReader {
public:
    virtual ~IInputReader() = default;
    
    /**
     * @brief Считать строку ввода от пользователя
     * @return Строка с командой
     */
    virtual std::string readInput() = 0;
    
    /**
     * @brief Проверить доступность ввода
     * @return true если можно читать ввод
     */
    virtual bool isAvailable() const = 0;
};

#endif // IINPUTREADER_H


