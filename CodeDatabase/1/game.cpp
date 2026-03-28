#include "game.h"
#include "event_system.h"
#include "game_events.h"
#include "game_saver.h"
#include "holy_water_spell.h"
#include "light_beam_spell.h"
#include "sacred_glow_spell.h"
#include "game_observer.h"
#include "command_registry.h"
#include "key_bindings_config.h"
#include <algorithm>
#include <iostream>
#include <sstream>
Game::Game(int width, int height, const std::string& hunter_name,
           EventSystem* event_system)
    : field_(width, height),
      hunter_(hunter_name, Position(1, 1)),
      dracula_(Position(width - 2, height - 2)),
      ui_event_manager_(),
      command_registry_(new CommandRegistry()),
      key_bindings_(new KeyBindingsConfig()),
      event_system_(event_system),
      game_running_(false),
      turn_count_(0),
      is_player_turn_(true),
      player_turn_completed_(false)
{
    ui_event_manager_.setEventSystem(event_system_);
    level_system_.applyLevelConfig(field_, hunter_, dracula_);

    if (event_system_) {
        key_bindings_->loadFromFile("key_bindings.cfg", event_system_);
    } else {
        key_bindings_->loadFromFile("key_bindings.cfg");
    }

    if (event_system_) {
        logGameEvent("ИГРА", "Создана новая игра: " + hunter_name);
    }
}

Game::~Game() {
    delete command_registry_;
    delete key_bindings_;
}

void Game::requestStatus() {
    ui_event_manager_.requestStatus();
    notifyObservers();
}

void Game::requestField() {
    ui_event_manager_.requestField();
    notifyObservers();
}

void Game::requestSpells() {
    if (canShowSpells()) {
        ui_event_manager_.requestSpells();
        notifyObservers();
    } else {
        logGameEvent("UI", "Нет заклинаний для показа");
    }
}

void Game::requestHelp() {
    ui_event_manager_.requestHelp();
    notifyObservers();
}

void Game::requestCommandHelp() {
    ui_event_manager_.requestCommandHelp();
    notifyObservers();
}

void Game::requestSpellSelection() {
    if (canSelectSpell()) {
        ui_event_manager_.requestSpellSelection();
        notifyObservers();
    } else {
        logGameEvent("UI", "Нет заклинаний для выбора");
    }
}

void Game::requestLibraryInteraction(const Position& pos) {
    if (getLibraryAtPosition(pos)) {
        ui_event_manager_.requestLibraryInteraction();
        notifyObservers();
    } else {
        logGameEvent("UI", "Нет библиотеки для взаимодействия");
    }
}

void Game::requestLevelUpInterface() {
    if (isLevelUpAvailable()) {
        ui_event_manager_.requestLevelUpInterface();
        notifyObservers();
    }
}

void Game::addObserver(GameObserver* observer) {
    if (observer) {
        observers_.push_back(observer);
    }
}

void Game::removeObserver(GameObserver* observer) {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end()
    );
}

void Game::notifyObservers() {
    for (GameObserver* observer : observers_) {
        observer->onGameStateChanged(*this);
    }
}

void Game::logDamage(const std::string& attacker, const std::string& target,
                    int damage, int remaining_hp) {
    if (event_system_) {
        DamageEvent event(attacker, target, damage, remaining_hp);
        event_system_->notify(event);
    }
}

void Game::logMove(const std::string& player, int x, int y,
                  const std::string& direction) {
    if (event_system_) {
        PlayerMoveEvent event(player, x, y, direction);
        event_system_->notify(event);
    }
}

void Game::logSpell(const std::string& caster, const std::string& spell_name) {
    if (event_system_) {
        SpellCastEvent event(caster, spell_name);
        event_system_->notify(event);
    }
}

void Game::logDeath(const std::string& character) {
    if (event_system_) {
        DeathEvent event(character);
        event_system_->notify(event);
    }
}

void Game::logGameEvent(const std::string& category, const std::string& message) {
    if (event_system_) {
        GeneralEvent event(category, message);
        event_system_->notify(event);
    }
}


GameContext Game::getContext() {
    return GameContext(field_, hunter_, dracula_);
}

ConstGameContext Game::getContext() const {
    return ConstGameContext(field_, hunter_, dracula_);
}

