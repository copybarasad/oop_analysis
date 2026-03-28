#pragma once
#include "spells/areaSpell.h"
#include "spells/directSpell.h"
#include "spells/callSpell.h"
#include "spells/trapSpell.h"
#include "spells/improveSpell.h"
#include "field/field.h"
#include "characters/ally.h"


class EnemyManager;
class InputManager;

class SpellManager{
    private:
        int trapDamage_;
        std::vector<std::unique_ptr<TrapSpell>> traps_;
        std::vector<std::unique_ptr<Ally>> allies_;

        void setZeroUsed(Field &field);

        public : 
        
            SpellManager();
            SpellManager(const SpellManager &) = delete;
            SpellManager &operator=(const SpellManager &) = delete;

            SpellManager(SpellManager &&) = default;
            SpellManager &operator=(SpellManager &&) = default;


            void cast(ISpellCard &base, Field &field, EnemyManager &enM,  Position target);
            void cast(AreaSpell &spell, Field &field, EnemyManager &enM,  Position target);
            void cast(DirectSpell &spell, Field &field, EnemyManager &enM,  Position target);
            void cast(TrapSpell &spell, Field &field, EnemyManager &enM,  Position target);
            void cast(CallSpell &spell, Field &field, EnemyManager &enM,  Position target);
            void cast(ImproveSpell &spell, Field &field, EnemyManager &enM, Hand *hand, int opt); 

            int getTrapDamage();
            void deliteTrap(Position pos);

            void allyMove(Field &field, EnemyManager &enM);
            const std::vector<std::unique_ptr<Ally>>& getAlly() const;

            void addAlly(std::unique_ptr<Ally> al);
            void cleanAlly();
};