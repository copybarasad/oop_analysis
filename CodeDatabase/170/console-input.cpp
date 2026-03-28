#include "console-input.hpp"
#include <iostream>
#include <limits>

namespace game
{
  ConsoleInputReader::ConsoleInputReader()
  {
    init_commands();
  }

  void ConsoleInputReader::init_commands()
  {
    command_map_["w"] = []() {
      return Command{ CommandType::Move, -1, 0 };
    };

    command_map_["s"] = []() {
      return Command{ CommandType::Move, 1, 0 };
    };

    command_map_["a"] = []() {
      return Command{ CommandType::Move, 0, -1 };
    };

    command_map_["d"] = []() {
      return Command{ CommandType::Move, 0, 1 };
    };

    command_map_["b"] = []() {
      Command cmd;
      cmd.type = CommandType::BuySpell;
      std::cin >> cmd.str_arg;
      return cmd;
    };

    command_map_["u"] = []() {
      Command cmd;
      cmd.type = CommandType::UseSpell;
      std::cin >> cmd.index >> cmd.target.first >> cmd.target.second;
      return cmd;
    };

    command_map_["S"] = []() {
      Command cmd;
      cmd.type = CommandType::Save;
      std::cin >> cmd.str_arg;
      return cmd;
    };

    command_map_["L"] = []() {
      Command cmd;
      cmd.type = CommandType::Load;
      std::cin >> cmd.str_arg;
      return cmd;
    };

    command_map_["q"] = []() {
      return Command{ CommandType::Quit };
    };

    command_map_["h"] = []() {
      return Command{ CommandType::Help };
    };
  }

  Command ConsoleInputReader::read_command()
  {
    std::cout << "Enter command (w/a/s/d, b [type], u [id] [x] [y], S [file], L [file], q, h): ";
    std::string input;
    std::cin >> input;

    auto it = command_map_.find(input);
    if (it != command_map_.end())
    {
      return it->second();
    }

    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return Command{ CommandType::Invalid };
  }
}