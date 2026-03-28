#pragma once
#include <vector>

#include "core/render/renderer/IRenderer.h"
#include "spell/Spell.h"

class DamageSpell : public Spell {
public:
    std::string id() override { return "damage"; }

    bool cast(GameLogic &logic, Entity &caster, int mouse_x, int mouse_y, int level) override;

    void render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x, int mouse_y, int level) override;

    bool is_valid_target(const World &world, const Entity &player, int tile_x, int tile_y, int level) const;

    ~DamageSpell() override = default;
};
