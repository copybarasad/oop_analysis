#pragma once
#include "./Entities/Hero.hpp"
#include "./Field/Field.hpp"
#include "./Systems/GameLogger.hpp"
#include "./Spells/Fireball.hpp"
#include "./Spells/Absorption.hpp"
#include "./Spells/Trap.hpp"
#include <SFML/Graphics.hpp>

enum class HeroAction {
    None,
    MoveUp, MoveDown, MoveLeft, MoveRight,
    AttackUp, AttackDown, AttackLeft, AttackRight,
    SwitchWeapon,
    Spell
};


class HeroController {
private:
    Field* field;
    GameLogger& logger;
    Entity_Manager* manager;
    int const static CELL_SIZE = 57;

    
    bool awaitingSpellTarget = false;
    bool awaitingSpellSelection = false;
    bool spellCanceled = false;
    bool skipEnemyTurn = false;
    InterfaceSpell* currentSpell = nullptr;

public:
    HeroController(Field* field, GameLogger& logger, Entity_Manager* manager);

    bool handleInput(const sf::Event& event, Field& field);
    void executeAction(HeroAction action);

    HeroAction keyToAction(sf::Keyboard::Key key) const;

    bool handleMouseClick(sf::RenderWindow& window, Field& field);

    bool get_awaiting_spell_target() { return awaitingSpellTarget; }

    bool wasSpellCanceled() {
        bool temp = spellCanceled;
        spellCanceled = false;
        return temp;
    }

    void setSkipEnemyTurn(bool val) { skipEnemyTurn = val; }
    bool shouldSkipEnemyTurn() const { return skipEnemyTurn; }
    void update_controller(Entity_Manager* manager, Field* field);
};
