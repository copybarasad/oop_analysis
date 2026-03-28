#include "level-parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <map>

std::string game::detail::LevelParser::trim(const std::string& string)
{
  const std::string spaces = " \t\n\r\f\v";
  size_t start = string.find_first_not_of(spaces);
  if (start == std::string::npos)
  {
    return "";
  }
  size_t end = string.find_last_not_of(spaces);
  return string.substr(start, end - start + 1);
}

void game::detail::LevelParser::parse_game_section(LevelConfig& config, const std::string& key, const std::string& value)
{
  try
  {
    if (key == "CURRENT_LEVEL_INDEX")
    {
      config.current_level_index = std::stoul(value);
    }
    else
    {
      throw std::runtime_error("Unknown key in [GAME] section");
    }
  }
  catch (const std::invalid_argument&)
  {
    throw std::runtime_error("Config data error");
  }
}

void game::detail::LevelParser::parse_field_section(LevelConfig& config, const std::string& key, const std::string& value)
{
  try
  {
    if (key == "ROWS")
    {
      config.rows = std::stoul(value);
    }
    else if (key == "COLS")
    {
      config.cols = std::stoul(value);
    }
    else
    {
      throw std::logic_error("Unknown key");
    }
  }
  catch (const std::invalid_argument&)
  {
    throw std::runtime_error("Config data error");
  }
}

void game::detail::LevelParser::parse_player_section(LevelConfig& config, const std::string& key, const std::string& value)
{
  try
  {
    if (key == "START_X")
    {
      config.player_pos.first = std::stoul(value);
    }
    else if (key == "START_Y")
    {
      config.player_pos.second = std::stoul(value);
    }
    else if (key == "HEALTH_POINTS")
    {
      config.player_traits.base_traits.health_points = std::stoul(value);
    }
    else if (key == "ATTACK_DAMAGE")
    {
      config.player_traits.base_traits.attack_damage = std::stoul(value);
    }
    else if (key == "SCORE")
    {
      config.player_traits.score = std::stoul(value);
    }
    else
    {
      throw std::logic_error("Unknown key");
    }
  }
  catch (const std::invalid_argument&)
  {
    throw std::runtime_error("Config data error");
  }
}

void game::detail::LevelParser::parse_enemy_line(LevelConfig& config, const std::string& section, const std::string& line)
{
  std::stringstream ss(line);
  std::string segment;
  std::map< std::string, std::string > kvs;

  while (std::getline(ss, segment, ','))
  {
    size_t delimiter_pos = segment.find('=');
    if (delimiter_pos == std::string::npos)
    {
      throw std::runtime_error("Invalid entity format: missing '='");
    }
    std::string key = trim(segment.substr(0, delimiter_pos));
    std::string value = trim(segment.substr(delimiter_pos + 1));
    kvs[key] = value;
  }

  try
  {
    size_t health = std::stoul(kvs.at("HEALTH_POINTS"));
    size_t damage = std::stoul(kvs.at("ATTACK_DAMAGE"));
    size_t x = std::stoul(kvs.at("START_X"));
    size_t y = std::stoul(kvs.at("START_Y"));
    Position pos = { x, y };

    if (section == "ENEMY")
    {
      EnemyTraits traits = { { health, damage } };
      config.enemies.push_back(std::make_pair(traits, pos));
    }
    else if (section == "TOWER")
    {
      size_t range = std::stoul(kvs.at("ATTACK_RANGE"));
      size_t cooldown = std::stoul(kvs.at("COOLDOWN"));
      EnemyTowerTraits traits = { { health, damage }, range, cooldown };
      config.towers.push_back(std::make_pair(traits, pos));
    }
    else
    {
      throw std::runtime_error("Unknown entity section");
    }
  }
  catch (const std::invalid_argument&)
  {
    throw std::runtime_error("Invalid number format in entity line");
  }
  catch (const std::out_of_range&)
  {
    throw std::runtime_error("Number out of range or missing key in entity line");
  }
}

game::LevelConfig game::detail::LevelParser::parse(const std::string& filename)
{
  std::ifstream file(filename);

  if (!file.is_open())
  {
    throw std::logic_error("Failed to open or find level configuration file");
  }

  LevelConfig config;
  std::string line;
  std::string current_section;
  size_t line_num = 0;
  while (std::getline(file, line))
  {
    ++line_num;
    line = trim(line);
    if (line.empty() || line[0] == '#')
    {
      continue;
    }

    if (line[0] == '[' && line.back() == ']')
    {
      current_section = line.substr(1, line.length() - 2);
      continue;
    }

    if (current_section.empty())
    {
      throw std::runtime_error("Missing section header before key-value pair");
    }

    try
    {
      if (current_section == "ENEMY" || current_section == "TOWER")
      {
        parse_enemy_line(config, current_section, line);
      }
      else
      {
        size_t delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos)
        {
          throw std::runtime_error("Invalid key-value format");
        }

        std::string key = trim(line.substr(0, delimiter_pos));
        std::string value = trim(line.substr(delimiter_pos + 1));

        if (current_section == "GAME")
        {
          parse_game_section(config, key, value);
        }
        else if (current_section == "FIELD")
        {
          parse_field_section(config, key, value);
        }
        else if (current_section == "PLAYER")
        {
          parse_player_section(config, key, value);
        }
      }
    }
    catch (const std::exception& e)
    {
      throw std::runtime_error("Parsing error");
    }
  }

  if (config.rows == 0 || config.cols == 0)
  {
    throw std::runtime_error("Config file is incomplete: FIELD ROWS or COLS are missing or invalid");
  }

  return config;
}
