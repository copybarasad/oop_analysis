#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "Game.h"
#include "GameRenderer.h"

template<typename RendererType>
class GameVisualizer {
private:
  Game& game;
  RendererType renderer;
  
public:
  GameVisualizer(Game& gameRef, const RendererType& rendererType) 
    : game(gameRef), renderer(rendererType) {}
  
  void visualize() {
    renderer.render(game);
  }
  
  void visualizeMenu(const std::vector<std::string>& options) {
    renderer.renderMenu(options);
  }
  
  void visualizeMessage(const std::string& message) {
    renderer.renderMessage(message);
  }
  
  void onGameStateChanged() {
    visualize();
  }
};

#endif