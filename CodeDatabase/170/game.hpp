#ifndef GAME_HPP
#define GAME_HPP

#include <optional>
#include "field.hpp"
#include "level-config.hpp"

namespace game
{
  class Game
  {
  public:
    void start();
    void run_level();
    void load_game(const std::string& filename);
    void save_game(const std::string& filename);

    State get_state() const;
    Field& get_level();
    size_t get_current_level_index() const;

  private:
    std::optional< Field > level_;
    size_t current_level_index_ = 0;

    LevelConfig load_new_level_config(size_t index);
    void setup_level(const LevelConfig& config);

    LevelConfig get_current_config() const;
    void write_config_to_file(const LevelConfig& config, const std::string& filename);
  };
}

#endif
