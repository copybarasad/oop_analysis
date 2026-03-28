#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Cell.hpp"
#include "Enemy.hpp"
#include "EnemyNest.hpp"
#include "Player.hpp"
#include "Position.hpp"

// Настройка игры для начала уровня.
struct GameParams {
    int hero_hp{10};
    int hero_melee_dmg{4};
    int hero_ranged_dmg{3};
    int enemy_hp{5};
    int enemy_atk{1};
    int enemy_count{2};
    int nest_count{2};
};

// Моментальный снимок поля для сохранения/загрузки.
struct FieldState {
    int w{0};
    int h{0};
    std::vector<Cell> cells;
    int enemy_hp{0};
    int enemy_atk{0};
    Player player;
    std::vector<Enemy> enemies;
    std::vector<EnemyNest> nests;
    int kill_count{0};
};

class Field {
    int w_{10}, h_{10};
    std::vector<Cell> cells_;

    int enemy_hp_{5};
    int enemy_atk_{1};

    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<EnemyNest> nests_;
    int kill_count_{0};

    int idx(int x, int y) const { return y * w_ + x; }
    bool inside(int x, int y) const { return x >= 0 && y >= 0 && x < w_ && y < h_; }
    std::optional<Pos> find_free_adjacent_(Pos around) const;
    bool give_random_spell_();
    void give_initial_spell_();

public:
    Field(int w, int h, const GameParams& params = GameParams());
    explicit Field(const FieldState& state);

    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&&) noexcept = default;
    Field& operator=(Field&&) noexcept = default;

    int width() const { return w_; }
    int height() const { return h_; }

    bool passable(int x, int y) const { return inside(x, y) && cells_[idx(x, y)] != Cell::Wall; }
    bool is_slow(int x, int y) const { return inside(x, y) && cells_[idx(x, y)] == Cell::Slow; }
    Cell get(int x, int y) const { return cells_[idx(x, y)]; }
    void set(int x, int y, Cell c) { cells_[idx(x, y)] = c; }

    Player& player() { return player_; }
    const Player& player() const { return player_; }

    std::vector<Enemy>& enemies() { return enemies_; }
    const std::vector<Enemy>& enemies() const { return enemies_; }

    std::vector<EnemyNest>& nests() { return nests_; }
    const std::vector<EnemyNest>& nests() const { return nests_; }

    bool enemy_at(int x, int y) const;
    bool nest_at(int x, int y) const;
    bool damage_enemy_at(int x, int y, int dmg);
    bool damage_nest_at(int x, int y, int dmg);
    std::optional<Pos> find_target_in_radius(Pos center, int radius) const;

    void tick_nests_and_spawn();

    std::string render() const;
    FieldState snapshot() const;
};
