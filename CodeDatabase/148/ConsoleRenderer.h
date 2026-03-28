#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "IRenderer.h"
#include <string>

/**
 * @file ConsoleRenderer.h
 * @brief Отрисовка игры в консоли (убраны рамки, добавлены расстояния)
 */

/**
 * @class ConsoleRenderer
 * @brief Реализация консольной отрисовки
 */
class ConsoleRenderer : public IRenderer {
public:
    void renderGame(const Game* game) override;
    void renderField(const Game* game) override;
    void renderPlayerStatus(const Player* player) override;
    void renderSpellHand(const SpellHand* spellHand) override;
    void showMessage(const std::string& message) override;
    void showError(const std::string& error) override;

    // Дополнительные методы
    void printWelcome() const;
    void printVictory(const Player* player) const;
    void printGameOver() const;

    // Новые методы
    void renderEnemyDistances(const Game* game);
    void renderCompactLegend() const;
};

#endif // CONSOLERENDERER_H


