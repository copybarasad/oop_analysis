#include "field.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy-tower.hpp"
#include <stdexcept>

game::Field::Field():
  grid_(MIN_FIELD_LENGTH, std::vector< Square >(MIN_FIELD_WIDTH)),
  player_(nullptr)
{}

game::Field::Field(size_t rows):
  Field(rows, rows)
{}

game::Field::Field(size_t rows, size_t cols):
  grid_(rows, std::vector< Square >(cols)),
  player_(nullptr)
{
  if (rows < MIN_FIELD_LENGTH || rows > MAX_FIELD_LENGTH
      || cols < MIN_FIELD_WIDTH || cols > MAX_FIELD_WIDTH)
  {
    throw std::logic_error("Unsupported field dimensions");
  }
}

game::Field::Field(const Field& rhs):
  grid_(rhs.grid_),
  player_(std::make_unique< Player >(*(rhs.player_))),
  state_(rhs.state_)
{
  for (const auto& enemy: rhs.enemies_)
  {
    if (enemy)
    {
      enemies_.emplace_back(std::make_unique< Enemy >(*enemy));
    }
  }
  for (const auto& tower: rhs.towers_)
  {
    if (tower)
    {
      towers_.emplace_back(std::make_unique< EnemyTower >(*tower));
    }
  }
  update_entity_ptrs();
}

game::Field::Field(Field&& rhs) noexcept:
  grid_(std::move(rhs.grid_)),
  player_(std::move(rhs.player_)),
  enemies_(std::move(rhs.enemies_)),
  towers_(std::move(rhs.towers_)),
  state_(rhs.state_)
{
  update_entity_ptrs();
}

game::Field& game::Field::operator=(const Field& rhs)
{
  Field copy(rhs);
  swap(copy);
  return *this;
}

game::Field& game::Field::operator=(Field&& rhs) noexcept
{
  Field copy(std::move(rhs));
  swap(copy);
  return *this;
}

void game::Field::swap(Field& other) noexcept
{
  std::swap(grid_, other.grid_);
  std::swap(player_, other.player_);
  std::swap(enemies_, other.enemies_);
  std::swap(towers_, other.towers_);
  std::swap(state_, other.state_);

  update_entity_ptrs();
  other.update_entity_ptrs();
}

void game::Field::summon_player(const PlayerTraits& traits, Position pos)
{
  if (state_ != State::None)
  {
    log_ << "Failed to summon player: there is another one already.\n";
    return;
  }

  if (!is_valid_position(pos))
  {
    log_ << "Failed to summon player: wrong position.\n";
    return;
  }

  if (grid_[pos.first][pos.second].type != SquareType::Empty)
  {
    log_ << "Failed to summon player: square is not empty.\n";
    return;
  }

  grid_[pos.first][pos.second].type = SquareType::Player;
  player_ = std::make_unique< Player >(traits, this, pos);
  state_ = State::Ingame;

  log_ << "Summoned player with ";
  log_ << traits.base_traits.health_points << " hp, ";
  log_ << traits.base_traits.attack_damage << " damage, ";
  log_ << '(' << pos.first << ';' << pos.second << ')' << " position.\n";
}

void game::Field::summon_enemy(const EnemyTraits& traits, Position pos)
{
  if (state_ == State::Lost || state_ == State::Won)
  {
    log_ << "Failed to summon enemy: game session ended, reset the game, then summon.\n";
    return;
  }

  if (!is_valid_position(pos))
  {
    log_ << "Failed to summon enemy: wrong position.\n";
    return;
  }

  if (grid_[pos.first][pos.second].type != SquareType::Empty)
  {
    log_ << "Failed to summon enemy: square is not empty.\n";
    return;
  }

  grid_[pos.first][pos.second].type = SquareType::Enemy;
  enemies_.emplace_back(std::make_unique< Enemy >(traits, this, pos));

  log_ << "Summoned enemy with ";
  log_ << traits.base_traits.health_points << " hp, ";
  log_ << traits.base_traits.attack_damage << " damage, ";
  log_ << '(' << pos.first << ';' << pos.second << ')' << " position.\n";
}

