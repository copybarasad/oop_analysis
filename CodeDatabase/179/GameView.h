#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "IGameRenderer.h"
#include "Game.h"
#include "GameEventNotifier.h"
#include "Logger.h"
#include <memory>
#include <functional>

template<typename RendererType>
class GameView {
private:
    std::unique_ptr<RendererType> renderer;
    Game* game;
    bool autoUpdate;
    
public:
    GameView(std::unique_ptr<RendererType> rendererObj, bool autoUpdate = true)
        : renderer(std::move(rendererObj)), game(nullptr), autoUpdate(autoUpdate) {
        
        LOG_INFO("GameView initialized");
        
        // Подписываемся на события игры
        GameEventNotifier::getInstance().addObserver(
            [this](const GameEvent& event) {
                this->onGameEvent(event);
            }
        );
    }
    
    virtual ~GameView() {
        LOG_INFO("GameView destroyed");
    }
    
    void setGame(Game* gameObj) {
        game = gameObj;
        LOG_DEBUG("Game reference set in GameView");
    }
    
    void update() {
        if (game && renderer) {
            try {
                renderer->clearScreen();
                
                // Получаем текущее состояние игры
                auto field = game->getCurrentField();
                if (field) {
                    renderer->renderField(*field);
                    renderer->renderGameState(*field, game->getCurrentLevel(),
                                             game->getPlayer()->getScore());
                }
                
                renderer->renderHelp();
            } catch (const std::exception& e) {
                LOG_ERROR("Error updating view: " + std::string(e.what()));
            }
        }
    }
    
    void renderMessage(const std::string& message) {
        if (renderer) {
            renderer->renderMessage(message);
        }
    }
    
    void onGameEvent(const GameEvent& event) {
        if (autoUpdate) {
            update();
        }
        
        // Выводим важные события
        switch (event.type) {
            case GameEventType::PLAYER_DAMAGE_TAKEN:
            case GameEventType::ENEMY_DESTROYED:
            case GameEventType::BUILDING_DESTROYED:
            case GameEventType::TOWER_DESTROYED:
            case GameEventType::LEVEL_UP:
            case GameEventType::GAME_OVER:
                renderMessage("Event: " + event.description);
                break;
            default:
                break;
        }
    }
    
    void setAutoUpdate(bool enable) {
        autoUpdate = enable;
    }
    
    RendererType* getRenderer() const {
        return renderer.get();
    }
};

#endif
