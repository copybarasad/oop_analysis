#pragma once
#include <string>
#include <memory>
#include "Pos.hpp"

class Field;

// Унифицированный интерфейс заклинания
class ISpell {
public:
    virtual ~ISpell() = default;

    // Попытаться применить заклинание
    // Возвращает true если заклинание использовано (ход потрачен)
    virtual bool cast(Field& field, const Pos& casterPos, const Pos& target) = 0;

    virtual std::string name() const = 0;

    // Глубокая копия для копирования Hand
    virtual std::unique_ptr<ISpell> clone() const = 0;
};
