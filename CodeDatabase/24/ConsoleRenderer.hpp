#pragma once
#include "Command.hpp"
#include "Field.hpp"
#include "CellType.hpp"
#include "SaveManager.hpp"

// Класс для отрисовки игры в консоли для использования в шаблонном GameVisualizer
class ConsoleRenderer {
public:
    void render(const Field& field);
    void renderStats(const Player& player);
    void renderMainMenu();
    void renderPauseMenu();
    void renderLoadMenu(const std::vector<SaveInfo>& saves);
    void renderGameOver(bool victory, int score);
    
private:
    // Получение символа для типа клетки
    char getCellSymbol(CellType type) const;
};
