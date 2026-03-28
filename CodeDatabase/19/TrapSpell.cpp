#include "TrapSpell.h"

#include "core/Game.h"
#include "entity/types/Player.h"
#include "entity/types/Trap.h"
#include "world/World.h"

bool TrapSpell::cast(GameLogic &logic, Entity &caster, int mouse_x, int mouse_y, int level) {
    const World &world = logic.world();

    const int tile_x = world.get_tile_x(mouse_x, mouse_y);
    const int tile_y = world.get_tile_y(mouse_x, mouse_y);

    if (!is_valid_target(logic.world(), caster, tile_x, tile_y)) return false;


    logic.add_entity(new Trap(tile_x, tile_y, caster.is_hostile(), level * 5));


    return true;
}

void TrapSpell::render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x, int mouse_y, int level) {
    int x = world.get_tile_x(mouse_x, mouse_y);
    int y = world.get_tile_y(mouse_x, mouse_y);

    if (!world.is_valid_tile(x, y)) return;

    std::string texture = is_valid_target(world, player, x, y) ? "tile_selection" : "invalid_tile_selection";

    renderer.depth_render(x + y + 0.1, [=, this](const IRenderer &r) {
        r.draw_texture_centered(texture, world.get_screen_x(x, y), world.get_screen_y(x, y), TILE_WIDTH);
    });
}

bool TrapSpell::is_valid_target(const World &world, const Entity &caster, int tile_x, int tile_y) const {
    if (world.is_valid_tile(tile_x, tile_y)) {
        const WorldTile &tile = world.get_tile(tile_x, tile_y);
        return tile.can_go();
    }
    return false;
}
