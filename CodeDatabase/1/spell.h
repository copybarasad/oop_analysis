#pragma once
#include "position.h"
#include <string>
#include <vector>

enum class SpellType {
    HOLY_WATER,     // Ловушка
    LIGHT_BEAM,     // Прямой урон  
    SACRED_GLOW     // Площадь урона
};

// Добавляем структуру для результата каста
struct SpellCastResult {
    std::vector<Position> affectedPositions;  // Позиции для урона/ловушек
    int damage;                              // Величина урона
    bool isTrap;                             // Это ловушка?
    
    SpellCastResult() : damage(0), isTrap(false) {}
};

class Spell {
protected:
    std::string name_;
    int range_;
    SpellType type_;

public:
    Spell(const std::string& name, int range, SpellType type)
        : name_(name), range_(range), type_(type) {
    }
    virtual ~Spell() = default;
    //ЗАМЕНЯЕМ cast на calculateCast - теперь только расчеты!
    virtual SpellCastResult calculateCast(const Position& casterPos, 
                                         const Position& targetPos) const = 0;
    
    // Добавляем проверку возможности каста
    virtual bool canCast(const Position& casterPos, const Position& targetPos) const = 0;
    
    std::string getName() const { return name_; }
    int getRange() const { return range_; }
    SpellType getType() const { return type_; }
};