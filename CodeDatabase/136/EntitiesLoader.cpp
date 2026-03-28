#include "EntitiesLoader.h"
#include "GameExceptions.h"
#include <iostream>

EntitiesLoader::EntitiesLoader(std::ifstream& file, GameField& field, 
                              BeastManager& beastManager, BuildingManager& buildingManager,
                              GameController& controller)
    : file(file), field(field), beastManager(beastManager), 
      buildingManager(buildingManager), gameController(controller) {}

bool EntitiesLoader::loadEntitiesData() {
    try {
        std::string marker;
        file >> marker;
        
        if (marker != "ENTITIES_DATA") {
            throw LoadException("entities data", "marker not found: " + marker);
        }

        beastManager.clearBeasts();
        
        while (file >> marker && marker != "END_ENTITIES_DATA") {
            if (marker == "BEASTS") {
                if (!loadBeasts()) return false;
            } else if (marker == "BUILDINGS") {
                if (!loadBuildings()) return false;
            } else if (marker == "TOWERS") {
                if (!loadTowers()) return false;
            } else if (marker == "SPELLS") {
                if (!loadSpells()) return false;
            } else {
                throw LoadException("entities data", "unknown entity type: " + marker);
            }
        }
        
        if (marker != "END_ENTITIES_DATA") {
            throw LoadException("entities data", "end marker not found");
        }
        
        std::cout << "Entities data loaded successfully\n";
        return true;
        
    } catch (const LoadException& e) {
        throw;
    } catch (const std::exception& e) {
        throw LoadException("entities data", std::string("unexpected: ") + e.what());
    }
}

bool EntitiesLoader::loadBeasts() {
    try {
        int beastCount;
        file >> beastCount;
        
        if (beastCount < 0 || beastCount > 1000) {
            throw ValidationException("beast count", std::to_string(beastCount));
        }
        
        std::cout << "Loading " << beastCount << " beasts\n";
        
        for (int i = 0; i < beastCount; ++i) {
            int x, y, beastHealth, beastDamage;
            file >> x >> y >> beastHealth >> beastDamage;
            
            if (file.fail()) {
                throw LoadException("beast data", "read failed for beast " + std::to_string(i));
            }
            
            if (!validateBeastData(beastHealth, beastDamage)) {
                return false;
            }
            
            if (field.isValidPosition(x, y)) {
                Beast beast(beastHealth, beastDamage);
                beastManager.addBeast(Position(x, y), beast);
                std::cout << "Beast at (" << x << "," << y << ") health=" << beastHealth << "\n";
            }
        }
        return true;
        
    } catch (const ValidationException& e) {
        throw LoadException("beasts", e.what());
    } catch (const std::exception& e) {
        throw LoadException("beasts", std::string("unexpected: ") + e.what());
    }
}

bool EntitiesLoader::loadBuildings() {
    try {
        int buildingCount;
        file >> buildingCount;
        
        if (buildingCount < 0 || buildingCount > 100) {
            throw ValidationException("building count", std::to_string(buildingCount));
        }
        
        std::cout << "Loading " << buildingCount << " buildings\n";
        
        for (int i = 0; i < buildingCount; ++i) {
            int x, y, buildingHealth, maxBuildingHealth;
            file >> x >> y >> buildingHealth >> maxBuildingHealth;
            
            if (file.fail()) {
                throw LoadException("building data", "read failed for building " + std::to_string(i));
            }
            
            if (!validateBuildingData(buildingHealth)) {
                return false;
            }
            
            if (field.isValidPosition(x, y)) {
                BeastBuilding building(10, maxBuildingHealth);
                if (buildingHealth < maxBuildingHealth) {
                    building.takeDamage(maxBuildingHealth - buildingHealth);
                }
                buildingManager.addBuilding(Position(x, y), building);
                std::cout << "Building at (" << x << "," << y << ") health=" << buildingHealth << "\n";
            }
        }
        return true;
        
    } catch (const ValidationException& e) {
        throw LoadException("buildings", e.what());
    } catch (const std::exception& e) {
        throw LoadException("buildings", std::string("unexpected: ") + e.what());
    }
}

bool EntitiesLoader::loadTowers() {
    try {
        int towerCount;
        file >> towerCount;
        
        if (towerCount < 0 || towerCount > 50) {
            throw ValidationException("tower count", std::to_string(towerCount));
        }
        
        std::cout << "Loading " << towerCount << " towers\n";
        
        for (int i = 0; i < towerCount; ++i) {
            int x, y, towerHealth, maxTowerHealth, range;
            file >> x >> y >> towerHealth >> maxTowerHealth >> range;
            
            if (file.fail()) {
                throw LoadException("tower data", "read failed for tower " + std::to_string(i));
            }
            
            if (!validateTowerData(range, towerHealth)) {
                return false;
            }
            
            if (field.isValidPosition(x, y)) {
                EnemyTower tower(range, 15, 3, maxTowerHealth);
                if (towerHealth < maxTowerHealth) {
                    tower.takeDamage(maxTowerHealth - towerHealth);
                }
                buildingManager.addTower(Position(x, y), tower);
                std::cout << "Tower at (" << x << "," << y << ") health=" << towerHealth << " range=" << range << "\n";
            }
        }
        return true;
        
    } catch (const ValidationException& e) {
        throw LoadException("towers", e.what());
    } catch (const std::exception& e) {
        throw LoadException("towers", std::string("unexpected: ") + e.what());
    }
}

bool EntitiesLoader::loadSpells() {
    try {
        int spellCount;
        file >> spellCount;
        
        if (spellCount < 0 || spellCount > 10) {
            throw ValidationException("spell count", std::to_string(spellCount));
        }
        
        file.ignore();
        
        Wanderer& player = gameController.getPlayer();
        Hand& hand = player.getHand();
        
        while (hand.getSpellCount() > 0) {
            hand.removeSpell(0);
        }
        
        SpellFactory factory;
        for (int i = 0; i < spellCount; ++i) {
            std::string spellName;
            std::getline(file, spellName);
            
            if (file.fail()) {
                throw LoadException("spell data", "read failed for spell " + std::to_string(i));
            }
            
            std::unique_ptr<SpellCard> spell;
            if (spellName == "Fireball") {
                spell = factory.createFireball();
            } else if (spellName == "Lightning Bolt") {
                spell = factory.createPunishmentOfZeus();
            } else if (spellName == "Frosty") {
                spell = factory.createFrosty();
            } else if (spellName == "Beast Trap") {
                spell = factory.createTrapSpell();
            } else {
                throw ValidationException("spell name", spellName);
            }
            
            if (spell) {
                hand.addSpell(std::move(spell));
            }
        }
        
        std::cout << "Loaded " << spellCount << " spells\n";
        return true;
        
    } catch (const ValidationException& e) {
        throw LoadException("spells", e.what());
    } catch (const std::exception& e) {
        throw LoadException("spells", std::string("unexpected: ") + e.what());
    }
}

bool EntitiesLoader::validateBeastData(int health, int damage) const {
    if (health <= 0) {
        throw ValidationException("beast health", std::to_string(health));
    }
    if (damage < 0) {
        throw ValidationException("beast damage", std::to_string(damage));
    }
    return true;
}

bool EntitiesLoader::validateBuildingData(int health) const {
    if (health < 0) {
        throw ValidationException("building health", std::to_string(health));
    }
    return true;
}

bool EntitiesLoader::validateTowerData(int range, int health) const {
    if (range <= 0) {
        throw ValidationException("tower range", std::to_string(range));
    }
    if (health < 0) {
        throw ValidationException("tower health", std::to_string(health));
    }
    return true;
}