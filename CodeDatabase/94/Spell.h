#ifndef SPELL_H
#define SPELL_H

#include "GameField.h"
#include "Position.h"
#include <string>

// Базовый класс для всех заклинаний в игре
// Определяет интерфейс, который должны реализовать все конкретные заклинания
class Spell {
public:
    virtual ~Spell() = default;
    
    // Основной метод применения заклинания
    virtual bool cast(const Position& target, GameField& field, Character& caster) = 0;
    
    // Название заклинания для отображения в интерфейсе
    virtual std::string getName() const = 0;
    
    // Описание заклинания (эффекты, параметры)
    virtual std::string getDescription() const = 0;
    
    // Дальность применения заклинания (0 - на себя)
    virtual int getRange() const = 0;
    
    // Требуется ли выбор цели для применения
    virtual bool requiresTarget() const = 0;
};

#endif