#include "game.hpp"
#include <fstream>
#include <stdexcept>
#include <utility>
#include "level-parser.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy-tower.hpp"

void game::Game::load_game(const std::string& filename)
{
  LevelConfig config = detail::LevelParser::parse(filename);
  setup_level(config);
}

game::LevelConfig game::Game::load_new_level_config(size_t index)
{
  std::string filename = "saves/level_" + std::to_string(index) + ".cfg";
  LevelConfig config = detail::LevelParser::parse(filename);
  if (config.current_level_index == 0)
  {
    config.current_level_index = index;
  }
  return config;
}

void game::Game::setup_level(const LevelConfig& config)
{
  level_.emplace(config.rows, config.cols);

  level_->summon_player(config.player_traits, config.player_pos);

  for (const auto& pair: config.enemies)
  {
    const EnemyTraits& traits = pair.first;
    const Position& pos = pair.second;
    level_->summon_enemy(traits, pos);
  }

  for (const auto& pair: config.towers)
  {
    const EnemyTowerTraits& traits = pair.first;
    const Position& pos = pair.second;
    level_->summon_tower(traits, pos);
  }
  current_level_index_ = config.current_level_index;
}

game::LevelConfig game::Game::get_current_config() const
{
  LevelConfig config;
  config.current_level_index = current_level_index_;

  std::pair< size_t, size_t > size = level_->get_grid_size();
  config.rows = size.first;
  config.cols = size.second;
  const Player& player = level_->get_player();

  config.player_traits.base_traits.health_points = player.get_health_points();
  config.player_traits.base_traits.attack_damage = player.get_attack_damage();
  config.player_traits.score = player.get_score();
  config.player_pos = player.get_position();
  const std::vector< std::unique_ptr< Enemy > >& enemies = level_->get_enemies_ptrs();
  for (const auto& enemy_ptr: enemies)
  {
    EnemyTraits traits;
    traits.base_traits.health_points = enemy_ptr->get_health_points();
    traits.base_traits.attack_damage = enemy_ptr->get_attack_damage();
    config.enemies.push_back(std::make_pair(traits, enemy_ptr->get_position()));
  }

  const std::vector< std::unique_ptr< EnemyTower > >& towers = level_->get_towers_ptrs();
  for (const auto& tower_ptr: towers)
  {
    EnemyTowerTraits traits;
    traits.base_traits.health_points = tower_ptr->get_health_points();
    traits.base_traits.attack_damage = tower_ptr->get_attack_damage();
    traits.attack_range = tower_ptr->get_attack_range();
    traits.cooldown = tower_ptr->get_cooldown();
    config.towers.push_back(std::make_pair(traits, tower_ptr->get_position()));
  }

  return config;
}

void game::Game::write_config_to_file(const LevelConfig& config, const std::string& filename)
{
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::ios_base::failure("Unable to open save file for writing: " + filename);
  }

  file << "[GAME]" << '\n';
  file << "CURRENT_LEVEL_INDEX=" << config.current_level_index << '\n';
  file << '\n';
  file << "[FIELD]" << '\n';
  file << "ROWS=" << config.rows << '\n';
  file << "COLS=" << config.cols << '\n';
  file << '\n';

  file << "[PLAYER]" << '\n';
  file << "HEALTH_POINTS=" << config.player_traits.base_traits.health_points << '\n';
  file << "ATTACK_DAMAGE=" << config.player_traits.base_traits.attack_damage << '\n';
  file << "SCORE=" << config.player_traits.score << '\n';
  file << "START_X=" << config.player_pos.first << '\n';
  file << "START_Y=" << config.player_pos.second << '\n';
  file << '\n';
  file << "[ENEMY]" << '\n';
  for (size_t i = 0; i < config.enemies.size(); ++i)
  {
    const auto& pair = config.enemies[i];
    file << "HEALTH_POINTS=" << pair.first.base_traits.health_points << ", ";
    file << "ATTACK_DAMAGE=" << pair.first.base_traits.attack_damage << ", ";
    file << "START_X=" << pair.second.first << ", ";
    file << "START_Y=" << pair.second.second << '\n';
  }

  file << '\n';

  file << "[TOWER]" << '\n';
  for (size_t i = 0; i < config.towers.size(); ++i)
  {
    const auto& pair = config.towers[i];
    file << "HEALTH_POINTS=" << pair.first.base_traits.health_points << ", ";
    file << "ATTACK_DAMAGE=" << pair.first.base_traits.attack_damage << ", ";
    file << "ATTACK_RANGE=" << pair.first.attack_range << ", ";
    file << "COOLDOWN=" << pair.first.cooldown << ", ";
    file << "START_X=" << pair.second.first << ", ";
    file << "START_Y=" << pair.second.second << '\n';
  }

  file << '\n';
}

void game::Game::start()
{
  current_level_index_ = 1;
  LevelConfig config = load_new_level_config(current_level_index_);
  setup_level(config);
}

void game::Game::run_level()
{
  if (!level_)
  {
    return;
  }

  State current_state = level_->get_state();
  if (current_state == State::Ingame)
  {
    return;
  }

  Player& player = level_->get_player();
  PlayerTraits current_player_traits = player.get_traits();
  std::vector< std::unique_ptr< SpellCard > > old_cards = player.get_hand().get_cards_for_transfer();

  if (current_state == State::Won)
  {
    ++current_level_index_;

    LevelConfig config;
    try
    {
      config = load_new_level_config(current_level_index_);
    }
    catch (const std::exception&)
    {
      level_->set_state(State::Won);
      return;
    }

    config.player_traits.score = current_player_traits.score;

    setup_level(config);

    Player& new_player = level_->get_player();
    new_player.get_hand().set_cards(std::move(old_cards));
    new_player.get_hand().remove_random_half_cards();
  }
}

void game::Game::save_game(const std::string& filename)
{
  if (!level_)
  {
    throw std::runtime_error("Cannot save game: no level loaded");
  }
  LevelConfig config = get_current_config();
  write_config_to_file(config, filename);
}

game::State game::Game::get_state() const
{
  if (!level_)
  {
    return State::None;
  }
  return level_->get_state();
}

game::Field& game::Game::get_level()
{
  if (!level_)
  {
    throw std::runtime_error("Level is not loaded");
  }
  return *level_;
}

size_t game::Game::get_current_level_index() const
{
  return current_level_index_;
}
