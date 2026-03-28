#ifndef GAMEVIEW_H
#define GAMEVIEW_H


#include "Game.h"


/**
 * @file GameView.h
 * @brief Шаблонный класс визуализации игры
 */


template<typename TRenderer>
class GameView {
private:
    TRenderer renderer;
    const Game* game;
    
public:
    explicit GameView(const Game* gameInstance) 
        : game(gameInstance) {}
    
    /**
     * @brief Полная отрисовка игрового состояния (поле + легенда + враги + статус)
     * 
     * Используется в основном игровом цикле после каждого хода.
     */
    void render() {
        if (!game) return;
        renderer.renderField(game);
        renderer.renderCompactLegend();
        renderer.renderEnemyDistances(game);
        renderer.renderPlayerStatus(game->getPlayer());
    }
    
    void update() {
        if (!game) return;
        renderer.renderGame(game);
    }
    
    void renderField() {
        if (!game) return;
        renderer.renderField(game);
    }
    
    void renderPlayerStatus() {
        if (!game || !game->getPlayer()) return;
        renderer.renderPlayerStatus(game->getPlayer());
    }
    
    void renderTargets() {
        if (!game) return;
        renderer.renderEnemyDistances(game);
    }
    
    /**
     * @brief Отрисовать руку заклинаний (вызывается только по команде C)
     */
    void renderSpellHand() {
        if (!game) return;
        const SpellHand* hand = game->getPlayerSpellHand();

        if (hand) {
            renderer.renderSpellHand(hand);
        }
    }
    
    /**
     * @brief Отрисовать краткую информацию 
     */
    void renderCompact() {
        if (!game) return;
        renderer.renderField(game);
        renderer.renderCompactLegend();
        renderer.renderEnemyDistances(game);
        renderer.renderPlayerStatus(game->getPlayer());
    }
    
    void showMessage(const std::string& message) {
        renderer.showMessage(message);
    }
    
    void showError(const std::string& error) {
        renderer.showError(error);
    }
    
    void showWelcome() {
        renderer.printWelcome();
    }
    
    void showVictory() {
        if (!game || !game->getPlayer()) return;
        renderer.printVictory(game->getPlayer());
    }
    
    void showGameOver() {
        renderer.printGameOver();
    }
    
    TRenderer& getRenderer() {
        return renderer;
    }
};


#endif // GAMEVIEW_H


