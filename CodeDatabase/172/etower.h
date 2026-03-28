#pragma once
#include "pos.h"
#include "direct_damage_spell.h"

namespace game {
    class Field;

    class ETower final {
        private:
            Pos pos_;
            int cd_;
            int cdLen_;
            DirectDamageSpell spell_;
            
        public:
            ETower() : pos_(8, 1), cd_(0), cdLen_(1), spell_(DirectDamageSpell(0, 0)) {}
            ETower(Pos p, int dmg, int radius, int cooldownLen = 1)
                : pos_(p), cd_(0), cdLen_(cooldownLen), spell_(dmg, radius) {}

            Pos getPosition()       const { return pos_; }
            int getCooldownLeft()   const { return cd_;}

            void setPosition(Pos p)      { pos_ = p; }
            void setCooldownLeft(int cd) { cd_ = cd; }

            void tick(Field& f);
    };
}