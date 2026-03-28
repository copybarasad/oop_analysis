#include "hand.h"
#include "SpellDirectDamage.h"
#include "SpellAreaDamage.h"
#include "SpellTrap.h"
#include "GameEventManager.h"  // Добавлено
#include <sstream>
#include <iostream>

Hand::Hand(int max_spells) : max_hand_spells(max_spells), num_spells(0) {
    hand_buffer.reserve(max_spells);

    // Логирование создания руки
    LOG_DEBUG("Hand created with max capacity: " + std::to_string(max_spells));
}

void Hand::Clear() {
    // Логирование перед очисткой
    int oldCount = num_spells;
    LOG_DEBUG("Clearing hand, current spell count: " + std::to_string(oldCount));

    hand_buffer.clear();
    num_spells = 0;

    // Логирование после очистки
    LOG_INFO("Hand cleared, removed " + std::to_string(oldCount) + " spells");
}

bool Hand::AddRandomSpell() {
    // Логирование перед добавлением
    LOG_DEBUG("Attempting to add random spell, current count: " +
        std::to_string(num_spells) + "/" + std::to_string(max_hand_spells));

    if (num_spells >= max_hand_spells) {
        LOG_WARNING("Cannot add random spell - hand is full");
        return 0;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);

    int random_spell = distrib(gen);
    std::string spellName;

    switch (random_spell) {
    case 0:
        hand_buffer.push_back(std::make_unique<SpellDirectDamage>(30, 3));
        spellName = "Direct Damage Spell";
        break;
    case 1:
        hand_buffer.push_back(std::make_unique<SpellAreaDamage>(20, 5));
        spellName = "Area Damage Spell";
        break;
    case 2:
        hand_buffer.push_back(std::make_unique<SpellTrap>(25, 4));
        spellName = "Trap Spell";
        break;
    }

    num_spells++;

    // Логирование успешного добавления
    LOG_INFO("Random spell added: " + spellName);
    LOG_DEBUG("New spell count: " + std::to_string(num_spells));

    // Логирование через GameEventManager
    GameEventManager::getInstance().logPlayerSpellGained(spellName);

    return 1;
}

bool Hand::AddSpell(std::unique_ptr<Spell> spell) {
    // Логирование перед добавлением
    LOG_DEBUG("Attempting to add specific spell, current count: " +
        std::to_string(num_spells) + "/" + std::to_string(max_hand_spells));

    if (num_spells >= max_hand_spells) {
        LOG_WARNING("Cannot add spell - hand is full");
        return false;
    }

    std::string spellInfo = spell ? spell->GetInfo() : "Unknown Spell";

    hand_buffer.push_back(std::move(spell));
    num_spells++;

    // Логирование успешного добавления
    LOG_INFO("Specific spell added: " + spellInfo);
    LOG_DEBUG("New spell count: " + std::to_string(num_spells));

    return true;
}

void Hand::Display() {
    // Логирование
    LOG_DEBUG("Displaying hand contents, spell count: " + std::to_string(num_spells));

    for (int i = 0; i < num_spells; i++) {
        std::string spellInfo = hand_buffer[i]->GetInfo();
        std::cout << (i + 1) << ". " << spellInfo << std::endl;

        // Логирование каждого заклинания
        LOG_DEBUG("Spell " + std::to_string(i + 1) + ": " + spellInfo);
    }
}

int Hand::CountSpells() const {
    // Логирование (если нужно часто вызывается, можно закомментировать)
    // LOG_DEBUG("CountSpells called: " + std::to_string(num_spells));
    return num_spells;
}

std::string Hand::GetSpellInfo(int index) const {
    // Логирование
    LOG_DEBUG("GetSpellInfo called for index: " + std::to_string(index));

    if (index >= 0 && index < num_spells && hand_buffer[index] != nullptr) {
        std::string info = hand_buffer[index]->GetInfo();
        LOG_DEBUG("Spell info retrieved: " + info);
        return info;
    }

    LOG_WARNING("Invalid spell index requested: " + std::to_string(index));
    return "Invalid spell index: " + std::to_string(index);
}

void Hand::DisplaySpellInfo() const {
    // Логирование
    LOG_DEBUG("DisplaySpellInfo called");

    std::cout << "Hand contains " << num_spells << " spells:" << std::endl;
    for (int i = 0; i < num_spells; i++) {
        std::string spellInfo = hand_buffer[i]->GetInfo();
        std::cout << "  Spell " << (i + 1) << ": " << spellInfo << std::endl;

        // Логирование каждого заклинания
        LOG_DEBUG("Displayed spell " + std::to_string(i + 1) + ": " + spellInfo);
    }
}

std::unique_ptr<Spell> Hand::ChooseSpell(int index) {
    // Логирование перед выбором
    LOG_DEBUG("ChooseSpell called for index: " + std::to_string(index));

    if (index < 0 || index >= num_spells) {
        LOG_WARNING("Invalid spell index for ChooseSpell: " + std::to_string(index));
        return nullptr;
    }

    // Получаем информацию о заклинании перед перемещением
    std::string spellInfo = hand_buffer[index] ? hand_buffer[index]->GetInfo() : "Unknown Spell";

    auto spell = std::move(hand_buffer[index]);
    hand_buffer.erase(hand_buffer.begin() + index);
    num_spells--;

    // Логирование после выбора
    LOG_INFO("Spell chosen and removed: " + spellInfo);
    LOG_DEBUG("Remaining spells: " + std::to_string(num_spells));

    return spell;
}

