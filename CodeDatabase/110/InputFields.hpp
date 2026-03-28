#ifndef INPUT_F_H
#define INPUT_F_H

#include <iostream>
#include "../BaseChangeableFields/ChangebaleFields.hpp"

class InputFields : public ChangeableFields {
    private:
    std::pair<size_t, size_t> target;
    std::string extra;

    public:
    InputFields (std::pair<size_t, size_t> target, std::string extra="");

    std::pair<size_t, size_t> getTarget () const;
    std::string getExtra () const;
};

#endif