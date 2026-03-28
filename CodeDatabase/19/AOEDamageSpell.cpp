#include "AOEDamageSpell.h"
#include "world/WorldTile.h"
#include "core/Game.h"

bool AOEDamageSpell::cast(GameLogic &logic, Entity &caster, int mouse_x, int mouse_y, int level) {
    auto &world = logic.world();

    const int size = get_size(level);

    std::vector<std::pair<int, int> > pos = get_affected_tiles(logic.world(), mouse_x, mouse_y, size);


    if (!is_in_range(caster, pos)) return false;

    const int dmg = 6;

    for (auto &[x,y]: pos) {
        logic.particle_helper().spawn_lava_effect(x,y);
        const WorldTile &tile = world.get_tile(x, y);
        if (auto *e = tile.entity()) {
            if (!e->is_ally(caster)) {
                logic.damage_entity(e, dmg);
            }
        }
    }


    return true;
}

void AOEDamageSpell::render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x,
                                    int mouse_y, int level) {
    std::vector<std::pair<int, int> > pos = get_affected_tiles(world, mouse_x, mouse_y, get_size(level));

    bool in_range = is_in_range(player, pos);
    std::string texture = in_range ? "tile_selection" : "invalid_tile_selection";

    for (const auto &p : pos) {
        int x = p.first;
        int y = p.second;
        renderer.depth_render(x + y + 0.1, [=, this](const IRenderer &r) {
            r.draw_texture_centered(texture, world.get_screen_x(x, y), world.get_screen_y(x, y), TILE_WIDTH);
        });
    }
}

int AOEDamageSpell::get_size(int level) const {
    if (level < 1) level = 1;
    return level + 1;
}

bool AOEDamageSpell::is_in_range(const Entity &caster, std::vector<std::pair<int, int> > positions) const {
    bool in_range = false;
    for (auto &[x,y]: positions) {
        if (caster.manh_distance(x, y) <= 2) {
            in_range = true;
            break;
        }
    }
    return in_range;
}

std::vector<std::pair<int, int> > AOEDamageSpell::get_affected_tiles(const World &world, const int mouse_x,
                                                                     const int mouse_y, const int size) const {
    std::vector<std::pair<int, int> > tiles;
    if (size <= 0) return tiles;

    int tile_x = world.get_tile_x(mouse_x, mouse_y);
    int tile_y = world.get_tile_y(mouse_x, mouse_y);
    int radius = size / 2;
    if (size % 2 == 1) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int x = tile_x + dx;
                int y = tile_y + dy;
                if (world.is_valid_tile(x, y))
                    tiles.emplace_back(x, y);
            }
        }
    } else {
        if (std::fmod(world.get_tile_x_fract(mouse_x, mouse_y), 1) > 0.5) {
            tile_x += 1;
        }
        if (std::fmod(world.get_tile_y_fract(mouse_x, mouse_y), 1) > 0.5) {
            tile_y += 1;
        }
        tile_x -= radius;
        tile_y -= radius;

        for (int dx = 0; dx < size; ++dx) {
            for (int dy = 0; dy < size; ++dy) {
                int x = tile_x + dx;
                int y = tile_y + dy;
                if (world.is_valid_tile(x, y))
                    tiles.emplace_back(x, y);
            }
        }
    }

    return tiles;
}
