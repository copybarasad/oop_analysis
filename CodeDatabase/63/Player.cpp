#include "Player.h"
#include <memory>
#include "IInputManager.h"

Player::Player(unsigned int health, unsigned int meleeDamage, unsigned int shootDamage, unsigned int shootDiameter, unsigned int spellsHandSize) :
        Entity(health, meleeDamage, EntityTeam::ALLY, 'P', 9, spellsHandSize),
        shootDamage(shootDamage), shootDiameter(std::max<unsigned int>(shootDiameter, 3)), mode(PlayerModes::MELEE),
        previousMode(PlayerModes::MELEE), upgradePoints(0) {
    std::mt19937 rng(std::random_device{}());
    spellsHand.addRandomSpell(rng);
    setPoints(0);
}

Player::Player(json &data) : Entity(data, EntityTeam::ALLY) {
    if (!data.contains("shootDamage"))
        throw std::runtime_error("Отсутствует поле shootDamage у игрока");
    if (!data["shootDamage"].is_number_unsigned())
        throw std::runtime_error("Значение поля shootDamage у игрока не является беззнаковым числом");
    shootDamage = data["shootDamage"].get<unsigned int>();

    if (!data.contains("shootDiameter"))
        throw std::runtime_error("Отсутствует поле shootDiameter у игрока");
    if (!data["shootDiameter"].is_number_unsigned())
        throw std::runtime_error("Значение поля shootDiameter у игрока не является беззнаковым числом");
    shootDiameter = data["shootDiameter"].get<unsigned int>();

    if (!data.contains("attackMode"))
        throw std::runtime_error("Отсутствует поле attackMode у игрока");
    if (!data["attackMode"].is_number_integer())
        throw std::runtime_error("Значение поля attackMode у игрока не является числом");
    mode = data["attackMode"].get<PlayerModes>();
    switch (mode) {
        case PlayerModes::MELEE:
        case PlayerModes::RANGE:
        case PlayerModes::SHOP:
        case PlayerModes::SPELL:
        case PlayerModes::UPGRADE:
            break;
        default:
            throw std::runtime_error("Значение поля attackMode у игрока принимает некорректное значение");
    }

    if (!data.contains("previousMode"))
        throw std::runtime_error("Отсутствует поле previousMode у игрока");
    if (!data["previousMode"].is_number_integer())
        throw std::runtime_error("Значение поля previousMode у игрока не является числом");
    previousMode = data["previousMode"].get<PlayerModes>();
    switch (previousMode) {
        case PlayerModes::MELEE:
        case PlayerModes::RANGE:
        case PlayerModes::SHOP:
        case PlayerModes::SPELL:
        case PlayerModes::UPGRADE:
            break;
        default:
            throw std::runtime_error("Значение поля previousMode у игрока принимает некорректное значение");
    }

    if (!data.contains("upgradePoints"))
        throw std::runtime_error("Отсутствует поле upgradePoints у игрока");
    if (!data["upgradePoints"].is_number_unsigned())
        throw std::runtime_error("Значение поля upgradePoints у игрока не является беззнаковым числом");
    upgradePoints = data["upgradePoints"].get<unsigned int>();
    if (upgradePoints && mode != PlayerModes::UPGRADE)
        throw std::runtime_error("У игрока во время игры есть очки улучшения");
    if (!upgradePoints && mode == PlayerModes::UPGRADE)
        throw std::runtime_error("У игрока отсутствуют очки улучшения при переходе на следующий уровень");

    if (!data.contains("protectedSpells"))
        throw std::runtime_error("Отсутствует поле protectedSpells у игрока");
    if (!data["protectedSpells"].is_array())
        throw std::runtime_error("Значение поля protectedSpells у игрока не является массивом");
    int index;
    for (const auto& value : data["protectedSpells"]) {
        if (!value.is_number())
            throw std::runtime_error("В массиве защищённых заклинаний должны быть числа");
        index = value.get<int>();
        if (index < 0 || index >= spellsHand.getSpells().size())
            throw std::runtime_error("В массиве защищённых заклинаний присутствует некорректное значение");
        if (std::ranges::find(protectedSpells, index) != protectedSpells.end())
            throw std::runtime_error("В массиве защищённых заклинаний присутствует повторяющееся значение");
        protectedSpells.push_back(index);
    }
    if (mode != PlayerModes::UPGRADE && !protectedSpells.empty())
        throw std::runtime_error("Массив защищённых заклинаний не является пустым во время игры");
}

