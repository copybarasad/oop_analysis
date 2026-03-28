#include "Entity.h"

Entity::Entity(unsigned int h, unsigned int d, EntityTeam team, char symbol, unsigned int viewDiameter, unsigned int spellsHandSize) : maxHealth(h),
        health(h), team(team), symbol(symbol), viewDiameter(viewDiameter), points(h + d), slowed(false),
        target({-1, -1}), damage(d), spellsHand(spellsHandSize) {}

Entity::Entity(json &data, EntityTeam team) : team(team),
        spellsHand(data.contains("spellsHand") ? SpellsHand(data["spellsHand"]) : throw std::runtime_error("Отсутствует поле spellsHand у сущности")) {
        if (!data.contains("maxHealth"))
                throw std::runtime_error("Отсутствует поле maxHealth у сущности");
        if (!data["maxHealth"].is_number_unsigned())
                throw std::runtime_error("Значение поля maxHealth у сущности не является беззнаковым числом");
        maxHealth = data["maxHealth"].get<unsigned int>();

        if (!data.contains("health"))
                throw std::runtime_error("Отсутствует поле health у сущности");
        if (!data["health"].is_number_unsigned())
                throw std::runtime_error("Значение поля health у сущности не является беззнаковым числом");
        health = data["health"].get<unsigned int>();
        if (health > maxHealth)
                throw std::runtime_error("Значение поля health у сущности принимает значение, большее чем максимальное здоровье");

        symbol = data["symbol"].get<char>();

        if (!data.contains("viewDiameter"))
                throw std::runtime_error("Отсутствует поле viewDiameter у сущности");
        if (!data["viewDiameter"].is_number_unsigned())
                throw std::runtime_error("Значение поля viewDiameter у сущности не является беззнаковым числом");
        viewDiameter = data["viewDiameter"].get<unsigned int>();

        if (!data.contains("points"))
                throw std::runtime_error("Отсутствует поле points у сущности");
        if (!data["points"].is_number_unsigned())
                throw std::runtime_error("Значение поля points у сущности не является беззнаковым числом");
        points = data["points"].get<unsigned int>();

        if (!data.contains("slowed"))
                throw std::runtime_error("Отсутствует поле slowed у сущности");
        if (!data["slowed"].is_boolean())
                throw std::runtime_error("Значение поля slowed у сущности не является булевым значением");
        slowed = data["slowed"].get<bool>();

        if (!data.contains("damage"))
                throw std::runtime_error("Отсутствует поле damage у сущности");
        if (!data["damage"].is_number_unsigned())
                throw std::runtime_error("Значение поля damage у сущности не является беззнаковым числом");
        damage = data["damage"].get<unsigned int>();

        if (!data.contains("target"))
                throw std::runtime_error("Отсутствует поле target у сущности");
        if (!(data["target"].is_array()
           && data["target"].size() == 2
           && data["target"][0].is_number_integer()
           && data["target"][1].is_number_integer()))
                throw std::runtime_error("Значение поля target у сущности не является парой чисел");
        target = data["target"].get<std::pair<int,int>>();

        if (!data.contains("position"))
                throw std::runtime_error("Отсутствует поле position у сущности");
        if (!(data["position"].is_array()
           && data["position"].size() == 2
           && data["position"][0].is_number_integer()
           && data["position"][1].is_number_integer()))
                throw std::runtime_error("Значение поля position у сущности не является парой чисел");
        position = data["position"].get<std::pair<int,int>>();
}

Entity::~Entity() = default;

json Entity::to_json() const {
        json j;
        j["maxHealth"] = maxHealth;
        j["health"] = health;
        j["team"] = team;
        j["symbol"] = symbol;
        j["viewDiameter"] = viewDiameter;
        j["points"] = points;
        j["slowed"] = slowed;
        j["damage"] = damage;
        j["target"] = target;
        j["position"] = position;
        j["spellsHand"] = spellsHand.to_json();
        return j;
}

unsigned int Entity::getHealth() const { return health; }

void Entity::setHealth(unsigned int value) { health = std::min<unsigned int>(value, maxHealth); }

void Entity::setMaxHealth(unsigned int value) { maxHealth = value; }

unsigned int Entity::getMaxHealth() const { return maxHealth; }

void Entity::restoreHealth() { health = maxHealth; }

bool Entity::isAlive() const { return health != 0; }

unsigned int Entity::getActionDiameter() {
        if (auto spell = spellsHand.getSelectedSpell())
                return spell->getCastDiameter();
        return viewDiameter;
}

void Entity::setViewDiameter(unsigned int newDistance) { viewDiameter = newDistance; }

unsigned int Entity::getViewDiameter() const { return viewDiameter; }

unsigned int Entity::getDamage() const { return damage; }

void Entity::setPosition(const std::pair<int,int> &new_position) { position = new_position; }

const std::pair<int,int>& Entity::getPosition() const { return position; }

bool Entity::takeDamage(unsigned int amount) {
        if (amount >= health) {
                health = 0;
                return true;
        }
        health -= amount;
        return false;
}

StepInfo Entity::move(StepRequirements& arguments) { return {StepType::WALK, arguments.wayToEnemy}; }

void Entity::onEnter(Entity* aggressor) {}

EntityTeam Entity::getTeam() const { return team; }

bool Entity::isSlowed() const { return slowed; }

char Entity::getSymbol() const { return symbol; }

unsigned int Entity::getPoints() const { return points; }

std::pair<int,int> Entity::getTarget() { return target; }

void Entity::setSlowed(bool state) { slowed = state; }

void Entity::setTarget(std::pair<int,int>& newTarget) { target = newTarget; }

void Entity::setTarget(std::pair<int,int> newTarget) { target = newTarget; }

void Entity::setPoints(unsigned int amount) { points = amount; }

void Entity::addPoints(unsigned int amount) { points += amount; }

SpellsHand& Entity::getSpellsHand() { return spellsHand; }

void Entity::reloadSpells() { spellsHand.reload(); }

std::string Entity::getSpellsInfo() const {
        std::string result;
        int index = 1;
        if (!spellsHand.empty())
                result += "Заклинания в руке:\n";
        for (auto& spell: spellsHand.getSpells()) {
                if (!spell->isReady())
                        result += "\033[2m";
                result += "Заклинание " + std::to_string(index++) + ". " + spell->getName();
                result += " (уровень " + std::to_string(spell->getLevel()) + ")";
                if (!spell->isReady()) {
                        result += " (перезарядка " + std::to_string(spell->getReloadLeft()) + ")";
                        result += "\033[m";
                }
                result += "\n";
        }
        return result;
}
