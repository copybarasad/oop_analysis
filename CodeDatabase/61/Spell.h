#ifndef SPELL_H
#define SPELL_H

#include <memory>
#include <string>

// Forward declarations
class Game;
class Player;
class GameField;

// Структура для передачи данных о результате применения заклинания
struct SpellResult {
    bool success;
    bool turnUsed;
    std::string message;
    
    SpellResult(bool s = false, bool t = false, const std::string& msg = "")
        : success(s), turnUsed(t), message(msg) {}
};

// Базовый интерфейс заклинания
class Spell {
protected:
    int baseRadius;
    int baseDamage;
    
public:
    Spell(int radius = 1, int damage = 0);
    virtual ~Spell() = default;
    
    // Основной метод применения заклинания
    virtual SpellResult use(Game& game, int targetX, int targetY) = 0;
    
    // Получение имени заклинания
    virtual std::string getName() const = 0;
    
    // Получение описания
    virtual std::string getDescription() const = 0;
    
    // Методы для улучшений
    virtual void applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus);
    
    // Клонирование заклинания (для создания копий)
    virtual std::unique_ptr<Spell> clone() const = 0;
    
    // Геттеры
    int getBaseRadius() const;
    int getBaseDamage() const;
};

#endif // SPELL_H

