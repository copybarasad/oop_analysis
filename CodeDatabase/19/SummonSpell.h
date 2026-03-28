#pragma once
#include "core/render/renderer/IRenderer.h"
#include "spell/Spell.h"

class SummonSpell : public Spell {
public:
    std::string id() override { return "summon"; }
    bool cast(GameLogic &logic, Entity& caster, int mouse_x, int mouse_y, int level) override;
    void render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x, int mouse_y, int level) override;

    ~SummonSpell() override = default;

};