void game::Field::summon_tower(const EnemyTowerTraits& traits, Position pos)
{
  if (state_ == State::Lost || state_ == State::Won)
  {
    log_ << "Failed to summon tower: game session ended, reset the game, then summon.\n";
    return;
  }

  if (!is_valid_position(pos))
  {
    log_ << "Failed to summon tower: wrong position.\n";
    return;
  }

  if (grid_[pos.first][pos.second].type != SquareType::Empty)
  {
    log_ << "Failed to summon tower: square is not empty.\n";
    return;
  }

  grid_[pos.first][pos.second].type = SquareType::EnemyTower;
  towers_.emplace_back(std::make_unique< EnemyTower >(traits, this, pos));

  log_ << "Summoned enemy tower with ";
  log_ << traits.base_traits.health_points << " hp, ";
  log_ << traits.base_traits.attack_damage << " damage, ";
  log_ << '(' << pos.first << ';' << pos.second << ')' << " position.\n";
}

void game::Field::move_player(Position pos)
{
  Position old_pos = player_->get_position();
  if (old_pos == pos)
  {
    return;
  }

  SquareType target_type = grid_[pos.first][pos.second].type;

  if (target_type == SquareType::Empty)
  {
    grid_[old_pos.first][old_pos.second].type = SquareType::Empty;
    player_->move_to_pos(pos);
    grid_[pos.first][pos.second].type = SquareType::Player;
    log_ << "Moved player to position (" << pos.first << ',' << pos.second << ").\n";
  }
  else if (target_type == SquareType::Enemy)
  {
    Enemy* enemy = get_enemy(pos);
    if (enemy != nullptr && enemy->is_alive())
    {
      size_t enemy_health_before = enemy->get_health_points();
      size_t enemy_damage = enemy->get_attack_damage();
      enemy->take_damage(player_->get_attack_damage());
      log_ << "Player attacks enemy at (" << pos.first << ',' << pos.second << ")";
      log_ << ", enemy lives: " << enemy->get_health_points() << '\n';

      size_t points = enemy_health_before + enemy_damage;
      player_->gain_points(points);
      if (!enemy->is_alive())
      {
        player_->gain_points(1);
        grid_[pos.first][pos.second].type = SquareType::Empty;
        log_ << "Enemy at (" << pos.first << ',' << pos.second << ") died. ";
        log_ << "Gained " << (points + 1) << " points.\n";
        kill_enemy(enemy);
        if (enemies_.empty() && towers_.empty())
        {
          state_ = State::Won;
          log_ << "All enemies defeated! You won! ";
          log_ << "Final score: " << player_->get_score() << '\n';
        }
      }
    }
  }
  else if (target_type == SquareType::EnemyTower)
  {
    EnemyTower* tower = get_tower(pos);
    if (tower != nullptr && tower->is_alive())
    {
      size_t tower_health_before = tower->get_health_points();
      size_t tower_damage = tower->get_attack_damage();
      tower->take_damage(player_->get_attack_damage());
      log_ << "Player attacks enemy tower at (" << pos.first << ',' << pos.second << ")";
      log_ << ", tower lives: " << tower->get_health_points() << '\n';

      size_t points = tower_health_before + tower_damage;
      player_->gain_points(points);
      if (!tower->is_alive())
      {
        player_->gain_points(1);
        grid_[pos.first][pos.second].type = SquareType::Empty;
        log_ << "Enemy tower at (" << pos.first << ',' << pos.second << ") destroyed. ";
        log_ << "Gained " << (points + 1) << " points.\n";
        kill_tower(tower);
        if (enemies_.empty() && towers_.empty())
        {
          state_ = State::Won;
          log_ << "All enemies defeated! You won! ";
          log_ << "Final score: " << player_->get_score() << '\n';
        }
      }
    }
  }
  else if (grid_[pos.first][pos.second].type == SquareType::Trap)
  {
    size_t damage = grid_[pos.first][pos.second].trap_damage;
    log_ << "Player steps on a trap at (" << pos.first << ',' << pos.second << ")!\n";
    player_->take_damage(damage);
    log_ << "Player takes " << damage << " damage. Health: " << player_->get_health_points() << "\n";

    grid_[pos.first][pos.second].type = SquareType::Empty;
    grid_[pos.first][pos.second].trap_damage = 0;

    grid_[old_pos.first][old_pos.second].type = SquareType::Empty;
    player_->move_to_pos(pos);
    grid_[pos.first][pos.second].type = SquareType::Player;

    if (!player_->is_alive())
    {
      state_ = State::Lost;
      log_ << "Game over! Player died from a trap!\n";
      log_ << "Final score: " << player_->get_score() << "\n";
      return;
    }
  }
  else
  {
    log_ << "Failed to move player: target position is occupied.\n";
  }
}

