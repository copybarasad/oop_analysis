#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "../Controllers/GameState.h"
#include <map>

template <class RendererType>
class GameVisualizer {
private:
    RendererType renderer;
    GameState& gameState;
    
public:
    GameVisualizer(GameState& gameState) : gameState(gameState) {}
    
    void render() {
        renderer.renderGameState(this->gameState);
    }
    
    void renderShop() {
        renderer.renderShop(this->gameState.getPlayer());
    }

    void renderKeyboardSettings(std::map<char, ActionType> keyboardSettings) {
        renderer.renderKeyboardSettings(keyboardSettings);
    }
    
    void renderField() {
        renderer.renderField(this->gameState);
    }
    
    void renderPlayerInfo() {
        renderer.renderPlayerInfo(this->gameState.getPlayer());
        this->renderHand();
    }
    
    void renderHand() {
        renderer.renderHand(this->gameState.getHand());
    }
    
    void renderMainMenu() {
        renderer.renderMainMenu();
    }
    
    void renderLevelComplete() {
        renderer.renderLevelComplete();
    }
    
    void renderUpgrade() {
        renderer.renderUpgrade();
    }

    void renderGameOver() {
        renderer.renderGameOver();
    }
    
    void renderMessage(const std::string& message) {
        renderer.renderMessage(message);
    }

    void renderErrorMessage(const std::string& message) {
        renderer.renderErrorMessage(message);
    }
};

#endif