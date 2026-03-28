#ifndef GAME_BASESPELL_H
#define GAME_BASESPELL_H


#include "../basics/Entity.h"
#include "../basics/ObjTypes.h"
#include "../basics/Coordinates.h"

class Spell {
protected:
    bool mustBeFree_;
    int damage_;
    int range_;
    int area_;
    int level_ = 1;
    ObjTypes targetType_;
    friend class SpellManager;

public:
    const std::string name_;
    const std::string description_;
    int cost_;

    virtual ~Spell() = default;


    explicit Spell(std::string name = "Base spell",
                   std::string description = "Doesn't do anything",
                   const int damage = 0,
                   const int distance = 0,
                   const int cost = 1,
                   const ObjTypes targetType = ObjTypes::Neither,
                   const int area = 1,
                   const bool mustBeFree = false)
        : name_(std::move(name)), description_(std::move(description)), mustBeFree_(mustBeFree),
          damage_(damage), range_(distance), area_(area),
          targetType_(targetType), cost_(cost) {
    };

    virtual void spellCast(Coordinates cord, GameLogic &glogic) {
    };

    virtual void setLevel(int level);
};


#endif
