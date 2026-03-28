#include "SpellHand.h"
#include "Game.h"
#include "GameEventSystem.h"
#include "GameEvent.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BoostSpell.h"
#include <random>



/**
 * @file SpellHand.cpp
 * @brief Реализация класса руки заклинаний
 */



/**
 * @brief Конструктор руки заклинаний
 * @param maxSize Максимальное количество заклинаний в руке
 */
SpellHand::SpellHand(int maxSize) 
    : maxHandSize(maxSize), boostStacks(0) {
}



/**
 * @brief Добавить заклинание в руку
 * @param spell Умный указатель на заклинание
 * @return true если успешно добавлено
 * 
 * Проверяет свободное место и добавляет заклинание.
 */
bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (isFull()) {
        return false;
    }
    
    spells.push_back(std::move(spell));
    return true;
}



/**
 * @brief Использовать заклинание без координат (Boost, Heal)
 * @param index Индекс заклинания в руке
 * @param game Указатель на игру
 * @return true если заклинание успешно использовано
 * 
 * Отправляет заклинание на выполнение, сбрасывает стаки усиления
 * (кроме самого BoostSpell) и удаляет использованное заклинание.
 */
bool SpellHand::useSpell(int index, Game* game) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return false;
    }
    
    // Логируем использование заклинания
    std::string spellName = spells[index]->getName();
    GameEvent castEvent(EventType::SPELL_CAST, 
                       "Player cast " + spellName);
    GameEventSystem::getInstance().notifyEvent(castEvent);
    
    // Запоминаем тип заклинания ДО удаления
    bool wasBoostSpell = spells[index]->isBoostSpell();
    
    // Применяем заклинание
    bool success = spells[index]->cast(game);
    
    if (success) {
        // Удаляем использованное заклинание
        spells.erase(spells.begin() + index);
        
        // Сбрасываем стаки усиления только если это НЕ BoostSpell
        if (!wasBoostSpell) {
            resetBoostStacks();
        }
        
        return true;
    }
    
    return false;
}



/**
 * @brief Использовать заклинание с координатами (Area Damage, Trap, Summon)
 * @param index Индекс заклинания в руке
 * @param game Указатель на игру
 * @param targetX Координата X цели
 * @param targetY Координата Y цели
 * @return true если заклинание успешно использовано
 */
bool SpellHand::useSpell(int index, Game* game, int targetX, int targetY) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return false;
    }
    
    if (!game) {
        return false;
    }
    
    Spell* spell = spells[index].get();
    if (!spell) {
        return false;
    }
    
    // Логируем использование заклинания с координатами
    std::string spellName = spell->getName();
    GameEvent castEvent(EventType::SPELL_CAST, 
                       "Player cast " + spellName + " at (" + 
                       std::to_string(targetX) + "," + std::to_string(targetY) + ")");
    GameEventSystem::getInstance().notifyEvent(castEvent);
    
    // Запоминаем тип заклинания ДО удаления
    bool wasBoostSpell = spell->isBoostSpell();
    
    // Применяем заклинание с координатами
    bool success = spell->cast(game, targetX, targetY);
    
    if (success) {
        // Удаляем использованное заклинание
        spells.erase(spells.begin() + index);
        
        // Сбрасываем стаки усиления только если это НЕ BoostSpell
        if (!wasBoostSpell) {
            resetBoostStacks();
        }
        
        return true;
    }
    
    return false;
}



/**
 * @brief Добавить случайное заклинание
 * 
 * Генерирует случайное заклинание из 5 типов и добавляет его в руку.
 */
void SpellHand::addRandomSpell() {
    if (isFull()) {
        return;
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 4);
    
    int spellType = dist(gen);
    std::unique_ptr<Spell> newSpell;
    
    switch (spellType) {
        case 0:
            newSpell = std::make_unique<DirectDamageSpell>();
            break;
        case 1:
            newSpell = std::make_unique<AreaDamageSpell>();
            break;
        case 2:
            newSpell = std::make_unique<TrapSpell>();
            break;
        case 3:
            newSpell = std::make_unique<SummonSpell>();
            break;
        case 4:
            newSpell = std::make_unique<BoostSpell>();
            break;
        default:
            newSpell = std::make_unique<DirectDamageSpell>();
    }
    
    // Логируем получение заклинания
    std::string spellName = newSpell->getName();
    GameEvent acquireEvent(EventType::SPELL_ADDED, 
                          "Player acquired spell: " + spellName);
    GameEventSystem::getInstance().notifyEvent(acquireEvent);
    
    addSpell(std::move(newSpell));
}


