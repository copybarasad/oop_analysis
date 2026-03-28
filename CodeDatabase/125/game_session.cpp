#include "game_session.h"
#include <cstdlib>
#include <iostream>
#include <vector>

GameSession::GameSession(int width, int height)
    : field_(width, height), player_("Герой", {0, 0}), enemy_manager_(), current_level_(1) {}

bool GameSession::LoadGame(int& level) {
    bool result = SaveManager::LoadGame(level, field_, player_, enemy_manager_);
    if (result) {
        current_level_ = level;
    }
    return result;
}

void GameSession::SaveGame(int level) {
    SaveManager::SaveGame(level, field_, player_, enemy_manager_);
}

void GameSession::StartLevel(int start_type) {
    int width = 16 + current_level_  * 3;
    int height = 16 + current_level_  * 3;
    field_ = GameField(width, height);

    player_.ResetHealth();
    player_.RemoveRandomHalfSpells();

    enemy_manager_ = EnemyManager();
    
    enemy_manager_.InitializeEnemies(field_, current_level_, 30 + current_level_ * 5, 12 + current_level_ * 3);

    Position tower_pos = {width / 2, height / 2};
    enemy_manager_.AddTower(tower_pos, field_);

    int enemy_count = current_level_;

    if (start_type == 2) { 
        std::cout << "Игра перезапущена!\n";
    }

    std::cout << "Размер поля: " << field_.GetWidth() << "x" << field_.GetHeight() << "\n";
}

bool GameSession::ProcessPlayerTurn(char cmd) {
    if (cmd == '?') {
        return false;
    }
    
    if (cmd == 'f') {
        player_.Attack(enemy_manager_, field_);
        return true;
    }

    if (cmd == 'm' || cmd == 'w' || cmd == 'a' || cmd == 's' || cmd == 'd') {
        return player_.ProcessAction(cmd, field_, enemy_manager_);
    }

    if (cmd >= '1' && cmd <= '3') {
        size_t index = cmd - '1';
        auto& hand = player_.GetHand();
        if (index < hand.GetSize()) {
            hand.CastSpell(index, player_, enemy_manager_, field_);
            return true;
        }
        std::cout << "Нет заклинания под номером " << (index + 1) << "!\n";
        return false;
    }

    if (cmd == 'e') {
        UIManager::ShowSpellShop(player_);
        return true;
    }

    std::cout << "Неизвестная команда!\n";
    return false;
}

void GameSession::EnemiesTurn() {
    bool action_happened = false;

    std::vector<Position> old_positions;
    for (const auto& enemy : enemy_manager_.GetEnemies()) {
        if (enemy.IsAlive()) {
            old_positions.push_back(enemy.GetPosition());
        }
    }

    enemy_manager_.MoveEnemies(player_, field_);

    size_t idx = 0;
    for (const auto& enemy : enemy_manager_.GetEnemies()) {
        if (enemy.IsAlive() && idx < old_positions.size()) {
            if (enemy.GetPosition() != old_positions[idx]) {
                action_happened = true;
            }
            ++idx;
        }
    }

    int hp_before = player_.GetHealth();
    enemy_manager_.UpdateTowers(player_, field_);
    if (player_.GetHealth() < hp_before) {
        action_happened = true;
    }
}

bool GameSession::IsLevelComplete() const {
    return !enemy_manager_.AnyAlive();
}

bool GameSession::IsGameOver() const {
    return !player_.IsAlive();
}

void GameSession::RestartGame() {
    int width = field_.GetWidth();
    int height = field_.GetHeight();
    
    field_ = GameField(width, height);
    player_ = Player("Герой", {0, 0});
    enemy_manager_ = EnemyManager();
    current_level_ = 1;
    
    enemy_manager_.InitializeEnemies(field_, 3);
    player_.ResetHealth();
}

void GameSession::HandleVictory() {
    std::cout << "\n";
    std::cout << "=======================================================================\n";
    std::cout << "                             ПОЗДРАВЛЯЕМ!           \n";
    std::cout << "                    ВЫ ПРОШЛИ ВСЕ " << MAX_LEVELS << " УРОВНЯ!   \n";
    std::cout << "=======================================================================\n";
    std::cout << "\n";
    std::cout << "Спасибо за игру! До новых встреч!\n";

    std::remove("savegame.json");
}

void GameSession::ProcessMovement(Position new_pos) {
    if (field_.IsWalkable(new_pos)) {
        if (player_.CheckEnemyCollision(enemy_manager_, new_pos)) {
            return;
        } else {
            player_.SetPosition(new_pos);
            return;
        }
    }
    std::cout << "Невозможно переместиться в эту клетку!\n";
}

void GameSession::ProcessAttack() {
    player_.Attack(enemy_manager_, field_);
}

void GameSession::ProcessSpellCast(int spellIndex) {
    auto& hand = player_.GetHand();
    if (spellIndex >= 0 && spellIndex < static_cast<int>(hand.GetSize())) {
        hand.CastSpell(spellIndex, player_, enemy_manager_, field_);
    } else {
        std::cout << "Нет заклинания под номером " << (spellIndex + 1) << "!\n";
    }
}

void GameSession::ProcessSwitchMode() {
    player_.SwitchCombatMode();
}

void GameSession::ProcessOpenShop() {
    UIManager::ShowSpellShop(player_);
}

void GameSession::ProcessSaveGame() {
    SaveGame(current_level_);
}
