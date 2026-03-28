#pragma once
#include "types.hpp"
#include "GameObjects/Entities/PersonBaseClass.hpp"
#include "LevelManager/LMtypes.hpp"

class Player : public PersonBaseClass {
public:
    Player(PlayerDTO p);
    PlayerDTO toDTO();

    bool isRangeForm();
    void changeRangeForm();
    void setRangeForm(bool rf);
private:
    bool rangeForm;
};
