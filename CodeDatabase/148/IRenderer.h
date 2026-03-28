#ifndef IRENDERER_H
#define IRENDERER_H

#include <string>

class Game;
class Player;
class SpellHand;

/**
 * @file IRenderer.h
 * @brief Интерфейс для различных способов отрисовки игры
 */

/**
 * @interface IRenderer
 * @brief Абстрактный интерфейс для различных рендереров
 * 
 */
class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    /**
     * @brief Отрисовать полное состояние игры
     * @param game Указатель на игру
     */
    virtual void renderGame(const Game* game) = 0;
    
    /**
     * @brief Отрисовать игровое поле
     * @param game Указатель на игру
     */
    virtual void renderField(const Game* game) = 0;
    
    /**
     * @brief Отрисовать статус игрока
     * @param player Указатель на игрока
     */
    virtual void renderPlayerStatus(const Player* player) = 0;
    
    /**
     * @brief Отрисовать руку заклинаний
     * @param spellHand Указатель на руку заклинаний
     */
    virtual void renderSpellHand(const SpellHand* spellHand) = 0;
    
    /**
     * @brief Показать сообщение
     * @param message Текст сообщения
     */
    virtual void showMessage(const std::string& message) = 0;
    
    /**
     * @brief Показать ошибку
     * @param error Текст ошибки
     */
    virtual void showError(const std::string& error) = 0;
};

#endif // IRENDERER_H


