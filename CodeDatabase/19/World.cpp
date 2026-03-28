#include <random>
#include "World.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <utility>



World::World(const int width, const int height) : width_(width), height_(height), tiles_(width * height) {
}


World::World(const World &other)
    : width_(other.width_), height_(other.height_), tiles_(other.tiles_), entities_(other.entities_), camera_x_(other.camera_x_), camera_y_(other.camera_y_) {
}


World::World(World &&other) noexcept
    : width_(other.width_), height_(other.height_), tiles_(std::move(other.tiles_)),
      entities_(std::move(other.entities_)), camera_x_(other.camera_x_), camera_y_(other.camera_y_) {
    other.width_ = 0;
    other.height_ = 0;
}

World &World::operator=(const World &other) {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    tiles_ = other.tiles_;
    entities_ = other.entities_;
    return *this;
}

World &World::operator=(World &&other) noexcept {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    tiles_ = std::move(other.tiles_);
    entities_ = std::move(other.entities_);
    other.width_ = 0;
    other.height_ = 0;
    return *this;
}

WorldTile &World::get_tile(const int x, const int y) {
    return tiles_[y * width_ + x];
}

const WorldTile &World::get_tile(const int x, const int y) const {
    return tiles_[y * width_ + x];
}

bool World::move_entity(Entity &entity, const int target_x, const int target_y) {
    if (!has_entity(&entity)) {
        return false;
    }
    if (!is_valid_tile(target_x, target_y)) {
        return false;
    }

    if (entity.x() == target_x && entity.y() == target_y) {
        return false;
    }
    auto &tile = get_tile(target_x, target_y);

    if (!tile.can_go()) {
        return false;
    }


    auto &old_tile = get_tile(entity.x(), entity.y());


    old_tile.entity_ = nullptr;
    tile.entity_ = &entity;
    entity.set_x(target_x);
    entity.set_y(target_y);

    return true;
}

bool World::add_entity(Entity *entity) {
    if (!is_valid_tile(entity->x(), entity->y())) {
        return false;
    }
    if (has_entity(entity)) {
        return false;
    }
    auto &tile = get_tile(entity->x(), entity->y());
    if (tile.entity_ != nullptr) {
        return false;
    }
    if (!tile.can_go()) {
        return false;
    }
    entities_.insert(entity);
    tile.entity_ = entity;

    return true;
}

void World::remove_entity(Entity *entity) {
    if (!has_entity(entity)) {
        return;
    }
    entities_.erase(entity);
    if (is_valid_tile(entity->x(), entity->y())) {
        get_tile(entity->x(), entity->y()).entity_ = nullptr;
    }
}

bool World::has_entity(Entity *entity) const {
    if (entity == nullptr) return false;
    return entities_.contains(entity);
}

bool World::is_valid_tile(const int x, const int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool World::is_passable(int x, int y) const {
    return is_valid_tile(x, y) && get_tile(x, y).can_go();
}


int World::get_screen_x(const double tile_x, const double tile_y) const {
    int x = (tile_x - tile_y) * (TILE_WIDTH / 2);
    x += TILE_WIDTH * (std::max(width(), height()) / 2.0 + 1.0);
    return x - camera_x_;
}

int World::get_screen_y(const double tile_x, const double tile_y) const {
    int y = (tile_x + tile_y) * (TILE_HEIGHT / 2);
    y += TILE_HEIGHT * 1.5;
    return y - camera_y_;
}

double World::get_tile_x_fract(int screen_x, int screen_y) const {
    screen_x+=camera_x_;
    screen_y+=camera_y_;

    const double offset_x = TILE_WIDTH * (std::max(width(), height()) / 2.0 + 1);
    const double offset_y = TILE_HEIGHT * 1.5;

    const double rel_x = static_cast<double>(screen_x) - offset_x;
    const double rel_y = static_cast<double>(screen_y) - offset_y;

    const double tile_x = (rel_x / (TILE_WIDTH / 2.0) + rel_y / (TILE_HEIGHT / 2.0)) / 2.0;
    return tile_x + 0.5;
}

double World::get_tile_y_fract(int screen_x, int screen_y) const {
    screen_x+=camera_x_;
    screen_y+=camera_y_;

    const double offset_x = TILE_WIDTH * (std::max(width(), height()) / 2.0 + 1);
    const double offset_y = TILE_HEIGHT * 1.5;

    const double rel_x = static_cast<double>(screen_x) - offset_x;
    const double rel_y = static_cast<double>(screen_y) - offset_y;

    const double tile_y = (rel_y / (TILE_HEIGHT / 2.0) - rel_x / (TILE_WIDTH / 2.0)) / 2.0;
    return tile_y + 0.5;
}


int World::get_tile_x(int screen_x, int screen_y) const {
    return static_cast<int>(std::floor(get_tile_x_fract(screen_x, screen_y)));
}

int World::get_tile_y(int screen_x, int screen_y) const {
    return static_cast<int>(std::floor(get_tile_y_fract(screen_x, screen_y)));
}

#include <cmath>

int World::get_tile_height_offset(int tile_x, int tile_y) const {
    TileType type = get_tile(tile_x, tile_y).type();
    if (type == TileType::Lava) return 4;
    return 0;
}



std::pair<int, int> World::pick_random_safe_pos() const {
    std::vector<std::pair<int, int>> available;

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            if (get_tile(x,y).can_go()) {
                available.emplace_back(x,y);
            }
        }
    }
    if (available.empty()) {
        return {-1, -1};
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, available.size() - 1);

    return available.at(dist(gen));
}

