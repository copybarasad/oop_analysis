//
// Created by Mac on 25.10.2025.
//

#ifndef GAME_TERM_DIRECTDAMAGESPELL_H
#define GAME_TERM_DIRECTDAMAGESPELL_H
#include <string>

#include "ISpell.h"

#define DEFAULT_DAMAGE_DIRECT 100
#define DEFAULT_RADIUS_DIRECT 3
#define DEFAULT_AREA_SIZE_DIRECT 3

#include "IDirectDamage.h"
#include "../field_objects/Field.h"

class DirectDamageSpell: public IDirectDamage{

    int damage;
    int radius;
    int areaSize;

public:

    explicit DirectDamageSpell(const int &damage = DEFAULT_DAMAGE_DIRECT, const int  & radius = DEFAULT_RADIUS_DIRECT, const int & areaSize = DEFAULT_AREA_SIZE_DIRECT);

    std::string getName() const override;

    void cast(Field & field) override;

    void castOnPlayer(std::shared_ptr<Player> player) override;

    Type getType() const override;

    void save(Saver &saver) const override;

    void load(Reader &reader) override;

};


#endif //GAME_TERM_DIRECTDAMAGESPELL_H