#ifndef SLIME_FIELD_HPP
#define SLIME_FIELD_HPP

#include <string>

#include "../creature.hpp"
#include "field.hpp"
#include "generic-field.hpp"

#define SLIME_FIELD_BYTE 2

class SlimeField : public GenericField {
  public:
    SlimeField();
    SlimeField(const SlimeField &);
    Field *clone() override;

    void occupy(Creature *occupier) override;

    Color color() override;

    bytedump dump() override;
};

#endif /* SLIME_FIELD_HPP */
