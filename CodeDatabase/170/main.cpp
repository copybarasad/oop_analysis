#include "game.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy-tower.hpp"
#include "game-manager.hpp"
#include "game-visualizer.hpp"
#include "console-input.hpp"
#include "console-renderer.hpp"
#include <iostream>

int main()
{
  game::Game game;

  try
  {
    game.start();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Failed to start game: " << e.what() << '\n';
    return 1;
  }

  game::GameManager< game::ConsoleInputReader > manager(game);
  game::GameVisualizer< game::ConsoleRenderer > visualizer(game);

  bool running = true;
  while (running)
  {
    visualizer.draw();

    game::State state = game.get_state();
    if (state == game::State::Won)
    {
      size_t current = game.get_current_level_index();
      if (current >= 3)
      {
        std::cout << "CONGRATULATIONS! YOU BEAT THE GAME!\n";
        running = false;
        continue;
      }
      else
      {
        std::cout << "Level " << current << " Complete! Proceeding to next level...\n";
        game.run_level();
        visualizer.draw();
      }
    }
    else if (state == game::State::Lost)
    {
      std::cout << "GAME OVER. Restarting...\n";
      game.start();
      visualizer.draw();
    }

    running = manager.process_input();
  }
}