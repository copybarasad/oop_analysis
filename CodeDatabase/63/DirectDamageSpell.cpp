#include "DirectDamageSpell.h"
#include "EntityManager.h"
#include "GameField.h"
#include "Entity.h"

DirectDamageSpell::DirectDamageSpell(unsigned int castDiameter, unsigned int cooldown) : Spell(castDiameter, cooldown) {}

DirectDamageSpell::DirectDamageSpell(json& data) : Spell(data) {}

std::shared_ptr<ISpell> DirectDamageSpell::clone() const {
    return std::make_shared<DirectDamageSpell>(*this);
}

json DirectDamageSpell::to_json() const {
    json j = Spell::to_json();
    j["name"] = getName();
    return j;
}

bool DirectDamageSpell::canHitTarget(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) {
    if (!field.isInBounds(coordinates))
        return false;
    const auto& cell = field.getCell(coordinates);
    if (!cell.isOccupied())
        return false;
    auto target = entityManager.findEntity(coordinates);
    return target->getTeam() != caster->getTeam();
}

std::string DirectDamageSpell::getName() const { return "Прямой урон"; }

bool DirectDamageSpell::cast(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) {
    int enhancement = getLevel() - 1;

    int effectiveRange = getCastDiameter() + enhancement;
    if (!field.areCoordinatesInArea(coordinates, caster->getPosition(), effectiveRange))
        return false;

    if (!canHitTarget(coordinates, caster, field, entityManager))
        return false;

    auto& cell = field.getCell(coordinates);
    auto target = entityManager.findEntity(coordinates);

    int damage = caster->getDamage() * 2;
    if (target->takeDamage(damage))
        cell.entityDied(target->getPoints());
    used();
    return true;
}

int DirectDamageSpell::getCastDiameter() const {
    return Spell::getCastDiameter() + getLevel() - 1;
}
