#pragma once

#include <set>
#include <vector>

#include "WorldTile.h"

#include "entity/EntityCompare.h"

inline constexpr double GUI_SCALE = 1.5;
inline constexpr double TILE_WIDTH = GUI_SCALE * 64;
inline constexpr double TILE_HEIGHT = GUI_SCALE * 32;
inline constexpr double TILE_GROUND_HEIGHT = GUI_SCALE * 8;
inline constexpr double TILE_SPRITE_HEIGHT = TILE_HEIGHT + TILE_GROUND_HEIGHT;


class World {
public:
    World(const int width, const int height);

    World(const World &other);

    World(World &&other) noexcept;

    World &operator=(const World &other);

    World &operator=(World &&other) noexcept;

    ~World() = default;

    int width() const { return width_; }

    int height() const { return height_; }

    bool move_entity(Entity &entity, int target_x, int target_y);

    bool add_entity(Entity *entity);

    void remove_entity(Entity *entity);

    bool has_entity(Entity *entity) const;

    bool is_valid_tile(int x, int y) const;

    bool is_passable(int x, int y) const;

    int get_screen_x(double tile_x, double tile_y) const;

    int get_screen_y(double tile_x, double tile_y) const;

    const WorldTile &get_tile(int x, int y) const;

    WorldTile &get_tile(int x, int y);

    double get_tile_x_fract(int screen_x, int screen_y) const;

    double get_tile_y_fract(int screen_x, int screen_y) const;

    int get_tile_x(int screen_x, int screen_y) const;

    int get_tile_y(int screen_x, int screen_y) const;

    int get_tile_height_offset(int tile_x, int tile_y) const;

    void get_camera(float& x, float& y) const {
        x = camera_x_;
        y = camera_y_;
    }

    void set_camera(const float& x, const float& y) {
        camera_x_ = x;
        camera_y_ = y;
    }






    const std::set<Entity *, EntityCompare> &entities() const {
        return entities_;
    }

    std::pair<int, int> pick_random_safe_pos() const;

    std::pair<int, int> pick_random_adjacent_pos(int x, int y) const;

    std::pair<int, int> get_path_pos(int x, int y, int to_x, int to_y) const;

    void randomize_tiles(const std::unordered_map<TileType, int> &tile_distribution);

private:
    int width_;
    int height_;
    float camera_x_ = -1;
    float camera_y_ = -1;
    std::vector<WorldTile> tiles_;
    std::set<Entity *, EntityCompare> entities_;

};
