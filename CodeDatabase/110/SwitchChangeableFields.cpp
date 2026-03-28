#include "SwitchChangeableFields.hpp"

SwitchFields::SwitchFields (std::string chosenType) :
    chosenType(chosenType) {}

std::string SwitchFields::getChosenType () const { return chosenType; }