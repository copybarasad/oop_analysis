#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "Player.h"
#include "Game.h"
#include "SpellHand.h"
#include <string>

/**
 * @file GameRenderer.h
 * @brief Класс для отрисовки игрового состояния
 */

class GameRenderer {
private:
    static const std::string SEPARATOR;
    static const std::string HEADER_CHAR;
    
public:
    /**
     * @brief Конструктор
     */
    GameRenderer() = default;
    
    /**
     * @brief Вывести приветственное сообщение
     */
    void printWelcome() const;
    
    /**
     * @brief Вывести игровое поле
     * @param game Указатель на игру
     */
    void printField(const Game* game) const;
    
    /**
     * @brief Вывести статус игрока
     * @param player Указатель на игрока
     */
    void printPlayerStatus(const Player* player) const;
    
    /**
     * @brief Вывести содержимое руки заклинаний
     * @param spellHand Указатель на руку заклинаний
     */
    void printSpellHand(const SpellHand* spellHand) const;
    
    /**
     * @brief Вывести общую информацию об игре
     * @param game Указатель на игру
     */
    void printGameInfo(const Game* game) const;
    
    /**
     * @brief Вывести сообщение об успехе
     * @param message Сообщение
     */
    void printSuccess(const std::string& message) const;
    
    /**
     * @brief Вывести сообщение об ошибке
     * @param message Сообщение
     */
    void printError(const std::string& message) const;
    
    /**
     * @brief Вывести информационное сообщение
     * @param message Сообщение
     */
    void printInfo(const std::string& message) const;
    
    /**
     * @brief Вывести победное сообщение
     * @param player Указатель на игрока
     */
    void printVictory(const Player* player) const;
    
    /**
     * @brief Вывести сообщение о поражении
     */
    void printGameOver() const;
};

#endif // GAMERENDERER_H


