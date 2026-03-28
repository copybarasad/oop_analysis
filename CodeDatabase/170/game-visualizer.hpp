#ifndef GAME_VISUALIZER_HPP
#define GAME_VISUALIZER_HPP

#include "game.hpp"

namespace game
{
  template < typename Renderer >
  class GameVisualizer
  {
  public:
    explicit GameVisualizer(Game& game) :
      game_(game)
    {}

    void draw()
    {
      renderer_.render(game_);
    }

  private:
    Game& game_;
    Renderer renderer_;
  };
}

#endif
