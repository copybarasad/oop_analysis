#ifndef MOVE_FIELDS_H
#define MOVE_FIELDS_H

#include "../BaseChangeableFields/ChangebaleFields.hpp"
#include "../../Field/Field/Field.hpp"

class MoveFields : public ChangeableFields {
    private:
    Field& field;
    std::pair<size_t, size_t> target;

    public:
    MoveFields (Field& field, std::pair<size_t, size_t> target);

    Field& getField () const;
    std::pair<size_t, size_t> getTarget () const;
};

#endif