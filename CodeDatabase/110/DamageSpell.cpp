#include "DamageSpell.hpp"

DamageSpell::DamageSpell (Field& field, size_t range, unsigned int damage, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    RangedSpell(field, range, caster, target), damage(damage) {}

unsigned int DamageSpell::getDamage () const { return damage; }

void DamageSpell::dealDamage (CastResult& result, size_t x, size_t y) {
    Entity& target = field.getCell(x, y).getEntity();
    unsigned int health = target.getCurrentHealth();
    if (health <= damage) {
        target.setCurrentHealth(0);
        result.destroyedEntities.push_back(target.getID());
        return;
    }
    target.setCurrentHealth(health - damage);
}