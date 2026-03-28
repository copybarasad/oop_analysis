#ifndef GAME_VISUALIZATION_MANAGER_H
#define GAME_VISUALIZATION_MANAGER_H

#include "game_renderer_interface.h"
#include "game_controller.h"
#include "game_enums.h"
#include <memory>
#include <functional>
#include <type_traits>

/**
 * Шаблонный класс управления визуализацией игры.
 * Параметр шаблона TRenderer должен реализовывать интерфейс IGameRenderer.
 * Отвечает за связь между игровой логикой и отрисовкой.
 */
template<typename TRenderer>
class GameVisualizationManager {
private:
    std::unique_ptr<TRenderer> renderer;
    
public:
    // Конструктор с perfect forwarding для аргументов рендерера
    template<typename... Args>
    explicit GameVisualizationManager(Args&&... args)
        : renderer(std::make_unique<TRenderer>(std::forward<Args>(args)...)) {
        static_assert(std::is_base_of_v<IGameRenderer, TRenderer>,
                     "TRenderer must inherit from IGameRenderer");
    }
    
    // Основные методы отрисовки состояний игры
    void onGameStateChanged(const GameController& game) const {
        renderer->renderGameState(game);
    }
    
    void onMainMenuShown() const {
        renderer->renderMainMenu();
    }
    
    void onLoadMenuShown(const std::vector<std::string>& saveFiles) const {
        renderer->renderLoadMenu(saveFiles);
    }
    
    void onInGameMenuShown() const {
        renderer->renderInGameMenu();
    }
    
    void onGameOver(const GameController& game) const {
        renderer->renderGameOverMenu(game);
    }
    
    // Методы для событий игры
    void onLevelStart(int level, const LevelConfig& config) const {
        renderer->renderLevelStart(level, config);
    }
    
    void onLevelComplete(int level, int score) const {
        renderer->renderLevelComplete(level, score);
    }
    
    void onVictory(int finalScore) const {
        renderer->renderVictory(finalScore);
    }
    
    // Методы для сообщений
    void showMessage(const std::string& message) const {
        renderer->renderMessage(message);
    }
    
    void showError(const std::string& error) const {
        renderer->renderError(error);
    }
    
    // Методы для отдельных компонентов
    void renderPlayerStats(const Player& player) const {
        renderer->renderPlayerStats(player);
    }
    
    void renderFieldGrid(const GameController& game) const {
        renderer->renderFieldGrid(game);
    }
    
    // Методы для конкретных игровых действий
    void onPlayerMoved(const GameController& game) const {
        onGameStateChanged(game);
    }
    
    void onSpellCast(const GameController& game) const {
        onGameStateChanged(game);
    }
    
    void onCombatModeSwitched(const GameController& game) const {
        onGameStateChanged(game);
    }
    
    void onSpellBought(const GameController& game) const {
        onGameStateChanged(game);
    }
    
    void onTurnCompleted(const GameController& game) const {
        onGameStateChanged(game);
    }
    
    // Универсальный метод для любых изменений состояния
    void onStateChanged(const GameController& game) const {
        onGameStateChanged(game);
    }
};

#endif