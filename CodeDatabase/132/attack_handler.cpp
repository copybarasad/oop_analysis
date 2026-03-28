#include "attack_handler.hpp"

AttackHandler::AttackHandler(FieldHandler& field_handler): 
    field_handler(field_handler) {};

bool AttackHandler::attack(const Position& target, Weapon weapon) {
    GameObject* obj = field_handler.get_obj(target);

    if (auto* target_entity = dynamic_cast<IEntity*>(obj)) {
        target_entity->add_effects(weapon.use(), true);
        return true;
    }
    else {
        std::string err = "Данный игровой объект не может быть атакован: ";
        throw std::invalid_argument(err + obj->get_screen_symbol());
    }

    return false;
}