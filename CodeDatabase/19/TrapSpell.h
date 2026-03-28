#pragma once
#include "core/render/renderer/IRenderer.h"
#include "spell/Spell.h"

class TrapSpell : public Spell {
public:
    std::string id() override { return "trap"; }
    bool cast(GameLogic &logic, Entity& caster, int mouse_x, int mouse_y, int level) override;
    void render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x, int mouse_y, int level) override;
    bool is_valid_target(const World& world, const Entity& caster, int tile_x, int tile_y) const;

    ~TrapSpell() override = default;
};
