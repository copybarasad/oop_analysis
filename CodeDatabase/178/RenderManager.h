#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "IRenderStrategy.h"
#include <memory>

template<typename RenderStrategy>
class RenderManager {
private:
    std::unique_ptr<RenderStrategy> renderStrategy;
    
public:
    RenderManager(std::unique_ptr<RenderStrategy> strategy)
        : renderStrategy(std::move(strategy)) {}
    
    template<typename... Args>
    RenderManager(Args&&... args) {
        renderStrategy = std::make_unique<RenderStrategy>(std::forward<Args>(args)...);
    }
    
    void renderGame(const GameController& controller) {
        renderStrategy->renderGame(controller);
    }
    
    void renderMenu(const std::string& menuText) {
        renderStrategy->renderMenu(menuText);
    }
    
    void renderMessage(const std::string& message) {
        renderStrategy->renderMessage(message);
    }
    
    void clearScreen() {
        renderStrategy->clearScreen();
    }
    
    void setStrategy(std::unique_ptr<RenderStrategy> newStrategy) {
        renderStrategy = std::move(newStrategy);
    }
    
    RenderStrategy* getStrategy() {
        return renderStrategy.get();
    }
};
#endif