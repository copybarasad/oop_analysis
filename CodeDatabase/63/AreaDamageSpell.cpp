#include "AreaDamageSpell.h"
#include "EntityManager.h"
#include "GameField.h"
#include "Entity.h"

AreaDamageSpell::AreaDamageSpell(unsigned int castDiameter, unsigned int cooldown) :
    Spell(castDiameter, cooldown), diameter(2) {}

AreaDamageSpell::AreaDamageSpell(json &data) : Spell(data) {
    if (!data.contains("diameter"))
        throw std::runtime_error("Отсутствует поле diameter у заклинания урона по площади");
    if (!data["diameter"].is_number_unsigned())
        throw std::runtime_error("Значение поля diameter у заклинания урона по площади не является беззнаковым числом");
    diameter = data["diameter"].get<unsigned int>();
}

std::shared_ptr<ISpell> AreaDamageSpell::clone() const {
    return std::make_shared<AreaDamageSpell>(*this);
}

json AreaDamageSpell::to_json() const {
    json j = Spell::to_json();
    j["name"] = getName();
    j["diameter"] = diameter;
    return j;
}

std::string AreaDamageSpell::getName() const { return "Урон по площади"; }

bool AreaDamageSpell::cast(std::pair<int, int> &coordinates, Entity* caster, GameField &field, EntityManager& entityManager) {
    int enhancement = getLevel() - 1;
    int effectDiameter = diameter + enhancement;
    auto [x, y] = coordinates;
    for (int dx = -effectDiameter / 2 + !(effectDiameter % 2); dx <= effectDiameter / 2; ++dx)
        for (int dy = -effectDiameter / 2 + !(effectDiameter % 2); dy <= effectDiameter / 2; ++dy) {
            if (!field.isInBounds({x + dx, y + dy}))
                continue;
            auto& cell = field.getCell({x + dx, y + dy});
            if (cell.isOccupied()) {
                auto occupant = entityManager.findEntity({x + dx, y + dy});
                if (occupant->getTeam() == caster->getTeam()) continue;
                if (occupant->takeDamage(caster->getDamage()))
                    cell.entityDied(occupant->getPoints());
            }
        }
    used();
    return true;
}

std::pair<int,int> AreaDamageSpell::visualData() const {
    return {diameter + getLevel() - 1, getCastDiameter()};
}

bool AreaDamageSpell::allCellsInRange(std::pair<int, int> &target) const {
    int enhancement = getLevel() - 1;
    int effectDiameter = diameter + enhancement;
    int castDiameter = getCastDiameter();
    auto [x, y] = target;
    return ((-castDiameter / 2 + !(castDiameter % 2) <= x - effectDiameter / 2 + !(effectDiameter % 2)) &&
            (x + effectDiameter / 2 <= castDiameter / 2)) &&
            ((-castDiameter / 2 + !(castDiameter % 2) <= y - effectDiameter / 2 + !(effectDiameter % 2)) &&
            (y + effectDiameter / 2 <= castDiameter / 2));
}

