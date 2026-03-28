#ifndef FIELD_HPP
#define FIELD_HPP

#include <vector>
#include <memory>
#include <iostream>
#include "entity-traits.hpp"

namespace game
{
  class Player;
  class Enemy;
  class EnemyTower;

  constexpr unsigned short MIN_FIELD_LENGTH = 10;
  constexpr unsigned short MAX_FIELD_LENGTH = 25;
  constexpr unsigned short MIN_FIELD_WIDTH = 10;
  constexpr unsigned short MAX_FIELD_WIDTH = 25;

  static constexpr std::ostream& INITIAL_LOG_STREAM = std::cout;

  enum class State
  {
    Ingame,
    Won,
    Lost,
    None
  };

  class Field
  {
  public:
    Field();
    explicit Field(size_t rows);
    Field(size_t rows, size_t cols);
    Field(const Field& rhs);
    Field(Field&& rhs) noexcept;
    ~Field() = default;

    Field& operator=(const Field& rhs);
    Field& operator=(Field&& rhs) noexcept;

    void swap(Field& other) noexcept;
    void summon_player(const PlayerTraits& traits, Position pos);
    void summon_enemy(const EnemyTraits& traits, Position pos);
    void summon_tower(const EnemyTowerTraits& traits, Position pos);
    void make_turn(int dx, int dy);
    void buy_spell_card(const std::string& spell_name);
    void use_spell_card(size_t index);
    void use_spell_card(size_t index, Position pos);
    void reset();
    bool is_valid_position(Position pos) const noexcept;
    SquareType get_square_type(Position pos) const noexcept;
    void set_square_type(SquareType type, Position pos, size_t trap_damage = 0) noexcept;
    std::pair< size_t, size_t > get_grid_size() const noexcept;

    Player& get_player();
    const Player& get_player() const;
    const std::vector< std::unique_ptr< Enemy > >& get_enemies_ptrs() const;
    const std::vector< std::unique_ptr< EnemyTower > >& get_towers_ptrs() const;

    Enemy* get_enemy(Position pos);
    EnemyTower* get_tower(Position pos);
    void kill_enemy(Enemy* enemy);
    void kill_tower(EnemyTower* tower);
    State get_state() const noexcept;
    void set_state(State state) noexcept;
    std::ostream& get_log_stream() noexcept;

  private:
    std::vector< std::vector< Square > > grid_;
    std::unique_ptr< Player > player_;
    std::vector< std::unique_ptr< Enemy > > enemies_;
    std::vector< std::unique_ptr< EnemyTower > > towers_;

    State state_ = State::None;
    std::ostream& log_ = INITIAL_LOG_STREAM;

    void move_player(Position pos);
    void move_enemies();
    void move_towers();

    void update_entity_ptrs() noexcept;
  };
}

#endif
