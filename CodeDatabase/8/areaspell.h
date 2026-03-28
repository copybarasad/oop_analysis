#pragma once
#include "basespell.h"

class AreaSpell : public BaseSpell {
public:
    using BaseSpell::BaseSpell;
    bool do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) override;
};