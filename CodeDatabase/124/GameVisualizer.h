#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "IRenderAdapter.h"
#include "gamefield.h"
#include "player.h"
#include "enemy.h"
#include "enemytower.h"
#include "EnemyManager.h"
#include <memory>
#include <vector>

template<typename RenderAdapter>
class GameVisualizer {
private: 
    std::unique_ptr<RenderAdapter> renderAdapter;
    
public: 
    explicit GameVisualizer(std::unique_ptr<RenderAdapter> render) 
        : renderAdapter(std::move(render)) { 
    }
     
    void renderGame(const GameField& field, const Player& player, 
                    const Enemy& enemy, const EnemyManager& enemyManager) {
        
        if (renderAdapter) {
            renderAdapter->renderGame(field, player, enemy, 
                                     field.getTowers(), enemyManager);
        }
    }
    
    void renderHelp() {
        if (renderAdapter) {
            renderAdapter->renderHelp();
        }
    }
    
    void renderUI(const GameField& field, const Player& player,
                  const Enemy& enemy, const EnemyManager& enemyManager) {
        if (renderAdapter) {
            renderAdapter->renderUI(field, player, enemy,
                                   field.getTowers(), enemyManager);
        }
    }
     
    RenderAdapter* getRenderer() { 
        return renderAdapter.get(); 
    }
    
    const RenderAdapter* getRenderer() const { 
        return renderAdapter.get(); 
    }
     
    bool isInitialized() const {
        return renderAdapter != nullptr;
    }
};

#endif