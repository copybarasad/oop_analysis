#include "HeroController.hpp"
#include "./Spells/SpellHand.hpp"


HeroController::HeroController(Field* field, GameLogger& logger, Entity_Manager* manager)
    : field(field), logger(logger), manager(manager) {}




bool HeroController::handleInput(const sf::Event& event, Field& field) {
    Hero* hero = manager->get_hero();
    if (!hero) {
        std::cout << "ERROR: Hero is null in executeAction!" << std::endl;
    }

    if (awaitingSpellTarget) { 
        if (event.key.code == sf::Keyboard::Num0) {
            logger.log("spell cast canceled.\n");
            awaitingSpellTarget = false;
            spellCanceled = true;
            return true;
        }
    }
    else if (awaitingSpellSelection) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Num0) {
                logger.log("Spell selection canceled.\n");
                awaitingSpellSelection = false;
                spellCanceled = true;
                return true;
            }

            InterfaceSpell* spell = nullptr;
            switch (event.key.code) {
                case sf::Keyboard::Num1:
                    spell = hero->get_SpellHand().getSpellByIndex(1, logger);
                    break;
                case sf::Keyboard::Num2:
                    spell = hero->get_SpellHand().getSpellByIndex(2, logger);
                    break;
                case sf::Keyboard::Num3:
                    spell = hero->get_SpellHand().getSpellByIndex(3, logger);
                    break;
                case sf::Keyboard::Num4:
                    spell = hero->get_SpellHand().getSpellByIndex(4, logger);
                    break;
                case sf::Keyboard::Num5:
                    spell = hero->get_SpellHand().getSpellByIndex(5, logger);
                    break;
                default:
                    return false;
            }

            if (spell) {
                currentSpell = spell;
                awaitingSpellSelection = false;

                if (spell->requiresTarget()) {
                    awaitingSpellTarget = true;
                    logger.log("Select a target for the " + spell->getName() + ".\n");
                } else {
                    bool success = spell->cast(*hero, field, logger, 0, 0, manager);
                    if (!success)
                        setSkipEnemyTurn(true);
                }
            }
            return true;
        }
    }

    else {
        if (event.type == sf::Event::KeyPressed) {
            HeroAction action = keyToAction(event.key.code);
            if (action != HeroAction::None) {
                executeAction(action);
                
                if (awaitingSpellSelection) {
                    return false;
                }
                
                return true;
            }
        }
    }

    return false;
}

HeroAction HeroController::keyToAction(sf::Keyboard::Key key) const {
    switch (key) {
        case sf::Keyboard::W: return HeroAction::MoveUp;
        case sf::Keyboard::S: return HeroAction::MoveDown;
        case sf::Keyboard::A: return HeroAction::MoveLeft;
        case sf::Keyboard::D: return HeroAction::MoveRight;
        case sf::Keyboard::Up: return HeroAction::AttackUp;
        case sf::Keyboard::Down: return HeroAction::AttackDown;
        case sf::Keyboard::Left: return HeroAction::AttackLeft;
        case sf::Keyboard::Right: return HeroAction::AttackRight;
        case sf::Keyboard::R: return HeroAction::SwitchWeapon;
        case sf::Keyboard::M: return HeroAction::Spell;
        default: return HeroAction::None;
    }
}


void HeroController::executeAction(HeroAction action) {
    Hero* hero = manager->get_hero();
    if (!hero) {
        std::cout << "ERROR: Hero is null in executeAction!" << std::endl;
        return;
    }
    auto& cells = field->get_cells();

    if (awaitingSpellTarget) return;

    switch (action) {
        case HeroAction::MoveUp: hero->move(0, -1, *field, logger); break;
        case HeroAction::MoveDown: hero->move(0, 1, *field, logger); break;
        case HeroAction::MoveLeft: hero->move(-1, 0, *field, logger); break;
        case HeroAction::MoveRight: hero->move(1, 0, *field, logger); break;

        case HeroAction::AttackUp: hero->hero_attack_range(cells, forward, logger); break;
        case HeroAction::AttackDown: hero->hero_attack_range(cells, backward, logger); break;
        case HeroAction::AttackLeft: hero->hero_attack_range(cells, left, logger); break;
        case HeroAction::AttackRight: hero->hero_attack_range(cells, right, logger); break;

        case HeroAction::SwitchWeapon:
            hero->Switching_weapons(logger);
            break;


        case HeroAction::Spell: 
            awaitingSpellSelection = true;
            logger.log("Select a spell (1 - " + std::to_string(hero->get_SpellHand().getCount()) + " ) or press 0 to cancel.\n");
            break;



        case HeroAction::None: break;
    }
}


bool HeroController::handleMouseClick(sf::RenderWindow& window, Field& field) {
    Hero* hero = manager->get_hero();
    if (awaitingSpellTarget && currentSpell) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int cellX = (mousePos.x - field.get_offset().x) / CELL_SIZE;
        int cellY = (mousePos.y - field.get_offset().y) / CELL_SIZE;

        if (cellX >= 0 && cellX < int(field.get_grid_size().x) && cellY >= 0 && cellY < int(field.get_grid_size().y)) {

            
            bool success = currentSpell->cast(*hero, field, logger, cellX, cellY, manager);

            awaitingSpellTarget = false;
            currentSpell = nullptr;

            if (!success)
                setSkipEnemyTurn(true);

            return true;
        }
    }
    return false;
}


void HeroController::update_controller(Entity_Manager* manager, Field* field) {
        this->field = field;
        this->manager = manager;
    }