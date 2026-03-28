#pragma once
#include <vector>

#include "entity/types/Player.h"
#include "spell/Spell.h"

class AOEDamageSpell : public Spell {
public:
    std::string id() override { return "aoe_damage"; }
    bool cast(GameLogic &logic, Entity& caster, int mouse_x, int mouse_y, int level) override;
    void render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x, int mouse_y, int level) override;

    ~AOEDamageSpell() override = default;

private:
    int get_size(int level) const;
    bool is_in_range(const Entity& caster, std::vector<std::pair<int, int>> positions) const;

    std::vector<std::pair<int, int>> get_affected_tiles(const World& world, int mouse_x, int mouse_y, int size) const;
};