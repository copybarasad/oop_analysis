#ifndef SWITCH_FIELDS_H
#define SWITCH_FIELDS_H

#include <string>
#include "../BaseChangeableFields/ChangebaleFields.hpp"

class SwitchFields : public ChangeableFields {
    private:
    std::string chosenType;
    public:
    SwitchFields (std::string chosenType);

    std::string getChosenType () const;
};

#endif