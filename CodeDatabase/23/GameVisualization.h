#ifndef GAME_VISUALIZATION_H
#define GAME_VISUALIZATION_H

#include "GameField.h"
#include "Renderer.h"
#include <memory>




template <typename TRenderer> class GameVisualization {
private:
  TRenderer renderer;
  const GameField *field;

public:
  explicit GameVisualization(const GameField *gameField) : field(gameField) {}

  GameVisualization(const GameField *gameField, TRenderer customRenderer)
      : renderer(std::move(customRenderer)), field(gameField) {}

  
  void render() { renderer.render(field); }

  
  void onGameStateChanged() { render(); }

  
  void displayMessage(const std::string &message) {
    renderer.displayMessage(message);
  }

  
  TRenderer &getRenderer() { return renderer; }

  const TRenderer &getRenderer() const { return renderer; }

  
  void setField(const GameField *newField) { field = newField; }
};

#endif 