std::unique_ptr<Entity> Player::clone() const {
    return std::make_unique<Player>(*this);
}

json Player::to_json() const {
    json j = Entity::to_json();
    j["shootDamage"] = shootDamage;
    j["shootDiameter"] = shootDiameter;
    j["attackMode"] = mode;
    j["previousMode"] = previousMode;
    j["upgradePoints"] = upgradePoints;
    j["protectedSpells"] = protectedSpells;
    return j;
}

void Player::setMode(PlayerModes newMode) {
    if (newMode == mode)
        return;
    previousMode = mode;
    mode = newMode;
    if (mode == PlayerModes::RANGE || mode == PlayerModes::SPELL)
        setTarget(getPosition());
    if (mode == PlayerModes::MELEE)
        setTarget({-1, -1});
}

PlayerModes Player::getPreviousMode() const { return previousMode; }

PlayerModes Player::getMode() const { return mode; }

int Player::getShootRange() const { return static_cast<int>(shootDiameter); }

void Player::addHealth(int value) {
    setHealth(getHealth() + value);
}

void Player::addMaxHealth(int value) {
    setMaxHealth(getMaxHealth() + value);
}

void Player::addDamage(int value) {
    shootDamage += value;
    damage += value;
}

unsigned int Player::getDamage() const {
    if (mode == PlayerModes::RANGE)
        return shootDamage;
    return damage;
}

void Player::addUpgradePoints(unsigned int value) { upgradePoints += value; }

bool Player::hasUpgradePoints() const { return upgradePoints != 0; }

std::string Player::upgradeInfo() const {
    std::string upgradeInfo;
    auto spells = spellsHand.getSpells();
    upgradeInfo += "Количество очков улучшения: " + std::to_string(upgradePoints) + "\n";
    upgradeInfo += "Усовершенствуйте характеристики игрока или улучшите заклинание:\n";
    upgradeInfo += "Движение вверх. Увеличение максимального здоровья (+20)\n";
    upgradeInfo += "Движение вниз. Увеличение урона (+10)\n";
    int index = 1;
    if (spells.size() == 1) {
        upgradeInfo += "Заклинание " + std::to_string(index) + ". " + spells[0]->getName() + " (уровень " + std::to_string(spells[0]->getLevel()) + ") - повысить уровень\n";
    } else
        for (const auto& spell : spells) {
            upgradeInfo += "Заклинание " + std::to_string(index) + ". " + spell->getName() + " (уровень " + std::to_string(spell->getLevel()) + ") - ";
            if (std::ranges::find(protectedSpells, index++ - 1) == protectedSpells.end())
                upgradeInfo += "гарантированно защитить заклинание от утраты на один ход\n";
            else
                upgradeInfo += "повысить уровень\n";
        }
    return upgradeInfo;
}

void Player::upgrade(std::mt19937 &gen) {
    spellsHand.clearSelection();
    spellsHand.processNextLevel(protectedSpells, gen);
    protectedSpells.clear();
}

void Player::upgradeHealth() {
    addMaxHealth(20);
    upgradePoints--;
}

void Player::upgradeDamage() {
    addDamage(10);
    upgradePoints--;
}

bool Player::upgradeSpell(int index) {
    auto spells = spellsHand.getSpells();
    if (index >= spells.size()) return false;
    if (spells.size() > 1 && std::ranges::find(protectedSpells, index) == protectedSpells.end())
        protectedSpells.push_back(index);
    else {
        const auto& spell = spells[index];
        spell->setLevel(spell->getLevel() + 1);
    }
    upgradePoints--;
    return true;
}