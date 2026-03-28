#ifndef ENHANCEMENTSPELL_H
#define ENHANCEMENTSPELL_H

#include "Spell.h"

class Game;

// Структура для хранения улучшений
struct Enhancement {
    int radiusBonus;
    int damageBonus;
    int areaBonus;
    int summonBonus;
    
    Enhancement() : radiusBonus(0), damageBonus(0), areaBonus(0), summonBonus(0) {}
    Enhancement(int r, int d, int a, int s) 
        : radiusBonus(r), damageBonus(d), areaBonus(a), summonBonus(s) {}
    
    Enhancement& operator+=(const Enhancement& other) {
        radiusBonus += other.radiusBonus;
        damageBonus += other.damageBonus;
        areaBonus += other.areaBonus;
        summonBonus += other.summonBonus;
        return *this;
    }
};

class EnhancementSpell : public Spell {
public:
    EnhancementSpell();
    
    SpellResult use(Game& game, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    std::unique_ptr<Spell> clone() const override;
    
    // Статический метод для получения текущих накопленных улучшений
    static Enhancement getPendingEnhancement();
    static void clearPendingEnhancement();
    static void addPendingEnhancement(const Enhancement& enh);
};

#endif // ENHANCEMENTSPELL_H

