#include "Spell.h"

Spell::Spell(unsigned int castDiameter, unsigned int cooldown) :
        castDiameter(std::max<unsigned int>(castDiameter, 1)), cooldown(cooldown), reloadTimer(0), level(1) {}

Spell::Spell(json& data) {
    if (!data.contains("castDiameter"))
        throw std::runtime_error("Отсутствует поле castDiameter у заклинания");
    if (!data["castDiameter"].is_number_unsigned())
        throw std::runtime_error("Значение поля castDiameter у заклинания не является беззнаковым числом");
    castDiameter = data["castDiameter"].get<unsigned int>();

    if (!data.contains("cooldown"))
        throw std::runtime_error("Отсутствует поле cooldown у заклинания");
    if (!data["cooldown"].is_number_unsigned())
        throw std::runtime_error("Значение поля cooldown у заклинания не является беззнаковым числом");
    cooldown = data["cooldown"].get<unsigned int>();

    if (!data.contains("reloadTimer"))
        throw std::runtime_error("Отсутствует поле reloadTimer у заклинания");
    if (!data["reloadTimer"].is_number_unsigned())
        throw std::runtime_error("Значение поля reloadTimer у заклинания не является беззнаковым числом");
    reloadTimer = data["reloadTimer"].get<unsigned int>();
    if (reloadTimer > cooldown)
        throw std::runtime_error("Значение оставшегося времени перезарядки у заклинания превышает базовое время перезарядки");

    if (!data.contains("level"))
        throw std::runtime_error("Отсутствует поле level у заклинания");
    if (!data["level"].is_number_unsigned())
        throw std::runtime_error("Значение поля level у заклинания не является беззнаковым числом");
    level = data["level"].get<unsigned int>();
}

json Spell::to_json() const {
    json j;
    j["castDiameter"] = castDiameter;
    j["cooldown"] = cooldown;
    j["reloadTimer"] = reloadTimer;
    j["level"] = level;
    return j;
}


bool Spell::reload() {
    if (reloadTimer > 0)
        reloadTimer--;
    return reloadTimer == 0;
}

void Spell::fullReload() { reloadTimer = 0; }

bool Spell::isReady() const { return reloadTimer == 0; }

void Spell::used() { reloadTimer = cooldown; }

int Spell::getCooldown() const { return cooldown; }

int Spell::getReloadLeft() const { return reloadTimer; }

int Spell::getCastDiameter() const { return castDiameter; }

int Spell::getLevel() const { return level; }

void Spell::setLevel(int newLevel) { level = newLevel; }

std::pair<int, int> Spell::visualData() const { return {1, getCastDiameter()}; }

bool Spell::allCellsInRange(std::pair<int,int>& target) const { return true; }