void Hand::AddSpellForAchievement(int enemies_defeated) {
    // Логирование
    LOG_DEBUG("AddSpellForAchievement called, enemies defeated: " + std::to_string(enemies_defeated));

    if (enemies_defeated % 3 == 0 && num_spells < max_hand_spells) {
        AddRandomSpell();
        std::cout << "You received a new spell for defeating " << enemies_defeated << " enemies!" << std::endl;

        // Логирование награды
        LOG_INFO("Achievement reward: new spell for defeating " + std::to_string(enemies_defeated) + " enemies");
    }
    else {
        LOG_DEBUG("No achievement reward - condition not met or hand is full");
    }
}

void Hand::UpgradeSpellDamage(int spellIndex, int damageIncrease) {
    // Логирование перед улучшением
    LOG_DEBUG("UpgradeSpellDamage called for index: " + std::to_string(spellIndex) +
        ", damage increase: " + std::to_string(damageIncrease));

    if (spellIndex >= 0 && spellIndex < num_spells && hand_buffer[spellIndex] != nullptr) {
        int oldDamage = hand_buffer[spellIndex]->GetDamage();

        hand_buffer[spellIndex]->UpgradeDamage(damageIncrease);

        int newDamage = hand_buffer[spellIndex]->GetDamage();
        std::cout << "Spell " << (spellIndex + 1) << " upgraded! New damage: "
            << hand_buffer[spellIndex]->GetDamage() << std::endl;

        // Логирование улучшения
        LOG_INFO("Spell " + std::to_string(spellIndex + 1) + " upgraded");
        LOG_DEBUG("Damage: " + std::to_string(oldDamage) + " -> " + std::to_string(newDamage));
    }
    else {
        LOG_WARNING("Cannot upgrade spell - invalid index or null spell: " + std::to_string(spellIndex));
    }
}

std::string Hand::SaveToString() const {
    // Логирование перед сохранением
    LOG_DEBUG("Hand SaveToString called, saving " + std::to_string(num_spells) + " spells");

    std::ostringstream ss;
    ss << "spell_count:" << num_spells << "\n";

    for (int i = 0; i < num_spells; i++) {
        if (hand_buffer[i] != nullptr) {
            std::string spellType = "";
            int damage = hand_buffer[i]->GetDamage();
            int range = hand_buffer[i]->GetRange();

            std::string info = hand_buffer[i]->GetInfo();
            if (info.find("Direct Damage") != std::string::npos) {
                spellType = "direct";
            }
            else if (info.find("Area Damage") != std::string::npos) {
                spellType = "area";
            }
            else if (info.find("Trap Spell") != std::string::npos) {
                spellType = "trap";
            }

            ss << "spell_" << i << "_type:" << spellType << "\n";
            ss << "spell_" << i << "_damage:" << damage << "\n";
            ss << "spell_" << i << "_range:" << range << "\n";

            // Логирование каждого сохраненного заклинания
            LOG_DEBUG("Saved spell " + std::to_string(i) + ": " + spellType +
                " (damage: " + std::to_string(damage) + ", range: " + std::to_string(range) + ")");
        }
    }

    std::string result = ss.str();
    LOG_DEBUG("Hand save string created, length: " + std::to_string(result.length()) + " chars");

    return ss.str();
}

void Hand::LoadFromString(const std::string& data) {
    // Логирование перед загрузкой
    LOG_DEBUG("Hand LoadFromString called, data length: " + std::to_string(data.length()));

    Clear();

    std::istringstream ss(data);
    std::string line;
    int spellCount = 0;

    while (std::getline(ss, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        if (key == "spell_count") {
            spellCount = std::stoi(value);
            LOG_DEBUG("Found spell count in save data: " + std::to_string(spellCount));
            break;
        }
    }

    LOG_INFO("Loading " + std::to_string(spellCount) + " spells from save");

    for (int i = 0; i < spellCount; i++) {
        std::string spellType = "";
        int damage = 0;
        int range = 0;

        ss.clear();
        ss.seekg(0);

        while (std::getline(ss, line)) {
            size_t colonPos = line.find(':');
            if (colonPos == std::string::npos) continue;

            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            if (key == "spell_" + std::to_string(i) + "_type") {
                spellType = value;
                LOG_DEBUG("Loaded spell type for index " + std::to_string(i) + ": " + value);
            }
            else if (key == "spell_" + std::to_string(i) + "_damage") {
                damage = std::stoi(value);
                LOG_DEBUG("Loaded spell damage for index " + std::to_string(i) + ": " + value);
            }
            else if (key == "spell_" + std::to_string(i) + "_range") {
                range = std::stoi(value);
                LOG_DEBUG("Loaded spell range for index " + std::to_string(i) + ": " + value);
            }
        }

        if (spellType == "direct") {
            hand_buffer.push_back(std::make_unique<SpellDirectDamage>(damage, range));
            LOG_DEBUG("Created DirectDamage spell for index " + std::to_string(i));
        }
        else if (spellType == "area") {
            hand_buffer.push_back(std::make_unique<SpellAreaDamage>(damage, range));
            LOG_DEBUG("Created AreaDamage spell for index " + std::to_string(i));
        }
        else if (spellType == "trap") {
            hand_buffer.push_back(std::make_unique<SpellTrap>(damage, range));
            LOG_DEBUG("Created Trap spell for index " + std::to_string(i));
        }

        num_spells++;
    }

    // Логирование после загрузки
    LOG_INFO("Hand loaded successfully, total spells: " + std::to_string(num_spells));
}