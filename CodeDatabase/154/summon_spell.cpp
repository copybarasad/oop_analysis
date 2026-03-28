#include "summon_spell.h"
#include "field.h"
#include "enemy.h"
#include "ally.h"

SummonSpell::SummonSpell(int allyHealth, int allyDamage, int range, int quantity)
: allyHealth_(allyHealth), allyDamage_(allyDamage), range_(range), quantity_(quantity) {}

bool SummonSpell::apply(int casterId, Field& field, int targetRow, int targetCol) {
    int cr, cc;
    if (!field.getPosition(casterId, cr, cc)) return false;
    Player* pl = field.getPlayerById(casterId);
    int effQuantity = quantity_;
    if (pl) effQuantity += pl->consumeExtraSummons();
    if (field.manhattanDistance(cr, cc, targetRow, targetCol) > range_) return false;
    int placed = 0;
    for (int dr = -1; dr <= 1 && placed < effQuantity; ++dr) {
        for (int dc = -1; dc <= 1 && placed < effQuantity; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int r = cr + dr;
            int c = cc + dc;
            if (!field.inBounds(r,c)) continue;
            if (!field.cellIsEmpty(r,c)) continue;
            int id = field.generateEntityId();
            Ally* ally = new Ally(id, allyHealth_, allyDamage_);
            field.placeEntity(ally, r, c);
            ++placed;
        }
    }
    return placed > 0;
}

std::string SummonSpell::getName() const { return "Summon"; }
