#ifndef I_DAMAGEABLE_H
#define I_DAMAGEABLE_H

#include "Position.h"
#include <string>

// Интерфейс для всех объектов, которые могут получать урон
// Это позволяет CombatSystem работать с любыми типами врагов, не зная их конкретных классов
class IDamageable {
public:
    virtual ~IDamageable() = default;
    
    // Нанести урон объекту - должна быть реализована в каждом классе
    virtual void takeDamage(int damage) = 0;
    
    // Проверить жив ли объект - обычно проверяет health > 0
    virtual bool isAlive() const = 0;
    
    // Получить позицию объекта на игровом поле - для расчета расстояний
    virtual Position getPosition() const = 0;
    
    // Получить имя объекта - для отображения в сообщениях ("Враг", "Здание" и т.д.)
    virtual std::string getName() const = 0;
    
    // Получить награду за уничтожение - очки которые получает игрок
    virtual int getReward() const = 0;
};

#endif