void game::Field::make_turn(int dx, int dy)
{
  switch (state_)
  {
  case State::None:
    log_ << "Game is not started yet. Summon player first.\n";
    return;
  case State::Lost:
    log_ << "Game is lost. Reset the game.\n";
    return;
  case State::Won:
    log_ << "Game is won. Reset the game.\n";
    return;
  default:
    break;
  }

  if (player_ == nullptr)
  {
    log_ << "Failed to make turn: there is no player yet, summon it first.";
    return;
  }

  Position current_pos = player_->get_position();
  Position new_pos = { current_pos.first + dx, current_pos.second + dy };

  if (!is_valid_position(new_pos))
  {
    log_ << "Failed to move player: position out of bounds.\n";
    return;
  }

  move_player(new_pos);

  Position final_pos = player_->get_position();
  log_ << "Player at (" << final_pos.first << ';' << final_pos.second << ')';
  log_ << ", " << player_->get_health_points() << " hp";
  log_ << ", " << player_->get_score() << " points\n";

  if (state_ == State::Ingame && player_->is_alive())
  {
    move_enemies();
    move_towers();
  }

  if (state_ == State::Ingame && !player_->is_alive())
  {
    state_ = State::Lost;
    log_ << "Game over! ";
    log_ << "Final score: " << player_->get_score() << "\n";
  }
}

void game::Field::buy_spell_card(const std::string& spell_name)
{
  if (state_ != State::Ingame)
  {
    log_ << "Cannot buy spell card, game is not started yet.\n";
    return;
  }

  size_t player_score = player_->get_score();

  if (spell_name == "BOOST")
  {
    if (player_score < 5)
    {
      log_ << "Failed to buy boost spell card: not enough score. Need 5, have " << player_score << "\n";
      return;
    }
    player_->spend_points(5);
    player_->add_boost_spell_card();
    log_ << "Bought Boost spell card for 5 points. Remaining: " << player_->get_score() << "\n";
  }
  else if (spell_name == "AREA")
  {
    if (player_score < 10)
    {
      log_ << "Failed to buy area spell card: not enough score. Need 10, have " << player_score << "\n";
      return;
    }
    player_->spend_points(10);
    player_->add_area_spell_card();
    log_ << "Bought Area spell card for 10 points. Remaining: " << player_->get_score() << "\n";
  }
  else if (spell_name == "TRAP")
  {
    if (player_score < 3)
    {
      log_ << "Failed to buy trap spell card: not enough score. Need 3, have " << player_score << "\n";
      return;
    }
    player_->spend_points(3);
    player_->add_trap_spell_card();
    log_ << "Bought Trap spell card for 3 points. Remaining: " << player_->get_score() << "\n";
  }
  else
  {
    log_ << "Unsupported spell card name to buy.\n";
  }
}

void game::Field::use_spell_card(size_t index)
{
  if (state_ != State::Ingame)
  {
    log_ << "Cannot use spell cards, game is not started yet.\n";
    return;
  }
  player_->use_spell_card(index, player_->get_position());
  log_ << "Spell card used.\n";
  move_enemies();
}

void game::Field::use_spell_card(size_t index, Position pos)
{
  if (state_ != State::Ingame)
  {
    log_ << "Cannot use any spell cards, game is not started yet.\n";
    return;
  }
  player_->use_spell_card(index, pos);
  log_ << "Spell card used.\n";
  move_enemies();
}

