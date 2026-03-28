#include "ISpell.hpp"

class UpgradeSpell : public ISpell {
public:
    UpgradeSpell();

    bool use(FieldHandler& field_handler, const Position& from, const Position& target) override;
    int apply_enhancement(int current_lvl) override;
};