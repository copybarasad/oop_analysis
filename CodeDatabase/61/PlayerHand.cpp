#include "PlayerHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"
#include <iostream>
#include <random>
#include <sstream>
#include <string>

PlayerHand::PlayerHand(int handSize)
    : maxSize(handSize), randomGenerator(std::random_device{}()) {
    
    if (handSize <= 0) {
        throw std::invalid_argument("Hand size must be positive");
    }
    
    // Начальная рука содержит одно случайное заклинание
    addRandomSpell();
}

std::unique_ptr<Spell> PlayerHand::createRandomSpell() const {
    std::uniform_int_distribution<> dis(0, 4);
    int spellType = dis(randomGenerator);
    
    switch (spellType) {
        case 0:
            return std::make_unique<DirectDamageSpell>(3, 25);
        case 1:
            return std::make_unique<AreaDamageSpell>(4, 20, 2);
        case 2:
            return std::make_unique<TrapSpell>(3, 30);
        case 3:
            return std::make_unique<SummonSpell>(1, 1);
        case 4:
            return std::make_unique<EnhancementSpell>();
        default:
            return std::make_unique<DirectDamageSpell>(3, 25);
    }
}

bool PlayerHand::addSpell(std::unique_ptr<Spell> spell) {
    if (isFull()) {
        return false;
    }
    
    spells.push_back(std::move(spell));
    return true;
}

bool PlayerHand::removeSpell(size_t index) {
    if (index >= spells.size()) {
        return false;
    }
    
    spells.erase(spells.begin() + index);
    return true;
}

Spell* PlayerHand::getSpell(size_t index) const {
    if (index >= spells.size()) {
        return nullptr;
    }
    
    return spells[index].get();
}

size_t PlayerHand::getSpellCount() const {
    return spells.size();
}

int PlayerHand::getMaxSize() const {
    return maxSize;
}

bool PlayerHand::isFull() const {
    return spells.size() >= static_cast<size_t>(maxSize);
}

void PlayerHand::addRandomSpell() {
    if (!isFull()) {
        addSpell(createRandomSpell());
    }
}

void PlayerHand::displayHand() const {
    std::cout << "\n=== Player Hand (" << spells.size() << "/" << maxSize << ") ===" << std::endl;
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << "[" << i << "] " << spells[i]->getName() 
                  << " - " << spells[i]->getDescription() << std::endl;
    }
    std::cout << "================================" << std::endl;
}

void PlayerHand::applyPendingEnhancementToNext() {
    // Этот метод будет вызван при применении следующего заклинания
    // Реализация будет в Game, где мы получим pending enhancement
}

std::string PlayerHand::serialize() const {
    std::ostringstream oss;
    oss << "[PlayerHand]\n";
    oss << "maxSize=" << maxSize << "\n";
    oss << "spellCount=" << spells.size() << "\n";
    oss << "[Spells]\n";
    
    for (size_t i = 0; i < spells.size(); ++i) {
        Spell* spell = spells[i].get();
        if (!spell) continue;
        
        std::string spellName = spell->getName();
        oss << "spell_" << i << "_type=" << spellName << "\n";
        oss << "spell_" << i << "_radius=" << spell->getBaseRadius() << "\n";
        oss << "spell_" << i << "_damage=" << spell->getBaseDamage() << "\n";
    }
    
    return oss.str();
}

bool PlayerHand::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    std::string currentSection;
    int readMaxSize = 5;
    int spellCount = 0;
    bool inSpellsSection = false;
    
    // Очищаем текущие заклинания
    spells.clear();
    
    // Читаем данные
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            inSpellsSection = (currentSection == "Spells");
            continue;
        }
        
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        if (currentSection == "PlayerHand") {
            if (key == "maxSize") {
                readMaxSize = std::stoi(value);
                maxSize = readMaxSize;
            } else if (key == "spellCount") {
                spellCount = std::stoi(value);
            }
        } else if (inSpellsSection) {
            // Парсим заклинания
            if (key.find("_type") != std::string::npos) {
                size_t idxPos = key.find("spell_");
                if (idxPos != std::string::npos) {
                    size_t underscorePos = key.find('_', 6);
                    if (underscorePos != std::string::npos) {
                        int spellIndex = std::stoi(key.substr(6, underscorePos - 6));
                        
                        // Читаем radius и damage из следующих строк
                        std::string radiusLine, damageLine;
                        if (std::getline(iss, radiusLine) && std::getline(iss, damageLine)) {
                            size_t radiusPos = radiusLine.find('=');
                            size_t damagePos = damageLine.find('=');
                            
                            if (radiusPos != std::string::npos && damagePos != std::string::npos) {
                                int radius = std::stoi(radiusLine.substr(radiusPos + 1));
                                int damage = std::stoi(damageLine.substr(damagePos + 1));
                                
                                // Создаем заклинание по типу
                                std::unique_ptr<Spell> spell;
                                if (value == "DirectDamage") {
                                    spell = std::make_unique<DirectDamageSpell>(radius, damage);
                                } else if (value == "AreaDamage") {
                                    spell = std::make_unique<AreaDamageSpell>(radius, damage, 2);
                                } else if (value == "Trap") {
                                    spell = std::make_unique<TrapSpell>(radius, damage);
                                } else if (value == "Summon") {
                                    spell = std::make_unique<SummonSpell>(radius, damage);
                                } else if (value == "Enhancement") {
                                    spell = std::make_unique<EnhancementSpell>();
                                }
                                
                                if (spell) {
                                    spells.push_back(std::move(spell));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return true;
}

