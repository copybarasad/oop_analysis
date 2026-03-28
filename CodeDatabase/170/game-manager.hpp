#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "game.hpp"
#include "command.hpp"
#include <iostream>

namespace game
{
  template< typename InputReader >
  class GameManager
  {
  public:
    explicit GameManager(Game& game):
      game_(game)
    {}

    bool process_input()
    {
      Command cmd = input_reader_.read_command();

      try
      {
        switch (cmd.type)
        {
        case CommandType::Move:
          if (game_.get_state() == State::Ingame)
          {
            game_.get_level().make_turn(cmd.dx, cmd.dy);
          }
          break;
        case CommandType::BuySpell:
          if (game_.get_state() == State::Ingame)
          {
            game_.get_level().buy_spell_card(cmd.str_arg);
          }
          break;
        case CommandType::UseSpell:
          if (game_.get_state() == State::Ingame)
          {
            std::cout << "Casting spell from slot " << cmd.index << " at (" << cmd.target.first << ";" << cmd.target.second << ")\n";
            game_.get_level().use_spell_card(cmd.index, cmd.target);
          }
          break;
        case CommandType::Save:
          game_.save_game(cmd.str_arg);
          std::cout << "Game saved to " << cmd.str_arg << "\n";
          break;
        case CommandType::Load:
          game_.load_game(cmd.str_arg);
          std::cout << "Game loaded from " << cmd.str_arg << "\n";
          break;
        case CommandType::Quit:
          return false;
        case CommandType::Help:
          std::cout << "Commands: w/a/s/d (move), b [TRAP/AREA/BOOST], u [idx] [x] [y], S [file], L [file], q\n";
          break;
        case CommandType::Invalid:
          std::cout << "Invalid command.\n";
          break;
        default:
          break;
        }
      }
      catch (const std::exception& e)
      {
        std::cout << "Error: " << e.what() << "\n";
      }

      return true;
    }

  private:
    Game& game_;
    InputReader input_reader_;
  };
}

#endif