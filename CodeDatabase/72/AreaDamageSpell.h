//
// Created by Mac on 25.10.2025.
//

#ifndef GAME_TERM_AREADAMAGESPELL_H
#define GAME_TERM_AREADAMAGESPELL_H

#include <string>

#include "IAreaDamage.h"
#include "ISpell.h"

#define DEFAULT_DAMAGE_AREA 100
#define DEFAULT_AREA_SIZE 2
#define DEFAULT_RADIUS_AREA 4

class AreaDamageSpell: public IAreaDamage{
    int damage;
    int areaSize;
    int radius;
public:

    explicit AreaDamageSpell(const int & damage = DEFAULT_DAMAGE_AREA, const int & areaSize = DEFAULT_AREA_SIZE, const int & radius = DEFAULT_RADIUS_AREA);

    std::string getName() const override;

    void cast(Field & field) override;

    Type getType() const override;

    void save(Saver &saver) const override;

    void load(Reader &reader) override;
};


#endif //GAME_TERM_AREADAMAGESPELL_H