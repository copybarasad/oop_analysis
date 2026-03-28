#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <utility>

namespace game
{
  enum class CommandType
  {
    Move,
    BuySpell,
    UseSpell,
    Save,
    Load,
    Quit,
    Help,
    None,
    Invalid
  };

  struct Command
  {
    CommandType type = CommandType::None;
    int dx = 0;
    int dy = 0;
    size_t index = 0;
    std::pair< size_t, size_t > target = { 0, 0 };
    std::string str_arg;
  };
}

#endif
