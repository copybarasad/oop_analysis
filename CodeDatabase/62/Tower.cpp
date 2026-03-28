#include "Tower.h"
#include "Spell.h"
#include "GameField.h"

Tower::Tower(Spell* spell, std::function<void(int, int, int, float)> applyDamage, int turns_to_attack, int radius,
    int health) : Building(),
    spell_(spell),
    applyDamage_(applyDamage),
    radius(radius),
    turns_to_attack(turns_to_attack),
    lasted_turns(turns_to_attack - 1),
    health_(health),
    max_health_(health)
{}

void Tower::makeTurn(
    GameField* field
) {
    lasted_turns++;
    if(lasted_turns == turns_to_attack) {
        spell_->use(*field, this);
        lasted_turns = 0;
    }
}