void game::Field::reset()
{
  player_.reset();
  enemies_.clear();
  towers_.clear();

  for (auto& row: grid_)
  {
    for (auto& square: row)
    {
      square.type = SquareType::Empty;
    }
  }

  state_ = State::None;
  log_ << "Game was reset.\n";
}

game::SquareType game::Field::get_square_type(Position pos) const noexcept
{
  return grid_[pos.first][pos.second].type;
}

void game::Field::set_square_type(SquareType type, Position pos, size_t trap_damage) noexcept
{
  grid_[pos.first][pos.second].type = type;
  if (type == SquareType::Trap)
  {
    grid_[pos.first][pos.second].trap_damage = trap_damage;
  }
  else
  {
    grid_[pos.first][pos.second].trap_damage = 0;
  }
}

game::State game::Field::get_state() const noexcept
{
  return state_;
}

void game::Field::set_state(State state) noexcept
{
  state_ = state;
}

std::ostream& game::Field::get_log_stream() noexcept
{
  return log_;
}

std::pair< size_t, size_t > game::Field::get_grid_size() const noexcept
{
  return { grid_.size(), grid_[0].size() };
}

game::Player& game::Field::get_player()
{
  if (!player_)
  {
    throw std::runtime_error("Player is not initialized");
  }
  return *player_;
}

const game::Player& game::Field::get_player() const
{
  if (!player_)
  {
    throw std::runtime_error("Player is not initialized");
  }
  return *player_;
}

const std::vector< std::unique_ptr< game::Enemy > >& game::Field::get_enemies_ptrs() const
{
  return enemies_;
}

const std::vector< std::unique_ptr< game::EnemyTower > >& game::Field::get_towers_ptrs() const
{
  return towers_;
}

bool game::Field::is_valid_position(Position pos) const noexcept
{
  if (grid_.empty() || grid_[0].empty())
  {
    return false;
  }
  return (pos.first < grid_.size() && pos.second < grid_[0].size());
}

