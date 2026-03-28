#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SpellType.h"
#include "GameExceptions.h"
#include <vector>
#include <memory>
#include <cstdlib>

class SpellFactory {
public:
    // Создает заклинание указанного типа
    static std::shared_ptr<Spell> createSpell(SpellType type) {
        switch(type) {
            case SpellType::DIRECT_DAMAGE: 
                return std::make_shared<DirectDamageSpell>();
            case SpellType::AREA_DAMAGE: 
                return std::make_shared<AreaDamageSpell>();
            case SpellType::TRAP: 
                return std::make_shared<TrapSpell>();
            default: 
                throw SpellException("Неизвестный тип заклинания");
        }
    }
    
    // Создает случайное заклинание
    static std::shared_ptr<Spell> createRandomSpell() {
        try {
            int randomValue = std::rand() % 3;
            SpellType selectedType;
            
            switch(randomValue) {
                case 0: selectedType = SpellType::DIRECT_DAMAGE; break;
                case 1: selectedType = SpellType::AREA_DAMAGE; break;
                case 2: selectedType = SpellType::TRAP; break;
                default: selectedType = SpellType::DIRECT_DAMAGE; break;
            }
            
            return createSpell(selectedType);
        } catch (const SpellException& e) {
            throw SpellException("Ошибка создания случайного заклинания: " + std::string(e.what()));
        }
    }
    
    // Получает список всех доступных типов заклинаний
    static std::vector<SpellType> getAvailableSpellTypes() {
        return {SpellType::DIRECT_DAMAGE, SpellType::AREA_DAMAGE, SpellType::TRAP};
    }
};

#endif