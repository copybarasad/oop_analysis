#ifndef CONSOLE_INPUT_HPP
#define CONSOLE_INPUT_HPP

#include <string>
#include <map>
#include <functional>
#include "command.hpp"

namespace game
{
  class ConsoleInputReader
  {
  public:
    ConsoleInputReader();
    Command read_command();

  private:
    using CommandHandler = std::function< Command() >;
    std::map< std::string, CommandHandler > command_map_;

    void init_commands();
  };
}

#endif