bool Game::moveHunter(int dx, int dy) {
    Position old_pos = hunter_.getPosition();
    
    if (hunter_.moveWithField(dx, dy, field_)) {
        Position new_pos = hunter_.getPosition();
        
        std::string direction;
        if (dx == 0 && dy == -1) direction = "ВВЕРХ";
        else if (dx == 0 && dy == 1) direction = "ВНИЗ";
        else if (dx == -1 && dy == 0) direction = "ВЛЕВО";
        else direction = "ВПРАВО";
        
        logMove(hunter_.getName(), new_pos.getX(), new_pos.getY(), direction);
        processCellEffects();
        processLibraryInteraction();
        
        player_turn_completed_ = true;
        notifyObservers();
        
        return true;
    }
    
    logGameEvent("ДВИЖЕНИЕ", hunter_.getName() + " не может двигаться");
    return false;
}

bool Game::attackDracula() {
    if (dracula_.isInvulnerable()) {
        logGameEvent("АТАКА", "Дракула неуязвим! Найдите его гробницу!");
        return false;
    }
    
    if (hunter_.canAttackAtPosition(dracula_.getPosition())) {
        std::string attack_type;
        int damage = 0;
        
        if (hunter_.isRangedMode()) {
            hunter_.useSilverBullet();
            attack_type = "серебряная пуля";
            damage = hunter_.getCurrentAttackDamage();
        } else {
            hunter_.useOakStake();
            attack_type = "осиновый кол";
            damage = hunter_.getCurrentAttackDamage();
        }
        
        logGameEvent("АТАКА", hunter_.getName() + " использовал " + attack_type);
        
        int old_health = dracula_.getHealth();
        dracula_.takeDamage(damage);
        
        logDamage(hunter_.getName(), dracula_.getName(), damage, dracula_.getHealth());
        
        player_turn_completed_ = true;
        notifyObservers();
        
        if (!dracula_.isAlive()) {
            logDeath(dracula_.getName());
            checkLevelCompletion();
        }
        
        return true;
    }
    
    logGameEvent("АТАКА", "Дракула вне досягаемости");
    return false;
}

bool Game::switchCombatMode() {
    hunter_.switchCombatMode();
    
    std::string mode = hunter_.isRangedMode() ? "ДАЛЬНИЙ БОЙ" : "БЛИЖНИЙ БОЙ";
    logGameEvent("РЕЖИМ", hunter_.getName() + " сменил режим на: " + mode);
    
    player_turn_completed_ = true;
    notifyObservers();
    
    return true;
}

bool Game::castSpell(int spell_index, const Position& target) {
    SpellHand& hand = hunter_.getSpellHand();
    
    if (spell_index < 0 || spell_index >= hand.getSpellCount()) {
        logGameEvent("ЗАКЛИНАНИЕ", "Неверный индекс заклинания");
        return false;
    }
    
    Spell* spell = hand.getSpell(spell_index);
    if (!spell) {
        logGameEvent("ЗАКЛИНАНИЕ", "Заклинание не найдено");
        return false;
    }
    
    if (!spell->canCast(hunter_.getPosition(), target)) {
        logGameEvent("ЗАКЛИНАНИЕ", "Цель вне радиуса действия");
        return false;
    }
    
    SpellCastResult result = spell->calculateCast(hunter_.getPosition(), target);
    applySpellEffects(result);
    hand.removeSpell(spell_index);
    
    logSpell(hunter_.getName(), spell->getName());
    
    player_turn_completed_ = true;
    notifyObservers();
    
    return true;
}

void Game::processEnemyTurn() {
    turn_count_++;
    is_player_turn_ = false;
    player_turn_completed_ = false;
    
    logGameEvent("ХОД", "Ход врагов");
    
    bool dracula_moved = dracula_.makeMove(hunter_.getPosition(), 
                                          hunter_.getResearchProgress(), 
                                          field_);
    
    if (dracula_moved) {
        Position new_pos = dracula_.getPosition();
        logMove("Дракула", new_pos.getX(), new_pos.getY(), "авто");
    }
    
    if (dracula_.getPosition().manhattanDistance(hunter_.getPosition()) == 1) {
        hunter_.takeDamage(15);
        logDamage("Дракула", hunter_.getName(), 15, hunter_.getHealth());
    }
    
    processTowerTurns();
    processTrapEffects();
    field_.applyCellEffect(dracula_);
    checkWinConditions();
    is_player_turn_ = true;
    notifyObservers();
}