std::pair<int, int> World::pick_random_adjacent_pos(int x, int y) const {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dir(0, 3);
    int new_x;
    int new_y;
    do {
        new_x = x;
        new_y = y;
        switch (dir(gen)) {
            case 0:
                new_x++;
                break;
            case 1:
                new_x--;
                break;
            case 2:
                new_y++;
                break;
            case 3:
                new_y--;
                break;
            default:
                break;
        }
    } while (!is_valid_tile(new_x, new_y));

    return {new_x, new_y};
}

std::pair<int, int> World::get_path_pos(int x, int y, int to_x, int to_y) const {
    if (x == to_x && y == to_y) return {x, y};


    constexpr int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    using Tile = WorldTile;
    const Tile *startT = &get_tile(x, y);
    const Tile *goalT = &get_tile(to_x, to_y);

    std::queue<std::pair<int, int> > q;
    std::unordered_set<const Tile *> visited;
    std::unordered_map<const Tile *, std::pair<int, int> > parent;

    q.push({x, y});
    visited.insert(startT);
    parent[startT] = {x, y};

    bool found = false;


    auto passable = [&](int x, int y) -> bool {
        if (x == to_x && y == to_y)
            return is_valid_tile(x, y);
        return is_passable(x, y);
    };

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        const Tile *curT = &get_tile(cx, cy);

        if (curT == goalT) {
            found = true;
            break;
        }

        for (auto &d: dirs) {
            int nx = cx + d[0];
            int ny = cy + d[1];
            if (!passable(nx, ny)) continue;

            const Tile *nxtT = &get_tile(nx, ny);
            if (visited.contains(nxtT)) continue;

            visited.insert(nxtT);
            parent[nxtT] = {cx, cy};
            q.push({nx, ny});
        }
    }

    if (!found) return {x, y};

    std::pair<int, int> step = {to_x, to_y};
    const Tile *stepT = goalT;

    while (true) {
        auto it = parent.find(stepT);
        if (it == parent.end()) break;
        auto p = it->second;
        if (p.first == x && p.second == y) {
            return step;
        }
        step = p;
        stepT = &get_tile(step.first, step.second);
    }

    return {x, y};
}

void World::randomize_tiles(const std::unordered_map<TileType, int>& tile_distribution) {
    auto remaining = tile_distribution;

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x)
            get_tile(x, y).set_type(TileType::Ground);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist_w(1, width_ - 2);
    std::uniform_int_distribution<int> dist_h(1, height_ - 2);

    int total_special_tiles = 0;
    for (const auto& [type, count] : remaining)
        total_special_tiles += count;

    int limit = (width_ - 2) * (height_ - 2) / 2;

    while (total_special_tiles > 0 && limit-- > 0) {
        int x = dist_w(gen);
        int y = dist_h(gen);

        if (tiles_[y * width_ + x].type() != TileType::Ground)
            continue;

        for (auto& [type, count] : remaining) {
            if (count > 0) {
                tiles_[y * width_ + x].set_type(type);
                --count;
                --total_special_tiles;
                break;
            }
        }
    }
}

