#pragma once

#include <memory>

template<typename TRenderer>
class RenderingSystem {
private:
    std::unique_ptr<TRenderer> renderer;

public:
    template<typename... Args>
    RenderingSystem(Args&&... args) 
        : renderer(std::make_unique<TRenderer>(std::forward<Args>(args)...)) {}

    void updateReferences(Field* field, Player* player) {
        if (renderer) {
            renderer->updateReferences(field, player);
        }
    }

    void draw() {
        if (renderer) {
            renderer->render();
        }
    }

    void clearScreen() {
        if (renderer) {
            renderer->clearScreen();
        }
    }

    void moveCursor(int x, int y) {
        if (renderer) {
            renderer->moveCursor(x, y);
        }
    }

    TRenderer* getRenderer() {
        return renderer.get();
    }
};
