#include <Gauntlet.hpp>
#include <Constants.hpp> 
#include <random>
#include <SiegeArrow.hpp>
#include <RainOfArrows.hpp>
#include <Player.hpp>
#include <SummonDecoySpell.hpp>
#include <EmpowerSpell.hpp>
#include <algorithm>

static const std::vector<SpellType> PLAYER_LEARNABLE_SPELLS = {
    SpellType::SiegeArrow,
    SpellType::RainOfArrows,
    SpellType::PlaceTrap,
    SpellType::SummonDecoy,
    SpellType::Empower
};

Gauntlet::Gauntlet() : maxSpells(MAX_SPELLS) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(0, PLAYER_LEARNABLE_SPELLS.size() - 1);
    int randomSpellIndex = distrib(gen);

    SpellType randomSpell = PLAYER_LEARNABLE_SPELLS[randomSpellIndex];

    switch (randomSpell) {
        case SpellType::SiegeArrow:
            learnSpell(std::make_shared<SiegeArrow>());
            break;
        case SpellType::RainOfArrows:
            learnSpell(std::make_shared<RainOfArrows>());
            break;
        case SpellType::PlaceTrap:
            learnSpell(std::make_shared<PlaceTrapSpell>());
            break;
        case SpellType::SummonDecoy: 
            learnSpell(std::make_shared<SummonDecoySpell>());
            break;
        case SpellType::Empower:
            learnSpell(std::make_shared<EmpowerSpell>());
            break;
    }
}

bool Gauntlet::learnSpell(std::shared_ptr<Spell> newSpell) {

    if (newSpell && spells.size() < maxSpells) {
        spells.push_back(newSpell);
        return true;
    }
    return false; 
}

const std::vector<std::shared_ptr<Spell>>& Gauntlet::getKnownSpells() const {
    return spells;
}

bool Gauntlet::activateMapSkill(int index, std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen) {

    if (index < 0) {return false;}

    if (index >= 0 && index < spells.size()) {

        auto& spellToActivate = spells[index];

        if (spellToActivate->getCost() > player->getMp()) {
            mvprintw(screen->yMax - 10, screen->xMax / 2 - 5, "Not enough MP");
            refresh();
            napms(1000);
            flushinp(); 
            return false;
        }

        bool usedSkill = spellToActivate->apply(player, ceils, entities, screen);

        if (usedSkill) {

            player->useSkill(spellToActivate->getCost());
        }
        return usedSkill;
    }

    return false;
}

void Gauntlet::clear() {
    spells.clear();
}

void Gauntlet::randomlyRemoveHalfSpells() { 
    if (spells.size() <= 1) {
        return; 
    }

    int spellsToRemove = spells.size() / 2;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(spells.begin(), spells.end(), g);

    spells.erase(spells.begin(), spells.begin() + spellsToRemove);
}