void Game::advanceToNextLevel() {
    if (level_system_.hasMoreLevels()) {
        level_system_.advanceToNextLevel();
        level_system_.applyLevelConfig(field_, hunter_, dracula_);
        turn_count_ = 0;
        player_turn_completed_ = false;
        notifyObservers();
    }
}
void Game::processCellEffects() {
    Position hunter_pos = hunter_.getPosition();
    CellType cell = field_.getCellType(hunter_pos);
    
    if (cell == CellType::DRACULA_TOMB && dracula_.isInvulnerable()) {
        dracula_.setInvulnerable(false);
        logGameEvent("СОБЫТИЕ", "Найдена гробница Дракулы! Теперь он уязвим!");
    }
    
    field_.applyCellEffect(hunter_);
}

void Game::processLibraryInteraction() {
    Position hunter_pos = hunter_.getPosition();
    Library* library = field_.getLibraryAt(hunter_pos);
    if (!library) return;
    
    bool has_spell = hunter_.getSpellHand().wasSpellEverObtained(library->getAvailableSpell());
    bool is_full = hunter_.getSpellHand().isFull();
    
    LibraryInteractionResult result = library->checkInteraction(has_spell, is_full);
    
    switch (result) {
        case LibraryInteractionResult::NOTHING:
            logGameEvent("БИБЛИОТЕКА", "Библиотека уже изучена");
            break;
            
        case LibraryInteractionResult::AUTO_RESEARCH:
            hunter_.doResearch();
            library->confirmInteraction();
            logGameEvent("БИБЛИОТЕКА", hunter_.getName() + " получил +1 исследование");
            break;
            
        case LibraryInteractionResult::CAN_CHOOSE:
            ui_event_manager_.requestLibraryInteraction();
            logGameEvent("БИБЛИОТЕКА", "Найдена библиотека с выбором");
            notifyObservers();
            break;
    }
}

void Game::processTrapEffects() {
    Position dracula_pos = dracula_.getPosition();
    
    if (field_.hasTrap(dracula_pos)) {
        int damage = field_.triggerTrap(dracula_pos);
        dracula_.takeDamage(damage);
        logDamage("Ловушка", dracula_.getName(), damage, dracula_.getHealth());
    }
}

void Game::processTowerTurns() {
    int towers_attacked = 0;
    
    for (VampireTower& tower : const_cast<std::vector<VampireTower>&>(field_.getTowers())) {
        if (tower.isAlive()) {
            tower.decreaseCooldown();
            
            if (tower.canAttack(hunter_.getPosition()) && tower.isReadyToAttack()) {
                SpellCastResult attack_result = tower.performAttack(hunter_.getPosition());
                if (attack_result.damage > 0) {
                    hunter_.takeDamage(attack_result.damage);
                    logDamage("Вампирская башня", hunter_.getName(), 
                             attack_result.damage, hunter_.getHealth());
                    towers_attacked++;
                }
            }
        }
    }
    
    if (towers_attacked > 0) {
        logGameEvent("БАШНИ", "Атаковало башен: " + std::to_string(towers_attacked));
    }
}

void Game::checkWinConditions() {
    if (!dracula_.isAlive()) {
        return;
    }
    
    if (!hunter_.isAlive()) {
        logDeath(hunter_.getName());
        game_running_ = false;
        logGameEvent("ИГРА", "ПОРАЖЕНИЕ! ОХОТНИК ПОГИБ!");
        notifyObservers();
    }
}

void Game::checkLevelCompletion() {
    if (!dracula_.isAlive() && level_system_.hasMoreLevels()) {
        logGameEvent("УРОВЕНЬ", "Уровень " + std::to_string(level_system_.getCurrentLevel()) + " пройден");
        requestLevelUpInterface();
    }
    else if (!dracula_.isAlive() && level_system_.isFinalLevel()) {
        game_running_ = false;
        logGameEvent("ПОБЕДА", "Дракула уничтожен навсегда!");
        notifyObservers();
    }
}  


void Game::applySpellEffects(const SpellCastResult& result) {
    for (const Position& pos : result.affectedPositions) {
        if (!field_.isValidPosition(pos)) continue;
        
        if (result.isTrap) {
            if (field_.placeTrap(pos, result.damage)) {
                logGameEvent("ЛОВУШКА", "Установлена ловушка на (" + 
                           std::to_string(pos.getX()) + "," + std::to_string(pos.getY()) + ")");
            }
        } else {
            if (pos == dracula_.getPosition()) {
                dracula_.takeDamage(result.damage);
                logDamage("Заклинание", dracula_.getName(), result.damage, dracula_.getHealth());
            }
            
            for (VampireTower& tower : const_cast<std::vector<VampireTower>&>(field_.getTowers())) {
                if (tower.isAlive() && tower.getPosition() == pos) {
                    tower.takeDamage(result.damage);
                    logDamage("Заклинание", "Вампирская башня", result.damage, tower.getHealth());
                    
                    if (!tower.isAlive()) {
                        logGameEvent("БАШНЯ", "Вампирская башня разрушена");
                    }
                }
            }
        }
    }
}

