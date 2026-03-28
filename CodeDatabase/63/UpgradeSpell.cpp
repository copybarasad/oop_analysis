#include "UpgradeSpell.h"
#include "Player.h"

UpgradeSpell::UpgradeSpell(unsigned int cooldown) : Spell(0, cooldown), target(-1) {}

UpgradeSpell::UpgradeSpell(json &data) : Spell(data), target(-1) {}

std::shared_ptr<ISpell> UpgradeSpell::clone() const {
    return std::make_shared<UpgradeSpell>(*this);
}

json UpgradeSpell::to_json() const {
    json data = Spell::to_json();
    data["name"] = getName();
    return data;
}

std::string UpgradeSpell::getName() const { return "Улучшение"; }

bool UpgradeSpell::cast(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) {
    auto* player = dynamic_cast<Player*>(caster);
    if (!player) return false;

    auto& hand = player->getSpellsHand();
    auto spells = hand.getSpells();
    if (target < 0 || spells.size() <= target) return false;

    used();
    auto* spell = spells[target].get();
    if (this == spell) {
        spell->setLevel(spell->getLevel() + 1);
    } else {
        spell->setLevel(spell->getLevel() + getLevel());
        setLevel(1);
    }
    return true;

}

void UpgradeSpell::setTarget(int index) { target = index; }

std::pair<int,int> UpgradeSpell::visualData() const {
    return {1, getCastDiameter()};
}
