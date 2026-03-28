#include "spell_factory.hpp"
#include "earthquake.hpp"
#include "heal.hpp"
#include <random>

std::unique_ptr<Spell> SpellFactory::createRandomSpell() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1); 
    if (dis(gen) == 0) {
        return std::make_unique<EarthquakeSpell>();
    } else {
        return std::make_unique<HealSpell>();
    }
}