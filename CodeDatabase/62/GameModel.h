#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "GameField.h"
#include "Deck.h"
#include "Shop.h"
#include "Wallet.h"
#include "ButtonModel.h"
#include "GameSaveException.h"
#include "Event.h"
#include "GameEventNotifier.h"

#define DECK_SIZE 5

struct ScheduledAction {
    float triggerTime;
    std::function<void()> action;
};

enum class GameState {
    Running,
    GameOver,
    Menu,
    Next
};

class GameModel {
private:
    bool is_running_;

    int player_boost_level_ = 1;

    GameState game_state_ = GameState::Menu;

    bool is_player_turn_ = true;

    bool done_move_ = false;

    GameField field;
    Deck deck;
    Shop shop;
    Wallet wallet;
    
    std::vector<ScheduledAction*> scheduledActions_;
    std::vector<int> v = {};
    std::pair<int, int> selected_cell_ = {-1, -1};

    Spell* mouse_on_spell_ = nullptr;
    Spell* selected_spell_ = nullptr;
    const ShopItem* selected_shop_item_ = nullptr;

    std::vector<ButtonModel> menu_btns_, next_btns_;

    int selected_button_id = 0;

    int goal_;
    int score_ = 0;

    std::string save_game_error_message = "";
    
    GameEventNotifier* eventNotifier_ = nullptr;  // Указатель на нотификатор событий

    void processScheduledActions(float dt);

    void makePlayerAttack(int dx, int dy);

    void makeBotsMove(bool allies = false);

    void resetBotsTurns();

    void applyDamage(int damage, int x, int y, float delay = 0.5f);

    void playerBoughtItem(const ShopItem* item);

    void addScore(int v);

    void finishLevel();

    void nextLevel();

public:
    GameModel(int field_width, int field_height, GameEventNotifier* notifier = nullptr);
    
    // Устанавливает нотификатор событий
    void setEventNotifier(GameEventNotifier* notifier) { eventNotifier_ = notifier; }

    std::vector<const Enemy*> getEnemies() const;
    std::vector<const SpawnerBuilding*> getSpawnerBuildings() const;

    std::vector<const Entity*> getEntities() const;

    const Player* getPlayer() const;

    bool getIsRunning() { return is_running_; }

    void movePlayer(float x, float y);

    void changePlayerAttackMode();

    void handleMouseClick();

    void update(float dt);

    const GameField& getField() const { return field; }

    void scheduleAction(float triggerTime, std::function<void()> action);

    void selectCell(int x, int y);

    const std::pair<int, int> getSelectedCell() const { return selected_cell_; }

    const bool getCanPlayerMove() const { return !done_move_ && is_player_turn_; }

    GameState getGameState() const { return game_state_; }

    const Deck& getDeck() const { return deck; }

    Deck& getDeck() { return deck; }

    void useSpell(Spell* spell);

    Shop& getShop() { return shop; }
    const Shop& getShop() const { return shop; }

    void setMouseOnSpell(Spell* spell) { mouse_on_spell_ = spell; }

    Spell* getMouseOnSpell() const { return mouse_on_spell_; }

    void selectSpell(Spell* spell);

    void selectShopItem(const ShopItem* item);

    const Spell* getSelectedSpell() const { return selected_spell_; }
    const ShopItem* getSelectedShopItem() const { return selected_shop_item_; }

    const Wallet& getWallet() const { return wallet; }

    int getBoostLevel() const { return player_boost_level_; }

    void setSelectedBtnId(int new_id) { selected_button_id = new_id; }

    const std::vector<ButtonModel>& getMenuButtons() const { return menu_btns_; }
    const std::vector<ButtonModel>& getNextLevelButtons() const { return next_btns_; }

    const int getGoal() const { return goal_; }
    const int getScore() const { return score_; }

    void saveGame();
    void loadGame();
    
    void processEvents(const std::vector<Event>& events);
    
    // Новые методы для управления с клавиатуры
    void useSpellFromHand(int index);
    void buyShopItem(int index);
    void attackDirection(float x, float y);
    void selectButton(int index);
    
    std::string getSaveGameErrorMessage() const { return save_game_error_message; }

    const int getGameLevel() const { return field.getLevel(); }
}; 