#ifndef SPELL_FACTORY_HPP
#define SPELL_FACTORY_HPP

#include <memory>

class Spell;

class SpellFactory {
public:
    SpellFactory();

    std::unique_ptr<Spell> GenerateRandomSpell() const;

    // создание заклинания по типу (для загрузки)
    static std::unique_ptr<Spell> CreateByType(int typeId);

    // определение типа заклинания по реальному объекту (для сохранения)
    static int DetectTypeId(const Spell& spell);

    // применяет level раз Upgrade() к заклинанию (для загрузки)
    static void ApplyUpgradeLevel(Spell& spell, int level);

    // коды типов
    static const int DIRECT_DAMAGE  = 1;
    static const int AREA_DAMAGE    = 2;
    static const int TRAP           = 3;
    static const int SUMMON         = 4;
    static const int ENHANCEMENT    = 5;
};

#endif