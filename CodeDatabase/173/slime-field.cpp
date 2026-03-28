#include "slime-field.hpp"

SlimeField::SlimeField() : GenericField() {}
SlimeField::SlimeField(const SlimeField &field) : GenericField(field) {}
Field *SlimeField::clone() { return new SlimeField(*this); }

void SlimeField::occupy(Creature *occupier) {
    GenericField::occupy(occupier);
    occupier->addStatus(Status::InSlime, 2);
}

Color SlimeField::color() {
    if (m_CreatureSlot != nullptr) {
        return m_CreatureSlot->color();
    }

    if (m_BuildingSlot != nullptr) {
        return m_BuildingSlot->color();
    }

    return Color::lime();
}

bytedump SlimeField::dump() {
    bytedump bytes;
    bytes.push_back(static_cast<std::byte>(SLIME_FIELD_BYTE));
    return bytes;
}
