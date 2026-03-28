#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "Player.h"
#include "SpellHand.h"
#include "EnhancementBuffer.h"
#include "GameEvent.h"
#include "KeyBindingConfig.h"
#include <memory>
#include <functional>

class Game;
class GameWorld;

template<typename TRenderer>
class GameVisualizer : public IGameEventListener {
private:
    TRenderer renderer;
    const Game* currentGame;
    bool needsRedraw;
    
public:
    GameVisualizer() : currentGame(nullptr), needsRedraw(true) {}
    
    void setGame(const Game* game) {
        currentGame = game;
        needsRedraw = true;
    }
    
    void render() {
        if (currentGame && needsRedraw) {
            renderer.renderGame(*currentGame);
            needsRedraw = false;
        }
    }
    
    void forceRender() {
        if (currentGame) {
            renderer.renderGame(*currentGame);
            needsRedraw = false;
        }
    }
    
    void renderMessage(const std::string& message) {
        renderer.renderMessage(message);
    }
    
    void renderGameOver(int score, int turns) {
        renderer.renderGameOver(score, turns);
    }
    
    void renderVictory(int score, int turns) {
        renderer.renderVictory(score, turns);
    }
    
    void renderWelcome() {
        renderer.renderWelcome();
    }
    
    void onEvent(const GameEvent& event) override {
        needsRedraw = true;
        renderer.onGameEvent(event);
    }
    
    TRenderer& getRenderer() { return renderer; }
    const TRenderer& getRenderer() const { return renderer; }
};

class ConsoleRenderer {
public:
    void renderGame(const Game& game);
    void renderMessage(const std::string& message) const;
    void renderGameOver(int score, int turns) const;
    void renderVictory(int score, int turns) const;
    void renderWelcome() const;
    void renderControls() const;
    void onGameEvent(const GameEvent& event);
    
private:
    void renderField(const Game& game) const;
    void renderStatus(const Game& game) const;
    void renderSpells(const Game& game) const;
};

#endif
