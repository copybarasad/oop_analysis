#include "DamageSpell.h"
#include "entity/types/Player.h"
#include "core/Game.h"

bool DamageSpell::cast(GameLogic &logic, Entity &caster, int mouse_x, int mouse_y, int level) {
    const World &world = logic.world();

    int tile_x = world.get_tile_x(mouse_x, mouse_y);
    int tile_y = world.get_tile_y(mouse_x, mouse_y);

    if (!is_valid_target(world, caster, tile_x, tile_y, level)) return false;

    bool weaken = caster.is_hostile();

    const WorldTile &tile = logic.world().get_tile(tile_x, tile_y);

    if (tile.entity() != nullptr) {
        logic.damage_entity(tile.entity(), weaken ? 3 : 10);
        logic.particle_helper().spawn_projectile("p_lava", 0.15, 15, caster.x() - 0.3, caster.y() - 0.3, tile_x, tile_y);
    }

    return true;
}

void DamageSpell::render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x,
                                 int mouse_y, int level) {
    int x = world.get_tile_x(mouse_x, mouse_y);
    int y = world.get_tile_y(mouse_x, mouse_y);

    if (!world.is_valid_tile(x, y)) return;
    std::string texture = is_valid_target(world, player, x, y, level) ? "tile_selection" : "invalid_tile_selection";


    renderer.depth_render(x + y + 0.1, [=, this](const IRenderer &r) {
        r.draw_texture_centered(texture, world.get_screen_x(x, y), world.get_screen_y(x, y), TILE_WIDTH);
    });
}

bool DamageSpell::is_valid_target(const World &world, const Entity &caster, const int tile_x, const int tile_y,
                                  const int level) const {
    return caster.cheb_distance(tile_x, tile_y) <= level
           && world.is_valid_tile(tile_x, tile_y)
           && world.get_tile(tile_x, tile_y).occupied()
           && !world.get_tile(tile_x, tile_y).entity()->is_ally(caster);
}