bool Game::processLibraryChoice(int choice, const Position& library_pos) {
    Library* library = field_.getLibraryAt(library_pos);
    if (!library) {
        logGameEvent("БИБЛИОТЕКА", "Библиотека не найдена");
        return false;
    }
    
    if (choice == 1) {
        Spell* newSpell = nullptr;
        SpellType spellType = library->getAvailableSpell();
        
        switch (spellType) {
            case SpellType::HOLY_WATER: 
                newSpell = new HolyWaterSpell(); 
                break;
            case SpellType::LIGHT_BEAM: 
                newSpell = new LightBeamSpell(); 
                break;
            case SpellType::SACRED_GLOW: 
                newSpell = new SacredGlowSpell(); 
                break;
            default:
                logGameEvent("БИБЛИОТЕКА", "Неизвестный тип заклинания: " + 
                           std::to_string(static_cast<int>(spellType)));
                return false;
        }
        
        if (!newSpell) {
            logGameEvent("БИБЛИОТЕКА", "Не удалось создать заклинание");
            return false;
        }
        
        bool added = false;
        try {
            added = hunter_.getSpellHand().tryAddSpell(newSpell);
        } catch (const std::exception& e) {
            logGameEvent("БИБЛИОТЕКА", "Ошибка добавления заклинания: " + 
                       std::string(e.what()));
            delete newSpell;
            return false;
        }
        
        if (added) {
            logGameEvent("БИБЛИОТЕКА", "Изучено заклинание: " + newSpell->getName());
            library->confirmInteraction();
        } else {
            logGameEvent("БИБЛИОТЕКА", "Не удалось изучить заклинание (рука полна?)");
            delete newSpell;  // Удаляем, так как tryAddSpell не взял владение
        }
        
        notifyObservers();
        return added;
    } 
    else if (choice == 2) {
        hunter_.doResearch();
        library->confirmInteraction();
        logGameEvent("БИБЛИОТЕКА", "Проведено исследование Дракулы");
        notifyObservers();
        return true;
    }
    
    logGameEvent("БИБЛИОТЕКА", "Неверный выбор: " + std::to_string(choice));
    return false;
}

bool Game::processSpellSelection(int spell_index, const Position& target) {
    return castSpell(spell_index, target);
}
bool Game::processUpgradeChoice(int upgrade_id) {
    try {
        auto upgrade_type = upgrade_system_.getUpgradeTypeById(upgrade_id);
        upgrade_system_.applyUpgrade(upgrade_type, hunter_);
        logGameEvent("ПРОКАЧКА", "Применено улучшение");
        
        if (!dracula_.isAlive() && level_system_.hasMoreLevels()) {
            advanceToNextLevel();
        }
        
        return true;
    } catch (const std::exception& e) {
        logGameEvent("ОШИБКА", "Не удалось применить улучшение: " + std::string(e.what()));
        return false;
    }
}

Library* Game::getLibraryAtPosition(const Position& pos) {
    return field_.getLibraryAt(pos);
}

bool Game::isLibraryInteractable(const Position& pos) const {
    const Library* library = field_.getLibraryAt(pos);
    if (!library) return false;
    
    bool has_spell = hunter_.getSpellHand().wasSpellEverObtained(library->getAvailableSpell());
    bool is_full = hunter_.getSpellHand().isFull();
    
    LibraryInteractionResult result = library->checkInteraction(has_spell, is_full);
    return result == LibraryInteractionResult::CAN_CHOOSE;
}

bool Game::isSpellSelectionAvailable() const {
    return hunter_.getSpellHand().getSpellCount() > 0;
}

bool Game::isLevelUpAvailable() const {
    return !dracula_.isAlive() && level_system_.hasMoreLevels();
}

const std::vector<PlayerUpgradeSystem::UpgradeOption>& Game::getUpgradeOptions() const {
    return upgrade_system_.getAvailableUpgrades();
}
