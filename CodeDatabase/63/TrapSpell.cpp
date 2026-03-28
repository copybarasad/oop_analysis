#include "TrapSpell.h"
#include "EntityManager.h"
#include "GameField.h"
#include "Trap.h"


TrapSpell::TrapSpell(unsigned int castDiameter, unsigned int cooldown) :
    Spell(castDiameter, cooldown) {}

TrapSpell::TrapSpell(json &data) : Spell(data) {}

std::shared_ptr<ISpell> TrapSpell::clone() const {
    return std::make_shared<TrapSpell>(*this);
}

json TrapSpell::to_json() const {
    json j = Spell::to_json();
    j["name"] = getName();
    return j;
}

std::string TrapSpell::getName() const { return "Мимик (ловушка)"; }

bool TrapSpell::cast(std::pair<int, int> &coordinates, Entity* caster, GameField &field, EntityManager& entityManager) {
    int enhancement = getLevel() - 1;
    if (!canBePlaced(coordinates, field, entityManager)) return false;
    int trapDamage = caster->getDamage() * 2 + enhancement * 20;
    auto* trap = entityManager.addEntity(Trap(trapDamage, caster->getTeam()));
    trap->setPosition(coordinates);
    trap->addPoints(field.getCell(coordinates).entityPlaced());
    used();
    return true;
}

std::pair<int,int> TrapSpell::visualData() const {
    return {1, getCastDiameter()};
}

bool TrapSpell::canBePlaced(std::pair<int, int> &coordinates, GameField &field, EntityManager& entityManager) const {
    if (!field.isInBounds(coordinates)) return false;
    auto& cell = field.getCell(coordinates);
    if (!cell.isPassable() || cell.isOccupied()) return false;
    return true;
}