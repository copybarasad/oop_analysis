#include "ImproveSpell.h"
#include "Player.h"
#include "Hand.h"

bool ImproveSpell::cast(int, Entity*, ISpellField* field) {
    Hand& hand = field->getPlayer().getHand();
    hand.setPendingBoostMultiplier(hand.getPendingBoostMultiplier() + 1);
    return true;
}



