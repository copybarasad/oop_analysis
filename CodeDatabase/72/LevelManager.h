//
// Created by Mac on 09.11.2025.
//

#ifndef GAME_TERM_LEVELMANAGER_H
#define GAME_TERM_LEVELMANAGER_H

#define DEFAULT_CURRENT_LEVEL 1
#include "field_objects/Field.h"
#include "SpellOffers.h"
#include "SpellShop.h"

class LevelManager: ISerializable{

    int currentLevel;

public:

    explicit LevelManager(const int & currentLevel = DEFAULT_CURRENT_LEVEL);

    int getCurrentLevel() const;

    void setCurrentLevel(const int & currentLevel);

    void save(Saver &saver) const override;

    void load(Reader &reader) override;

};


#endif //GAME_TERM_LEVELMANAGER_H