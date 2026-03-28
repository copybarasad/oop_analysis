#ifndef GAME_SPELLMANAGER_H
#define GAME_SPELLMANAGER_H

#include <tuple>
#include <map>
#include "Spell.h"
#include "SpellFactory.h"
#include "../basics/Object.h"
#include "../basics/GameLogic.h"
#include "Mark.h"

class SpellManager {
private:
    SpellFactory factory;
    GameLogic *glogic_ = nullptr;
    int mana_ = 0;
    int spells_count_ = 0;
    int max_mana_ = 5;
    int max_spell_ = 8;
    int level_ = 0;
    int areaDx0_ = 0;
    int areaDy0_ = 0;
    int areaDx1_ = 0;
    int areaDy1_ = 0;
    Object *owner_ = nullptr;
    std::map<std::string, std::pair<Spell*, int> > spells_;
    std::vector<std::string> keys_;
    std::vector<Coordinates> targets_;
    int chosenTarget_ = 0;
    int chosenSpell_ = 0;
    std::vector<Mark *> marks_;
    std::vector<std::string> known_spells_ = {
        "Direct damage spell", "Area damage spell",
        "Trap spell", "Ally spell", "Improving spell"
    };

public:
    SpellManager() = default;

    void setOwner(GameLogic *glogic, Object *owner);

    void updateTargets();

    void changeTargets(int dx, int dy);

    void showTargets();

    void hideTargets();

    int getChosenSpellId() const;

    void changeSpell(int delta);

    int addSpell(Spell *spell);

    void removeSpell(int index);

    void changeMana(int mana);

    int getMana() const;

    int castSpell();

    std::vector<std::tuple<std::string, std::string, int, int> > getSpells();

    void changeLevel(int delta);

    int getLevel() const;

    bool haveSpells() const;

    void addRandomSpell();

    void removeRandomSpells();

    void saveToJson(nlohmann::json *savefile);

    void loadFromJson(nlohmann::json *savefile);
};


#endif
