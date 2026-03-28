#ifndef LEVEL_PARSER_HPP
#define LEVEL_PARSER_HPP

#include <ios>
#include "level-config.hpp"

namespace game
{
  namespace detail
  {
    class LevelParser
    {
    public:
      static LevelConfig parse(const std::string& filename);

    private:
      static std::string trim(const std::string& string);
      static void parse_game_section(LevelConfig& config, const std::string& key, const std::string& value);
      static void parse_field_section(LevelConfig& config, const std::string& key, const std::string& value);
      static void parse_player_section(LevelConfig& config, const std::string& key, const std::string& value);
      static void parse_enemy_line(LevelConfig& config, const std::string& key, const std::string& value);
    };
  }
}

#endif
