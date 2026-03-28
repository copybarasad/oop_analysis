#ifndef RENDERING_MANAGER_H
#define RENDERING_MANAGER_H

#include <string>
#include <memory>

class Game;
class GameRenderer;

// Базовый класс для управления отрисовкой (интерфейс)
class RenderingManager {
public:
    virtual ~RenderingManager() = default;

    // Методы отрисовки различных элементов игры
    virtual void renderField() = 0;
    virtual void renderPlayerStatus() = 0;
    virtual void renderSpellList() = 0;
    virtual void renderMessage(const std::string& message) = 0;
    virtual void renderControls() = 0;
    virtual void update() = 0;

    // Дополнительные методы отображения для совместимости
    virtual void displayField() const {}
    virtual void displayPlayerStatus() const {}
    virtual void displaySpells() const {}
    virtual void displayGameStatus() const {}
    virtual void displayGameOver() const {}
};

#endif