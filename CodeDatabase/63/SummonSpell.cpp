#include "SummonSpell.h"
#include "EntityManager.h"
#include "GameField.h"
#include "Ally.h"
#include "Enemy.h"

SummonSpell::SummonSpell(unsigned int castDiameter, unsigned int cooldown) : Spell(castDiameter, cooldown) {}

SummonSpell::SummonSpell(json &data) : Spell(data) {}

std::shared_ptr<ISpell> SummonSpell::clone() const {
    return std::make_shared<SummonSpell>(*this);
}

json SummonSpell::to_json() const {
    json j = Spell::to_json();
    j["name"] = getName();
    return j;
}

std::string SummonSpell::getName() const { return "Призыв союзников"; }

bool SummonSpell::cast(std::pair<int,int> &coordinates, Entity* caster, GameField &field, EntityManager& entityManager) {
    if (!canBePlaced(coordinates, field, entityManager))
        return false;

    std::vector<std::pair<int,int>> candidates;
    for (int r = 0; r <= getLevel(); ++r) {
        for (int dx = -r; dx <= r; ++dx) {
            for (int dy = -r; dy <= r; ++dy) {
                auto pos = std::pair<int,int>{coordinates.first + dx, coordinates.second + dy};
                if (canBePlaced(pos, field,  entityManager))
                    candidates.push_back(pos);
            }
        }
    }

    if (candidates.empty()) return false;

    int placed = 0;
    for (auto& pos : candidates) {
        if (placed >= getLevel()) break;
        Entity* entity;
        if (caster->getTeam() == EntityTeam::ALLY)
            entity = entityManager.addEntity(Ally(caster->getMaxHealth() / 2, caster->getDamage() / 2, caster->getViewDiameter()));
        else
            entity = entityManager.addEntity(Enemy(caster->getMaxHealth() / 2, caster->getDamage() / 2, caster->getViewDiameter()));
        entity->setPosition(pos);
        entity->addPoints(field.getCell(pos).entityPlaced());
        placed++;
    }
    if (placed == 0) return false;
    used();
    return true;
}

std::pair<int,int> SummonSpell::visualData() const {
    return {1, getCastDiameter()};
}

bool SummonSpell::canBePlaced(std::pair<int, int> &coordinates, GameField &field, EntityManager& entityManager) const {
    if (!field.isInBounds(coordinates)) return false;
    auto& cell = field.getCell(coordinates);
    if (cell.isOccupied() || !cell.isPassable()) return false;
    return true;
}
