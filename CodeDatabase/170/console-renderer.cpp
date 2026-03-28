#include "console-renderer.hpp"
#include <iostream>
#include "field.hpp"
#include "player.hpp"

namespace game
{
  void ConsoleRenderer::render(Game& game)
  {
    if (game.get_state() == State::None)
    {
      std::cout << "Game not started.\n";
      return;
    }

    Field& field = game.get_level();
    auto size = field.get_grid_size();
    size_t rows = size.first;
    size_t cols = size.second;

    std::cout << "\n   ";
    for (size_t y = 0; y < cols; ++y)
    {
      std::cout << y % 10 << " ";
    }
    std::cout << "\n";

    for (size_t x = 0; x < rows; ++x)
    {
      if (x < 10) std::cout << " ";
      std::cout << x << " ";
      for (size_t y = 0; y < cols; ++y)
      {
        Position pos = { x, y };
        SquareType type = field.get_square_type(pos);
        char symbol = '.';

        switch (type)
        {
        case SquareType::Player: symbol = 'P'; break;
        case SquareType::Enemy: symbol = 'E'; break;
        case SquareType::EnemyTower: symbol = 'T'; break;
        case SquareType::Trap: symbol = '^'; break;
        default: break;
        }
        std::cout << symbol << " ";
      }
      std::cout << "\n";
    }

    try
    {
      auto& player = field.get_player();
      std::cout << "HP: " << player.get_health_points()
                << " | DMG: " << player.get_attack_damage()
                << " | Score: " << player.get_score()
                << " | Cards: " << player.get_hand().get_card_count() << "\n";
    }
    catch (...)
    {}
  }
}
