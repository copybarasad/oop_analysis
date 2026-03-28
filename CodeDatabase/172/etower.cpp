#include "etower.h"
#include "field.h"
#include "player.h"
#include "helpers.h"
#include "direct_damage_spell.h"

namespace game {
    void ETower::tick(Field& field) {
        if (cd_ > 0) {
            cd_--;
            return;
        }

        if (spell_.castFromTower(field, *this)) cd_ = cdLen_;
    }
}