void game::Field::move_enemies()
{
  Position player_pos = player_->get_position();

  for (auto it = enemies_.begin(); it != enemies_.end();)
  {
    auto& enemy = *it;
    Position enemy_pos = enemy->get_position();

    if (grid_[enemy_pos.first][enemy_pos.second].type == SquareType::Trap)
    {
      size_t damage = grid_[enemy_pos.first][enemy_pos.second].trap_damage;
      log_ << "Enemy at (" << enemy_pos.first << ';' << enemy_pos.second << ") stepped on a trap!\n";
      enemy->take_damage(damage);
      log_ << "Enemy takes " << damage << " damage. Health: " << enemy->get_health_points() << "\n";

      grid_[enemy_pos.first][enemy_pos.second].type = SquareType::Empty;
      grid_[enemy_pos.first][enemy_pos.second].trap_damage = 0;

      if (!enemy->is_alive())
      {
        log_ << "Enemy died from the trap.\n";
        grid_[enemy_pos.first][enemy_pos.second].type = SquareType::Empty;
        it = enemies_.erase(it);

        if (enemies_.empty() && towers_.empty())
        {
          state_ = State::Won;
          log_ << "All enemies defeated! You won! ";
          log_ << "Final score: " << player_->get_score() << '\n';
        }
        continue;
      }
    }

    bool can_attack = false;

    Position possible_moves[] = {
      {enemy_pos.first + 1, enemy_pos.second},
      {enemy_pos.first - 1, enemy_pos.second},
      {enemy_pos.first, enemy_pos.second + 1},
      {enemy_pos.first, enemy_pos.second - 1}
    };

    for (const auto& move: possible_moves)
    {
      if (is_valid_position(move) && grid_[move.first][move.second].type == SquareType::Player)
      {
        can_attack = true;
        break;
      }
    }

    if (can_attack)
    {
      player_->take_damage(enemy->get_attack_damage());
      log_ << "Enemy attacks player. ";
      log_ << "Player health: " << player_->get_health_points() << "\n";

      if (!player_->is_alive())
      {
        state_ = State::Lost;
        log_ << "Game over! ";
        log_ << "Final score: " << player_->get_score() << "\n";
        return;
      }
      ++it;
    }
    else
    {
      int dx = 0;
      int dy = 0;

      if (player_pos.first > enemy_pos.first)
      {
        dx = 1;
      }
      else if (player_pos.first < enemy_pos.first)
      {
        dx = -1;
      }
      if (player_pos.second > enemy_pos.second)
      {
        dy = 1;
      }
      else if (player_pos.second < enemy_pos.second)
      {
        dy = -1;
      }

      Position new_pos = { enemy_pos.first + dx, enemy_pos.second + dy };

      if (is_valid_position(new_pos))
      {
        if (grid_[new_pos.first][new_pos.second].type == SquareType::Empty)
        {
          grid_[enemy_pos.first][enemy_pos.second].type = SquareType::Empty;
          enemy->move_to_pos(new_pos);
          grid_[new_pos.first][new_pos.second].type = SquareType::Enemy;
          log_ << "Moved enemy to position (" << new_pos.first << ';' <<  new_pos.second << ").\n";
          log_ << "Enemy at (" << new_pos.first << ';' << new_pos.second << ')';
          log_ << ", " << enemy->get_health_points() << " hp.\n";
        }
        else if (grid_[new_pos.first][new_pos.second].type == SquareType::Trap)
        {
          size_t damage = grid_[new_pos.first][new_pos.second].trap_damage;
          log_ << "Enemy moves to (" << new_pos.first << ';' << new_pos.second << ") and triggers a trap!\n";
          enemy->take_damage(damage);
          log_ << "Enemy takes " << damage << " damage. Health: " << enemy->get_health_points() << "\n";

          grid_[new_pos.first][new_pos.second].type = SquareType::Empty;
          grid_[new_pos.first][new_pos.second].trap_damage = 0;

          grid_[enemy_pos.first][enemy_pos.second].type = SquareType::Empty;
          enemy->move_to_pos(new_pos);
          grid_[new_pos.first][new_pos.second].type = SquareType::Enemy;

          if (!enemy->is_alive())
          {
            log_ << "Enemy died from the trap.\n";
            grid_[new_pos.first][new_pos.second].type = SquareType::Empty;
            it = enemies_.erase(it);

            if (enemies_.empty() && towers_.empty())
            {
              state_ = State::Won;
              log_ << "All enemies defeated! You won! ";
              log_ << "Final score: " << player_->get_score() << '\n';
              return;
            }
            continue;
          }
        }
      }
      ++it;
    }
  }
}

void game::Field::move_towers()
{
  for (auto& tower: towers_)
  {
    if (tower->is_alive())
    {
      tower->update(player_.get());
    }
    if (state_ == State::Ingame && !player_->is_alive())
    {
      state_ = State::Lost;
      log_ << "Game over! ";
      log_ << "Final score: " << player_->get_score() << "\n";
      return;
    }
  }
}

game::Enemy* game::Field::get_enemy(Position pos)
{
  for (auto& enemy: enemies_)
  {
    if (enemy->get_position() == pos)
    {
      return enemy.get();
    }
  }
  return nullptr;
}

game::EnemyTower* game::Field::get_tower(Position pos)
{
  for (auto& tower: towers_)
  {
    if (tower->get_position() == pos)
    {
      return tower.get();
    }
  }
  return nullptr;
}

void game::Field::kill_enemy(Enemy* enemy)
{
  for (auto it = enemies_.begin(); it != enemies_.end(); ++it)
  {
    if (it->get() == enemy)
    {
      enemies_.erase(it);
      break;
    }
  }
}

void game::Field::kill_tower(EnemyTower* tower)
{
  for (auto it = towers_.begin(); it != towers_.end(); ++it)
  {
    if (it->get() == tower)
    {
      towers_.erase(it);
      break;
    }
  }
}

void game::Field::update_entity_ptrs() noexcept
{
  player_->link_to_field(this);
  for (auto& enemy: enemies_)
  {
    enemy->link_to_field(this);
  }
  for (auto& tower: towers_)
  {
    tower->link_to_field(this);
  }
}
