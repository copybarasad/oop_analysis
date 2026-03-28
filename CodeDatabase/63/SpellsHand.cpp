#include "SpellsHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"

SpellsHand::SpellsHand(unsigned int spellsHandSize) : maximumSize(std::clamp<unsigned int>(spellsHandSize, 0, 10)), selectedSpell(-1) {
    spells.reserve(maximumSize);
}

SpellsHand::SpellsHand(json &data) {
    if (!data.contains("maximumSize"))
        throw std::runtime_error("Отсутствует поле maximumSize у руки заклинаний");
    if (!data["maximumSize"].is_number_unsigned())
        throw std::runtime_error("Значение поля maximumSize у руки заклинаний не является беззнаковым числом");
    maximumSize = data["maximumSize"].get<unsigned int>();
    if (maximumSize > 10)
        throw std::runtime_error("Значение поля maximumSize у руки заклинаний принимает некорректное значение");

    if (!data.contains("selectedSpell"))
        throw std::runtime_error("Отсутствует поле selectedSpell у руки заклинаний");
    if (!data["selectedSpell"].is_number_integer())
        throw std::runtime_error("Значение поля selectedSpell у руки заклинаний не является числом");
    selectedSpell = data["selectedSpell"].get<int>();
    if (!(-1 <= selectedSpell && selectedSpell < static_cast<int>(maximumSize)))
        throw std::runtime_error("Значение поля selectedSpell у руки заклинаний принимает некорректное значение");

    if (!data.contains("spells"))
        throw std::runtime_error("Отсутствует поле spells у руки заклинаний");
    if (!data["spells"].is_array())
        throw std::runtime_error("Значение поля spells у руки заклинаний не является массивом");
    if (maximumSize < data["spells"].size())
        throw std::runtime_error("Количество заклинаний превышает вместимость руки заклинаний");
    spells.reserve(maximumSize);
    for (auto spellData: data["spells"]) {
        std::shared_ptr<ISpell> spell = nullptr;
        if (!spellData.contains("name"))
            throw std::runtime_error("Отсутствует поле name у заклинания");
        if (!spellData["name"].is_string())
            throw std::runtime_error("Значение поля name у заклинания не является строкой");
        std::string name = spellData["name"].get<std::string>();
        if (name == "Урон по площади")
            spell = std::make_shared<AreaDamageSpell>(spellData);
        else if (name == "Прямой урон")
            spell = std::make_shared<DirectDamageSpell>(spellData);
        else if (name == "Мимик (ловушка)")
            spell = std::make_shared<TrapSpell>(spellData);
        else if (name == "Призыв союзников")
            spell = std::make_shared<SummonSpell>(spellData);
        else if (name == "Улучшение")
            spell = std::make_shared<UpgradeSpell>(spellData);
        if (spell)
            spells.push_back(spell);
        else
            throw std::runtime_error("Нестандартное название заклинания");
    }
    if (selectedSpell >= static_cast<int>(spells.size()))
        throw std::runtime_error("Значение поля selectedSpell у руки заклинаний принимает значение большее, чем определено заклинаний");
}

SpellsHand::SpellsHand(const SpellsHand& other) {
    maximumSize = other.maximumSize;
    selectedSpell = other.selectedSpell;
    spells.reserve(other.spells.size());
    for (const auto& spell : other.spells)
        spells.push_back(spell->clone());
}

SpellsHand& SpellsHand::operator=(const SpellsHand& other) {
    if (this == &other) return *this;
    maximumSize = other.maximumSize;
    selectedSpell = other.selectedSpell;
    spells.clear();
    spells.reserve(maximumSize);
    for (const auto& spell : other.spells)
        spells.push_back(spell->clone());
    return *this;
}

json SpellsHand::to_json() const {
    json j;
    j["maximumSize"] = maximumSize;
    j["selectedSpell"] = selectedSpell;
    std::vector<json> spells_json;
    spells_json.reserve(spells.size());
    for (const auto& spell : spells)
        spells_json.push_back(spell->to_json());
    j["spells"] = spells_json;
    return j;
}

void SpellsHand::addSpell(std::shared_ptr<ISpell> spell) {
    if (spells.size() < maximumSize)
        spells.push_back(std::move(spell));
}

std::vector<std::shared_ptr<ISpell>> SpellsHand::getSpells() const { return spells; }

bool SpellsHand::empty() const { return spells.empty(); }

void SpellsHand::reload(bool full) const {
    for (auto& spell : spells)
        if (full)
            spell->fullReload();
        else
            spell->reload();
}

ISpell* SpellsHand::getFirstReadySpell() const {
    for (const auto& spell : spells)
        if (spell->isReady())
            return spell.get();
    return nullptr;
}

std::vector<ISpell*> SpellsHand::getReadySpells() const {
    std::vector<ISpell*> result;
    for (const auto& spell : spells) {
        if (spell->isReady())
            result.push_back(spell.get());
    }
    return result;
}

ISpell* SpellsHand::selectSpell(int spellIndex) {
    if (spellIndex >= spells.size())
        return nullptr;
    if (!spells[spellIndex]->isReady())
        return nullptr;
    selectedSpell = spellIndex;
    return spells[selectedSpell].get();
}

void SpellsHand::clearSelection() { selectedSpell = -1; }

ISpell* SpellsHand::getSelectedSpell() {
    if (selectedSpell == -1)
        return nullptr;
    ISpell* spell = spells[selectedSpell].get();
    if (!spell->isReady()) {
        selectedSpell = -1;
        return nullptr;
    }
    return spell;
}

unsigned int SpellsHand::getMaxSize() const { return maximumSize; }

void SpellsHand::addRandomSpell(std::mt19937& gen) {
    std::vector<std::shared_ptr<ISpell>> availableSpells;
    availableSpells.push_back(std::make_unique<DirectDamageSpell>(7, 8));
    availableSpells.push_back(std::make_unique<AreaDamageSpell>(7, 10));
    availableSpells.push_back(std::make_unique<TrapSpell>(7, 10));
    availableSpells.push_back(std::make_unique<SummonSpell>(3, 15));
    availableSpells.push_back(std::make_unique<UpgradeSpell>(10));
    std::ranges::shuffle(availableSpells.begin(), availableSpells.end(), gen);
    spells.push_back(std::move(availableSpells[0]));
}

void SpellsHand::processNextLevel(const std::vector<int> &protectedSpells, std::mt19937 gen) {
    for (auto& spell : spells)
        if (spell->getLevel() > 1)
            spell->setLevel(spell->getLevel() / 2);

    if (spells.size() <= 1) return;


    std::vector<int> removable;
    removable.reserve(spells.size());

    std::vector<int> saved;
    saved.reserve(protectedSpells.size());

    for (int i = 0; i < spells.size(); ++i)
        if (std::ranges::find(protectedSpells, i) == protectedSpells.end())
            removable.push_back(i);
        else
            saved.push_back(i);

    std::ranges::shuffle(removable, gen);

    size_t countToRemove = std::min(spells.size() / 2, removable.size());
    removable.resize(removable.size() - countToRemove);

    std::vector<int> keep;
    keep.reserve(saved.size() + removable.size() - countToRemove);
    keep.insert(keep.end(), saved.begin(), saved.end());
    keep.insert(keep.end(), removable.begin(), removable.end());
    std::ranges::sort(keep);

    int count = 0;
    for (int i : keep)
        spells[count++] = spells[i];
    spells.resize(count);
    reload(true);
}
