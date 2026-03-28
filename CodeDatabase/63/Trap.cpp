#include "Trap.h"

Trap::Trap(unsigned int d, EntityTeam team) : Entity(1, d, team, 'M', 0, 0), lifetime(5) {
    setPoints(0);
}

Trap::Trap(json &data) : Entity(data, EntityTeam::ALLY) {
    if (!data.contains("lifetime"))
        throw std::runtime_error("Отсутствует поле lifetime у ловушки");
    if (!data["lifetime"].is_number_unsigned())
        throw std::runtime_error("Значение поля lifetime у ловушки не является беззнаковым числом");
    lifetime = data["lifetime"].get<unsigned int>();
}

json Trap::to_json() const {
    json data = Entity::to_json();
    data["lifetime"] = lifetime;
    return data;
}

std::unique_ptr<Entity> Trap::clone() const {
    return std::make_unique<Trap>(*this);
}

StepInfo Trap::move(StepRequirements& arguments) {
    lifetime--;
    if (lifetime == 0)
        takeDamage(getHealth());
    return {StepType::WALK, SUCCESSFUL_STEP};
}

void Trap::onEnter(Entity *aggressor) {
    aggressor->takeDamage(damage);
}