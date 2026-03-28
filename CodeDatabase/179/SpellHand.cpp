#include "SpellHand.h"
#include "GameField.h"
#include "Player.h"
#include <iostream>
#include <random>

// Реализация остается той же
SpellHand::SpellHand(size_t maxSize) : maxSize(maxSize), nextEnhancement(nullptr) {}

bool SpellHand::addSpell(std::shared_ptr<Spell> spell) {
    if (spells.size() >= maxSize) return false;
    spells.push_back(spell);
    return true;
}

bool SpellHand::removeSpell(size_t index) {
    if (index >= spells.size()) return false;
    spells.erase(spells.begin() + index);
    return true;
}

std::shared_ptr<Spell> SpellHand::getSpell(size_t index) const {
    if (index >= spells.size()) return nullptr;
    return spells[index];
}

size_t SpellHand::getSpellCount() const { return spells.size(); }
size_t SpellHand::getMaxSize() const { return maxSize; }

void SpellHand::addRandomSpell() {
    if (spells.size() >= maxSize) return;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 4);
    
    std::shared_ptr<Spell> newSpell;
    switch (dis(gen)) {
        case 0: newSpell = std::make_shared<DirectDamageSpell>(); break;
        case 1: newSpell = std::make_shared<AreaDamageSpell>(); break;
        case 2: newSpell = std::make_shared<TrapSpell>(); break;
        case 3: newSpell = std::make_shared<SummonSpell>(); break;
        case 4: newSpell = std::make_shared<EnhancementSpell>(); break;
    }
    
    spells.push_back(newSpell);
}

bool SpellHand::castSpell(size_t index, const Position& target, GameField& field, const Position& casterPos) {
    if (index >= spells.size()) return false;
    
    auto spell = spells[index];
    auto player = field.getPlayer();
    
    // Проверяем ману
    if (!player->useMana(spell->getManaCost())) {
        std::cout << "Not enough mana! Need " << spell->getManaCost() << " mana.\n";
        return false;
    }
    
    bool result = false;
    
    // Если это заклинание улучшения - применяем его отдельно
    if (spell->isEnhancement()) {
        setNextEnhancement(spell);
        result = spell->cast(target, field, casterPos);
    } else {
        // Применяем улучшение если есть
        if (nextEnhancement) {
            applyEnhancement(spell);
            nextEnhancement = nullptr;
        }
        result = spell->cast(target, field, casterPos);
    }
    
    return result;
}

void SpellHand::setNextEnhancement(std::shared_ptr<Spell> enhancement) {
    nextEnhancement = enhancement;
}

void SpellHand::applyEnhancement(std::shared_ptr<Spell> spell) {
    if (nextEnhancement) {
        // Применяем улучшение в зависимости от типа заклинания
        int power = nextEnhancement->getEnhancementLevel() + 1;
        spell->setEnhancementLevel(spell->getEnhancementLevel() + power);
        
        // Улучшаем базовые параметры
        spell->enhance();
        
        std::cout << "✨ Spell enhanced! (Power: " << power << ")\n";
        nextEnhancement = nullptr;
    }
}

void SpellHand::displaySpells() const {
    std::cout << "\n=== ВАШИ ЗАКЛИНАНИЯ ===\n";
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ". " << spells[i]->getName() << " - "
                  << spells[i]->getDescription() << " (Мана: " << spells[i]->getManaCost() << ")\n";
    }
    if (nextEnhancement) {
        std::cout << "Следующее заклинание будет усилено!\n";
    }
    if (spells.empty()) {
        std::cout << "У вас нет заклинаний!\n";
    }
    std::cout << "========================\n";
}

void SpellHand::save(std::ostream& file) const {
    // Сохраняем количество заклинаний
    size_t spellCount = spells.size();
    file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    
    // Сохраняем каждое заклинание
    for (const auto& spell : spells) {
        // Сохраняем тип заклинания (по имени)
        std::string spellType = spell->getName();
        size_t typeLength = spellType.length();
        file.write(reinterpret_cast<const char*>(&typeLength), sizeof(typeLength));
        file.write(spellType.c_str(), typeLength);
        
        // Сохраняем уровень улучшения
        int enhancementLevel = spell->getEnhancementLevel();
        file.write(reinterpret_cast<const char*>(&enhancementLevel), sizeof(enhancementLevel));
    }
}

void SpellHand::load(std::istream& file) {
    // Очищаем текущие заклинания
    spells.clear();
    
    // Загружаем количество заклинаний
    size_t spellCount;
    file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    
    // Загружаем каждое заклинание
    for (size_t i = 0; i < spellCount; ++i) {
        // Загружаем тип заклинания
        size_t typeLength;
        file.read(reinterpret_cast<char*>(&typeLength), sizeof(typeLength));
        std::string spellType(typeLength, ' ');
        file.read(&spellType[0], typeLength);
        
        // Загружаем уровень улучшения
        int enhancementLevel;
        file.read(reinterpret_cast<char*>(&enhancementLevel), sizeof(enhancementLevel));
        
        // Создаем заклинание по типу
        std::shared_ptr<Spell> spell;
        if (spellType == "Direct Damage") {
            spell = std::make_shared<DirectDamageSpell>();
        } else if (spellType == "Area Damage") {
            spell = std::make_shared<AreaDamageSpell>();
        } else if (spellType == "Trap") {
            spell = std::make_shared<TrapSpell>();
        } else if (spellType == "Summon Ally") {
            spell = std::make_shared<SummonSpell>();
        } else if (spellType == "Enhancement") {
            spell = std::make_shared<EnhancementSpell>();
        }
        
        // Устанавливаем уровень улучшения
        if (spell) {
            spell->setEnhancementLevel(enhancementLevel);
            spells.push_back(spell);
        }
    }
}
