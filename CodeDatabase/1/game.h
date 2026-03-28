#pragma once

#include "field.h"
#include "vampire_hunter.h"
#include "dracula.h"
#include "level_system.h"
#include "game_context.h"
#include "player_upgrade_system.h"
#include "ui_event_manager.h"
#include <string>
#include <vector>

class EventSystem;
class GameObserver;
class CommandRegistry;
class KeyBindingsConfig;

class Game {
private:
    Field field_;
    VampireHunter hunter_;
    Dracula dracula_;
    LevelSystem level_system_;
    PlayerUpgradeSystem upgrade_system_;
    UIEventManager ui_event_manager_;
    CommandRegistry* command_registry_;
    KeyBindingsConfig* key_bindings_;
    EventSystem* event_system_;
    
    bool game_running_;
    int turn_count_;
    bool is_player_turn_;
    bool player_turn_completed_;
    std::vector<GameObserver*> observers_;
    
    void logDamage(const std::string& attacker, const std::string& target,
                  int damage, int remaining_hp);
    void logMove(const std::string& player, int x, int y,
                const std::string& direction);
    void logSpell(const std::string& caster, const std::string& spell_name);
    void logDeath(const std::string& character);
    void logGameEvent(const std::string& category, const std::string& message);
    
    void processCellEffects();
    void processLibraryInteraction();
    void processTrapEffects();
    void processTowerTurns();
    void checkWinConditions();
    void checkLevelCompletion();
    
    void applySpellEffects(const SpellCastResult& result);

public:
    Game(int width, int height, const std::string& hunter_name,
         EventSystem* event_system = nullptr);
    ~Game();
    
    bool moveHunter(int dx, int dy);
    bool attackDracula();
    bool switchCombatMode();
    bool castSpell(int spell_index, const Position& target);
    void processEnemyTurn();
    void advanceToNextLevel();
    
    void requestStatus();
    void requestField();
    void requestSpells();
    void requestHelp();
    void requestCommandHelp();
    void requestSpellSelection();
    void requestLibraryInteraction(const Position& pos);
    void requestLevelUpInterface();
    
    void startGame() { 
        game_running_ = true; 
        turn_count_ = 0;
        is_player_turn_ = true;
        player_turn_completed_ = false;
    }
    void stopGame() { game_running_ = false; }
    

    const Field& getField() const { return field_; }
    Field& getField() { return field_; }
    
    const VampireHunter& getHunter() const { return hunter_; }
    VampireHunter& getHunter() { return hunter_; }
    
    const Dracula& getDracula() const { return dracula_; }
    Dracula& getDracula() { return dracula_; }
    
    const LevelSystem& getLevelSystem() const { return level_system_; }
    LevelSystem& getLevelSystem() { return level_system_; }
    
    const PlayerUpgradeSystem& getUpgradeSystem() const { return upgrade_system_; }
    PlayerUpgradeSystem& getUpgradeSystem() { return upgrade_system_; }
    
    const CommandRegistry& getCommandRegistry() const { return *command_registry_; }
    const KeyBindingsConfig& getKeyBindings() const { return *key_bindings_; }
    
    UIEventManager& getUIEventManager() { return ui_event_manager_; }
    const UIEventManager& getUIEventManager() const { return ui_event_manager_; }
    
    bool isGameRunning() const { return game_running_; }
    bool isPlayerTurn() const { return is_player_turn_; }
    bool isPlayerTurnCompleted() const { return player_turn_completed_; }
    int getTurnCount() const { return turn_count_; }
    

    GameContext getContext();
    ConstGameContext getContext() const;
 
    void setGameRunning(bool running) { game_running_ = running; }
    void setTurnCount(int count) { turn_count_ = count; }
    void setPlayerTurn(bool player_turn) { is_player_turn_ = player_turn; }
    void setPlayerTurnCompleted(bool completed) { player_turn_completed_ = completed; }

    void addObserver(GameObserver* observer);
    void removeObserver(GameObserver* observer);
    void notifyObservers();
    
    bool processLibraryChoice(int choice, const Position& library_pos);
    bool processSpellSelection(int spell_index, const Position& target);
    bool processUpgradeChoice(int upgrade_id);
    Library* getLibraryAtPosition(const Position& pos);
    bool isLibraryInteractable(const Position& pos) const;
    bool isSpellSelectionAvailable() const;
    bool isLevelUpAvailable() const;
    bool canShowStatus() const { return true; }
    bool canShowField() const { return true; }
    bool canShowSpells() const { return hunter_.getSpellHand().getSpellCount() > 0; }
    bool canShowHelp() const { return true; }
    bool canSelectSpell() const { return hunter_.getSpellHand().getSpellCount() > 0; }
    const std::vector<PlayerUpgradeSystem::UpgradeOption>& getUpgradeOptions() const;
};