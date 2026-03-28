#ifndef SPELL_H
#define SPELL_H

#include <string>

class Game;

/**
 * @class Spell
 * @brief Базовый класс для всех заклинаний
 */
class Spell {
protected:
    std::string name;
    std::string description;
    int power;

public:
    Spell(const std::string& spellName, const std::string& desc, int spellPower)
        : name(spellName), description(desc), power(spellPower) {}
    
    virtual ~Spell() = default;

    /**
     * @brief Использовать заклинание без координат (Boost, Heal)
     * @param game Указатель на игру
     * @return true если успешно
     */
    virtual bool cast(Game* game) = 0;
    
    /**
     * @brief Использовать заклинание с координатами (Area Damage, Trap, Summon)
     * @param game Указатель на игру
     * @param targetX Координата X
     * @param targetY Координата Y
     * @return true если успешно
     */
    virtual bool cast(Game* game, int targetX, int targetY) {
        // По умолчанию вызываем версию без координат (для совместимости)
        return cast(game);
    }

    /**
     * @brief Проверить, требует ли заклинание цель (координаты)
     */
    virtual bool requiresTarget() const {
        return false;  // По умолчанию не требует
    }

    /**
     * @brief Проверить, является ли заклинание Boost
     */
    virtual bool isBoostSpell() const {
        return false;
    }

    // Геттеры
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    int getPower() const { return power; }
};

#endif // SPELL_H


