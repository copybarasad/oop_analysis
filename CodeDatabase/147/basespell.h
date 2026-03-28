#ifndef BASESPELL_H
#define BASESPELL_H

#include <string>
#include <memory>
#include "i_spell.h"

class BaseSpell : public ISpell {
protected:
    bool _consumed = false;      // Было ли заклинание использовано
    std::string _name;           // Название заклинания
    int _damage;                 // Сила урона
    int _radius;                 // Радиус действия

public:
    BaseSpell(const std::string& name, int damage, int radius)
        : _name(name), _damage(damage), _radius(radius) {}

    // --- Общие методы интерфейса ---
    bool isConsumed() const override { return _consumed; }
    void consume() override { _consumed = true; }

    std::string name() const override { return _name; }

    // --- Геттеры, если нужно получить параметры ---
    int damage() const { return _damage; }
    int radius() const { return _radius; }

    // --- Чисто виртуальный clone() ---
    virtual std::unique_ptr<ISpell> clone() const override = 0;

    virtual ~BaseSpell() = default;
};

#endif // BASESPELL_H

