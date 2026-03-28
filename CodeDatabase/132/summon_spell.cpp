#include "summon_spell.hpp"
#include "field_handler.hpp"

SummonSpell::SummonSpell(): SpawnSpell('A') {};

bool SummonSpell::use(FieldHandler& field_handler, const Position& from, const Position& target) {
    bool used = false;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (
                field_handler.get_field().in_bounds({ i, j }) && 
                Position::manhattan_distance({ i, j }, from) == 1
            )
            {
                Ally* al = new Ally();
                field_handler.place_obj({i, j}, al);
                lvl--;
                used = true;
            }

            if (!lvl) break;
        }
    }

    return used;
}