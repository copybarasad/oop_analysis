#include "game.h"
#include "level_manager.h"
#include "save_manager.h"
#include "upgrade_system.h"
#include "console_renderer.h"
#include "game_visualizer.h"
#include "game_exceptions.h"
#include "game_event.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include <sstream>
#include <random>
#include <iostream>
#include <limits>

namespace rpg {
    Game::Game()
        : level_manager_(std::make_unique<LevelManager>()),
          save_manager_(std::make_unique<SaveManager>()),
          upgrade_system_(std::make_unique<UpgradeSystem>()),
          visualizer_(std::make_unique<GameVisualizer<ConsoleRenderer>>()),
          field_(GameField::kDefaultFieldSize, GameField::kDefaultFieldSize),
          player_(kPlayerStartHealth, kPlayerMeleeDamage, kPlayerRangedDamage, kPlayerStartPosition),
          turn_number_(0),
          is_running_(false) {
    }

    Game::~Game() = default;

    Game::Game(size_t field_width, size_t field_height)
        : field_(field_width, field_height),
          player_(kPlayerStartHealth, kPlayerMeleeDamage, kPlayerRangedDamage, kPlayerStartPosition),
          turn_number_(0),
          is_running_(true) {
        field_.ClearArea(kPlayerStartPosition.x, kPlayerStartPosition.y);
    }

    [[nodiscard]] bool Game::IsRunning() const {
        return is_running_;
    }

    [[nodiscard]] bool Game::IsPlayerAlive() const {
        return player_.IsAlive();
    }

    [[nodiscard]] const Player &Game::GetPlayer() const {
        return player_;
    }

    [[nodiscard]] int Game::GetTurnNumber() const {
        return turn_number_;
    }

    [[nodiscard]] std::string Game::ProcessCommand(GameCommand command, Direction dir) {
        if (!is_running_) {
            return "Игра завершена.";
        }

        if (!player_.IsAlive()) {
            is_running_ = false;
            return "Игрок погиб! Игра окончена.";
        }

        std::string result;

        switch (command) {
            case GameCommand::kMove: {
                Position old_pos = player_.GetPosition();
                result = turn_manager_.MovePlayer(player_, dir, field_, entity_manager_);
                Position new_pos = player_.GetPosition();
                
                if (old_pos.x != new_pos.x || old_pos.y != new_pos.y) {
                    NotifyEvent(GameEventType::kPlayerMoved,
                               "Игрок переместился",
                               {{"from_x", std::to_string(old_pos.x)},
                                {"from_y", std::to_string(old_pos.y)},
                                {"to_x", std::to_string(new_pos.x)},
                                {"to_y", std::to_string(new_pos.y)}});
                }
                break;
            }
            case GameCommand::kSwitchCombatMode:
                result = SwitchPlayerCombatMode();
                break;
            case GameCommand::kAttack:
                result = AttackNearby();
                break;
            case GameCommand::kQuit:
                Quit();
                result = "Игра завершена по команде игрока.";
                break;
            default:
                return "Неизвестная команда.";
        }

        std::string ally_messages = ProcessAllyTurns();
        std::string enemy_messages = ProcessEnemyTurns();
        ProcessBuildingTurns();
        ProcessTowerTurns();
        std::string trap_messages = CheckTraps();
        EndTurn();

        if (!ally_messages.empty()) {
            result += "\n" + ally_messages;
        }
        if (!enemy_messages.empty()) {
            result += "\n" + enemy_messages;
        }
        if (!trap_messages.empty()) {
            result += "\n" + trap_messages;
        }

        if (!player_.IsAlive()) {
            is_running_ = false;
            result += "\nИгрок погиб! Игра окончена.";
        }

        return result;
    }

    [[nodiscard]] std::string Game::SwitchPlayerCombatMode() {
        player_.SwitchCombatMode();

        const std::string mode = (player_.GetCombatMode() == CombatMode::kMelee)
                                     ? "ближний бой"
                                     : "дальний бой";

        NotifyEvent(GameEventType::kCombatModeChanged,
                   "Режим боя изменён на: " + mode,
                   {{"mode", mode}, {"damage", std::to_string(player_.GetDamage())}});

        return "Режим боя изменен на: " + mode + ". Урон: " +
               std::to_string(player_.GetDamage()) + ".";
    }

    [[nodiscard]] std::string Game::AttackNearby() {
        const Position player_pos = player_.GetPosition();
        const int damage = player_.GetDamage();

        std::vector<std::pair<int, int> > offsets = {
            {-1, -1}, {0, -1}, {1, -1}, // вверх-влево, вверх, вверх-вправо
            {-1, 0}, {1, 0}, // влево, вправо
            {-1, 1}, {0, 1}, {1, 1} // вниз-влево, вниз, вниз-вправо
        };

        int enemies_hit = 0;
        int enemies_killed = 0;

        for (const auto &offset: offsets) {
            int new_x = static_cast<int>(player_pos.x) + offset.first;
            int new_y = static_cast<int>(player_pos.y) + offset.second;

            if (new_x < 0 || new_y < 0 ||
                new_x >= static_cast<int>(field_.GetWidth()) ||
                new_y >= static_cast<int>(field_.GetHeight())) {
                continue;
            }

            Position check_pos(static_cast<size_t>(new_x), static_cast<size_t>(new_y));
            Enemy *enemy = FindEnemyAt(check_pos);
            if (enemy != nullptr) {
                enemy->TakeDamage(damage);
                enemies_hit++;

                NotifyEvent(GameEventType::kEnemyDamaged,
                           "Враг получил урон",
                           {{"damage", std::to_string(damage)},
                            {"x", std::to_string(check_pos.x)},
                            {"y", std::to_string(check_pos.y)}});

                if (!enemy->IsAlive()) {
                    enemies_killed++;
                    player_.AddScore(kScorePerKill);
                    
                    NotifyEvent(GameEventType::kEnemyKilled,
                               "Враг уничтожен",
                               {{"score", std::to_string(kScorePerKill)},
                                {"x", std::to_string(check_pos.x)},
                                {"y", std::to_string(check_pos.y)}});
                }
            }
        }

        std::ostringstream oss;
        if (enemies_hit == 0) {
            oss << "Рядом нет врагов для атаки.";
        } else {
            oss << "Игрок атаковал " << enemies_hit
                    << " врага(ов), нанеся по " << damage << " урона.";

            if (enemies_killed > 0) {
                oss << " Уничтожено врагов: " << enemies_killed
                        << "! Получено очков: " << (enemies_killed * kScorePerKill) << ".";
            }
        }

        return oss.str();
    }

    void Game::Quit() {
        is_running_ = false;
    }

    void Game::RestorePlayer(const Player& player) {
        player_ = player;
    }

    void Game::SetTurnNumber(int turn_number) {
        turn_number_ = turn_number;
    }

    Player& Game::GetMutablePlayer() {
        return player_;
    }


    void Game::AddEnemy(int health, int damage, const Position &pos) {
        if (field_.IsInBounds(pos.x, pos.y) && field_.IsPassable(pos.x, pos.y) &&
            !IsPositionOccupiedByPlayer(pos)) {
            entity_manager_.AddEnemy(health, damage, pos);
        }
    }

    void Game::AddEnemyBuilding(const Position &pos, int spawn_interval,
                                int enemy_health, int enemy_damage) {
        if (field_.IsInBounds(pos.x, pos.y) && field_.IsPassable(pos.x, pos.y) &&
            !IsPositionOccupiedByPlayer(pos)) {
            entity_manager_.AddBuilding(pos, spawn_interval, enemy_health, enemy_damage);
        }
    }

    [[nodiscard]] std::string Game::ProcessEnemyTurns() {
        std::string messages = turn_manager_.ProcessEnemyTurns(entity_manager_, player_, field_);
        RemoveDeadEnemies();
        return messages;
    }

    void Game::ProcessBuildingTurns() {
        turn_manager_.ProcessBuildingTurns(entity_manager_, *this);
    }

    void Game::EndTurn() {
        ++turn_number_;
        player_.UpdateAfterTurn();
        RemoveDeadAllies();
        RemoveDeadEnemies();
    }

    [[nodiscard]] GameField &Game::GetField() {
        return field_;
    }

    [[nodiscard]] const GameField &Game::GetField() const {
        return field_;
    }

    [[nodiscard]] std::vector<Enemy> &Game::GetEnemies() {
        return entity_manager_.GetEnemies();
    }

    [[nodiscard]] const std::vector<Enemy> &Game::GetEnemies() const {
        return entity_manager_.GetEnemies();
    }

    [[nodiscard]] std::vector<EnemyBuilding> &Game::GetBuildings() {
        return entity_manager_.GetBuildings();
    }

    [[nodiscard]] const std::vector<EnemyBuilding> &Game::GetBuildings() const {
        return entity_manager_.GetBuildings();
    }

    [[nodiscard]] std::vector<Trap> &Game::GetTraps() {
        return entity_manager_.GetTraps();
    }

    [[nodiscard]] const std::vector<Trap> &Game::GetTraps() const {
        return entity_manager_.GetTraps();
    }

    [[nodiscard]] std::vector<Ally> &Game::GetAllies() {
        return entity_manager_.GetAllies();
    }

    [[nodiscard]] const std::vector<Ally> &Game::GetAllies() const {
        return entity_manager_.GetAllies();
    }

    [[nodiscard]] std::vector<EnemyTower> &Game::GetTowers() {
        return entity_manager_.GetTowers();
    }

    [[nodiscard]] const std::vector<EnemyTower> &Game::GetTowers() const {
        return entity_manager_.GetTowers();
    }

    [[nodiscard]] Position Game::GetPlayerPosition() const {
        return player_.GetPosition();
    }

    void Game::DamagePlayer(int damage) {
        player_.TakeDamage(damage);
        
        NotifyEvent(GameEventType::kPlayerDamaged,
                   "Игрок получил урон",
                   {{"damage", std::to_string(damage)},
                    {"health", std::to_string(player_.GetHealth())},
                    {"x", std::to_string(player_.GetPosition().x)},
                    {"y", std::to_string(player_.GetPosition().y)}});
    }

    void Game::ActivateEnhancement() {
        player_.GetSpellHand().ActivateEnhancement();
    }

    void Game::AddTower(const Position &pos) {
        entity_manager_.AddTower(pos);
    }

    [[nodiscard]] std::string Game::CastSpell(size_t spell_index, const Position &target_pos) {
        Spell *spell = player_.GetSpellHand().GetSpellMutable(spell_index);

        if (!spell) {
            return "Некорректный индекс заклинания!";
        }

        if (!spell->CanCast(target_pos, *this)) {
            return "Невозможно применить " + spell->GetName() + " в этом месте!";
        }

        std::string spell_name = spell->GetName();
        SpellType spell_type = spell->GetType();

        if (spell_type == SpellType::kEnhancement) {
            if (player_.GetSpellHand().IsNextCastEnhanced()) {
                return "Усиление уже активно! Сначала используйте другое заклинание.";
            }
            spell->Cast(target_pos, *this);
            player_.GetSpellHand().RemoveSpell(spell_index);

            std::string ally_messages = ProcessAllyTurns();
            std::string enemy_messages = ProcessEnemyTurns();
            ProcessBuildingTurns();
            ProcessTowerTurns();
            std::string trap_messages = CheckTraps();
            EndTurn();

            std::string result = "Ваше следующее заклинание будет УСИЛЕНО! (Заклинание использовано)";
            if (!enemy_messages.empty()) {
                result += "\n" + enemy_messages;
            }
            return result;
        }

        std::vector<std::pair<int, int> > enemies_before;
        for (const auto &enemy: entity_manager_.GetEnemies()) {
            enemies_before.emplace_back(enemy.GetHealth(), enemy.GetHealth() + 0);
        }

        std::string result_message;
        bool is_enhanced = player_.GetSpellHand().IsNextCastEnhanced();
        if (is_enhanced) {
            spell->CastEnhanced(target_pos, *this);
            player_.GetSpellHand().DeactivateEnhancement();
            result_message = "Применено УСИЛЕННОЕ " + spell_name + "!";
        } else {
            spell->Cast(target_pos, *this);
            result_message = "Применено " + spell_name + "!";
        }
        
        NotifyEvent(GameEventType::kSpellCast,
                   "Использовано заклинание: " + spell_name,
                   {{"spell_name", spell_name},
                    {"spell_index", std::to_string(spell_index)},
                    {"enhanced", is_enhanced ? "true" : "false"},
                    {"target_x", std::to_string(target_pos.x)},
                    {"target_y", std::to_string(target_pos.y)}});

        std::ostringstream spell_effects;
        size_t idx = 0;
        int enemies_killed = 0;
        int total_damage = 0;

        for (const auto &enemy: entity_manager_.GetEnemies()) {
            if (idx < enemies_before.size()) {
                int damage_dealt = enemies_before[idx].first - enemy.GetHealth();
                if (damage_dealt > 0) {
                    total_damage += damage_dealt;
                    if (!enemy.IsAlive()) {
                        enemies_killed++;
                    }
                }
            }
            idx++;
        }

        if (total_damage > 0) {
            spell_effects << "\nНанесено " << total_damage << " урона!";
            if (enemies_killed > 0) {
                spell_effects << " Убито врагов: " << enemies_killed;
                player_.AddScore(enemies_killed * kScorePerKill);
            }
        }

        result_message += spell_effects.str() + " (Заклинание использовано)";

        player_.GetSpellHand().RemoveSpell(spell_index);

        std::string ally_messages = ProcessAllyTurns();
        std::string enemy_messages = ProcessEnemyTurns();
        ProcessBuildingTurns();
        ProcessTowerTurns();
        std::string trap_messages = CheckTraps();
        EndTurn();

        if (!ally_messages.empty()) {
            result_message += "\n" + ally_messages;
        }
        if (!enemy_messages.empty()) {
            result_message += "\n" + enemy_messages;
        }
        if (!trap_messages.empty()) {
            result_message += "\n" + trap_messages;
        }

        return result_message;
    }

    [[nodiscard]] std::string Game::BuyRandomSpell() {
        auto result = player_.GetSpellHand().BuyRandomSpell(player_);

        if (!result.has_value()) {
            if (player_.GetSpellHand().IsFull()) {
                return "Рука заклинаний заполнена!";
            }
            return "Недостаточно очков для покупки заклинания!";
        }

        const Spell *spell = player_.GetSpellHand().GetSpell(result.value());
        
        NotifyEvent(GameEventType::kSpellBought,
                   "Куплено заклинание: " + spell->GetName(),
                   {{"spell_name", spell->GetName()},
                    {"spell_index", std::to_string(result.value())}});
        
        return "Куплено " + spell->GetName() + "!";
    }
    
    [[nodiscard]] std::string Game::SaveGame() {
        if (!save_manager_) {
            return "Ошибка: менеджер сохранений не инициализирован!";
        }
        
        try {
            save_manager_->SaveGame(*this);
            
            NotifyEvent(GameEventType::kGameSaved,
                       "Игра сохранена",
                       {{"turn", std::to_string(turn_number_)},
                        {"level", std::to_string(GetCurrentLevel())},
                        {"score", std::to_string(player_.GetScore())}});
            
            return "Игра успешно сохранена!";
        } catch (const FileSaveException& e) {
            return std::string("Ошибка сохранения: ") + e.what();
        } catch (const GameException& e) {
            return std::string("Ошибка: ") + e.what();
        } catch (const std::exception& e) {
            return std::string("Неожиданная ошибка при сохранении: ") + e.what();
        }
    }

    void Game::ProcessTowerTurns() {
        turn_manager_.ProcessTowerTurns(entity_manager_, *this);
    }

    [[nodiscard]] std::string Game::ProcessAllyTurns() {
        return turn_manager_.ProcessAllyTurns(entity_manager_, field_);
    }

    [[nodiscard]] std::string Game::CheckTraps() {
        return turn_manager_.CheckTraps(entity_manager_);
    }

    void Game::RemoveDeadEnemies() {
        entity_manager_.RemoveDeadEnemies();
    }

    void Game::RemoveDeadAllies() {
        entity_manager_.RemoveDeadAllies();
    }

    [[nodiscard]] bool Game::IsPositionOccupiedByEnemy(const Position &pos) const {
        return entity_manager_.IsPositionOccupiedByEnemy(pos);
    }

    [[nodiscard]] bool Game::IsPositionOccupiedByBuilding(const Position &pos) const {
        return entity_manager_.IsPositionOccupiedByBuilding(pos);
    }

    [[nodiscard]] bool Game::IsPositionOccupiedByPlayer(const Position &pos) const {
        return player_.GetPosition() == pos;
    }

    [[nodiscard]] Enemy *Game::FindEnemyAt(const Position &pos) {
        return entity_manager_.FindEnemyAt(pos);
    }
    
    void Game::StartNewGame() {
        level_manager_ = std::make_unique<LevelManager>();
        save_manager_ = std::make_unique<SaveManager>();
        upgrade_system_ = std::make_unique<UpgradeSystem>();
        visualizer_ = std::make_unique<GameVisualizer<ConsoleRenderer>>();
        
        LevelConfig config = level_manager_->GetCurrentLevelConfig();
        InitializeLevel(config);
        
        visualizer_->ShowHeader();
        visualizer_->GetRenderer().RenderMessage("Новая игра начата!");
        visualizer_->GetRenderer().RenderMessage("Цель: Уничтожайте врагов, набирайте очки и выживайте!");
        visualizer_->GetRenderer().RenderLevelStart(level_manager_->GetCurrentLevel());
        
        is_running_ = true;
    }
    
    void Game::LoadGameFromFile(const std::string& filename) {
        if (!SaveManager::SaveFileExists(filename)) {
            if (visualizer_) {
                visualizer_->GetRenderer().RenderMessage("Файл сохранения не найден!");
            }
            return;
        }
        
        try {
            level_manager_ = std::make_unique<LevelManager>();
            save_manager_ = std::make_unique<SaveManager>();
            upgrade_system_ = std::make_unique<UpgradeSystem>();
            visualizer_ = std::make_unique<GameVisualizer<ConsoleRenderer>>();
            
            // SaveManager загружает состояние напрямую в этот Game
            save_manager_->LoadGame(*this, filename);
            
            visualizer_->ShowHeader();
            visualizer_->GetRenderer().RenderMessage("Игра загружена!");
            
            is_running_ = true;
            
        } catch (const FileNotFoundException& e) {
            visualizer_->GetRenderer().RenderMessage(std::string("Ошибка: ") + e.what());
        } catch (const FileCorruptedException& e) {
            visualizer_->GetRenderer().RenderMessage(std::string("Ошибка: ") + e.what());
            visualizer_->GetRenderer().RenderMessage("Файл сохранения поврежден.");
        } catch (const GameException& e) {
            visualizer_->GetRenderer().RenderMessage(std::string("Ошибка: ") + e.what());
        }
    }
     

    


    

    


    
    void Game::InitializeLevel(const LevelConfig& config) {
        // Пересоздать поле с нужным размером
        field_ = GameField(config.field_width, config.field_height);
        field_.ClearArea(kPlayerStartPosition.x, kPlayerStartPosition.y);
        
        // Создать или восстановить игрока
        if (level_manager_->GetCurrentLevel() == 1) {
            player_ = Player(kPlayerStartHealth, kPlayerMeleeDamage, 
                            kPlayerRangedDamage, kPlayerStartPosition);
        } else {
            // Игрок сохраняет свое состояние между уровнями
            player_.SetPosition(kPlayerStartPosition);
        }
        
        // Очистить сущности
        entity_manager_ = EntityManager();
        
        // Заполнить уровень
        PopulateLevelWithBuildings(config);
        PopulateLevelWithTowers(config);
        PopulateLevelWithEnemies(config);
        
        turn_number_ = 0;
        is_running_ = true;
    }
    
    void Game::PopulateLevelWithEnemies(const LevelConfig& config) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> health_dist(
            config.enemy_health_base,
            config.enemy_health_base + 10
        );
        std::uniform_int_distribution<> damage_dist(
            config.enemy_damage_base,
            config.enemy_damage_base + 5
        );
        
        for (int i = 0; i < config.enemy_count; ++i) {
            std::uniform_int_distribution<size_t> x_dist(0, field_.GetWidth() - 1);
            std::uniform_int_distribution<size_t> y_dist(0, field_.GetHeight() - 1);
            
            for (int attempt = 0; attempt < 20; ++attempt) {
                Position pos(x_dist(gen), y_dist(gen));
                
                if (field_.IsPassable(pos.x, pos.y) && pos != kPlayerStartPosition) {
                    int distance_to_player = std::abs(static_cast<int>(pos.x)) + 
                                            std::abs(static_cast<int>(pos.y));
                    
                    if (distance_to_player > 5) {
                        AddEnemy(health_dist(gen), damage_dist(gen), pos);
                        break;
                    }
                }
            }
        }
    }
    
    void Game::PopulateLevelWithBuildings(const LevelConfig& config) {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        for (int i = 0; i < config.building_count; ++i) {
            std::uniform_int_distribution<size_t> x_dist(
                field_.GetWidth() / 2, field_.GetWidth() - 1
            );
            std::uniform_int_distribution<size_t> y_dist(
                field_.GetHeight() / 2, field_.GetHeight() - 1
            );
            
            for (int attempt = 0; attempt < 10; ++attempt) {
                Position pos(x_dist(gen), y_dist(gen));
                
                if (field_.IsPassable(pos.x, pos.y) && pos != kPlayerStartPosition) {
                    int spawn_interval = 5 - config.level_number / 3;
                    spawn_interval = std::max(2, spawn_interval);
                    
                    AddEnemyBuilding(
                        pos,
                        spawn_interval,
                        config.enemy_health_base,
                        config.enemy_damage_base
                    );
                    break;
                }
            }
        }
    }
    
    void Game::PopulateLevelWithTowers(const LevelConfig& config) {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        for (int i = 0; i < config.tower_count; ++i) {
            std::uniform_int_distribution<size_t> x_dist(3, field_.GetWidth() - 4);
            std::uniform_int_distribution<size_t> y_dist(3, field_.GetHeight() - 4);
            
            for (int attempt = 0; attempt < 10; ++attempt) {
                Position pos(x_dist(gen), y_dist(gen));
                
                if (field_.IsPassable(pos.x, pos.y) && pos != kPlayerStartPosition) {
                    AddTower(pos);
                    break;
                }
            }
        }
    }
    
    void Game::CheckLevelCompletion() {
        visualizer_->GetRenderer().RenderLevelComplete(player_.GetScore());
    }
    
    void Game::AdvanceToNextLevel() {
        if (!level_manager_->CanAdvanceToNextLevel()) {
            return;
        }
        
        visualizer_->GetRenderer().RenderMessage("Переход на следующий уровень...");
        
        ShowUpgradeMenu();
        
        level_manager_->AdvanceToNextLevel();
        upgrade_system_->ApplyLevelTransition(player_);
        
        LevelConfig config = level_manager_->GetCurrentLevelConfig();
        InitializeLevel(config);
        
        visualizer_->GetRenderer().RenderLevelStart(level_manager_->GetCurrentLevel());
    }
    
    void Game::ShowUpgradeMenu() {
        auto upgrades = upgrade_system_->GetAvailableUpgrades(level_manager_->GetCurrentLevel());
        
        int choice = 0;
        while (choice < 1 || choice > static_cast<int>(upgrades.size())) {
            visualizer_->GetRenderer().RenderUpgradeMenu(upgrades);
            
            std::cin >> choice;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                visualizer_->GetRenderer().RenderMessage("Ошибка ввода! Введите число от 1 до " + std::to_string(upgrades.size()));
                choice = 0;
                continue;
            }
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (choice < 1 || choice > static_cast<int>(upgrades.size())) {
                visualizer_->GetRenderer().RenderMessage("Неверный выбор! Введите число от 1 до " + std::to_string(upgrades.size()));
            }
        }
        
        upgrade_system_->ApplyUpgrade(player_, upgrades[choice - 1]);
        visualizer_->GetRenderer().RenderMessage("Улучшение применено: " + upgrades[choice - 1].name);
    }
    
    void Game::HandleGameOver() {
        visualizer_->GetRenderer().RenderGameOver(
            player_.GetScore(),
            turn_number_,
            level_manager_->GetCurrentLevel()
        );
    }
    
    [[nodiscard]] int Game::GetCurrentLevel() const {
        if (level_manager_) {
            return level_manager_->GetCurrentLevel();
        }
        return 1;
    }
    
    void Game::SetCurrentLevel(int level) {
        if (level_manager_) {
            level_manager_->SetCurrentLevel(level);
        }
    }
    
    // ===== Memento Pattern =====
    
    [[nodiscard]] GameMemento Game::CreateMemento() const {
        GameMemento memento;
        
        // Метаданные
        memento.turn_number = turn_number_;
        memento.current_level = GetCurrentLevel();
        
        // Размеры поля
        memento.field_width = field_.GetWidth();
        memento.field_height = field_.GetHeight();
        
        // Состояние игрока
        memento.player_health = player_.GetHealth();
        memento.player_max_health = player_.GetMaxHealth();
        memento.player_melee_damage = player_.GetMeleeDamage();
        memento.player_ranged_damage = player_.GetRangedDamage();
        memento.player_score = player_.GetScore();
        memento.player_position = player_.GetPosition();
        memento.player_combat_mode = player_.GetCombatMode();
        memento.player_slowed_turns = player_.CanMove() ? 0 : 1;
        
        // Заклинания
        const SpellHand& spell_hand = player_.GetSpellHand();
        for (size_t i = 0; i < spell_hand.GetSize(); ++i) {
            const Spell* spell = spell_hand.GetSpell(i);
            if (spell) {
                memento.spell_types.push_back(static_cast<int>(spell->GetType()));
            }
        }
        
        // Клетки поля
        for (size_t y = 0; y < memento.field_height; ++y) {
            for (size_t x = 0; x < memento.field_width; ++x) {
                const Cell& cell = field_.GetCell(x, y);
                if (cell.GetType() == CellType::kImpassable) {
                    memento.field_impassable_cells.emplace_back(x, y);
                } else if (cell.GetType() == CellType::kSlowing) {
                    memento.field_slowing_cells.emplace_back(x, y);
                }
            }
        }
        
        // Сущности
        memento.enemies = entity_manager_.GetEnemies();
        memento.buildings = entity_manager_.GetBuildings();
        memento.traps = entity_manager_.GetTraps();
        memento.allies = entity_manager_.GetAllies();
        memento.towers = entity_manager_.GetTowers();
        
        return memento;
    }
    
    void Game::RestoreFromMemento(const GameMemento& memento) {
        // Восстановить метаданные
        turn_number_ = memento.turn_number;
        SetCurrentLevel(memento.current_level);
        
        // Восстановить поле
        field_ = GameField(memento.field_width, memento.field_height);
        
        for (size_t y = 0; y < memento.field_height; ++y) {
            for (size_t x = 0; x < memento.field_width; ++x) {
                field_.SetCellType(x, y, CellType::kNormal);
            }
        }
        
        for (const auto& [x, y] : memento.field_impassable_cells) {
            if (field_.IsInBounds(x, y)) {
                field_.SetCellType(x, y, CellType::kImpassable);
            }
        }
        
        for (const auto& [x, y] : memento.field_slowing_cells) {
            if (field_.IsInBounds(x, y)) {
                field_.SetCellType(x, y, CellType::kSlowing);
            }
        }
        
        // Восстановить игрока
        Player restored_player(
            memento.player_health,
            memento.player_max_health,
            memento.player_melee_damage,
            memento.player_ranged_damage,
            memento.player_score,
            memento.player_position,
            memento.player_combat_mode,
            memento.player_slowed_turns
        );
        
        player_ = restored_player;
        
        // Восстановить заклинания
        SpellHand& spell_hand = player_.GetSpellHand();
        for (int spell_type_int : memento.spell_types) {
            SpellType spell_type = static_cast<SpellType>(spell_type_int);
            std::unique_ptr<Spell> spell;
            
            switch (spell_type) {
                case SpellType::kDirectDamage:
                    spell = std::make_unique<DirectDamageSpell>();
                    break;
                case SpellType::kAreaDamage:
                    spell = std::make_unique<AreaDamageSpell>();
                    break;
                case SpellType::kTrap:
                    spell = std::make_unique<TrapSpell>();
                    break;
                case SpellType::kSummon:
                    spell = std::make_unique<SummonSpell>();
                    break;
                case SpellType::kEnhancement:
                    spell = std::make_unique<EnhancementSpell>();
                    break;
            }
            
            if (spell) {
                spell_hand.AddSpell(std::move(spell));
            }
        }
        
        // Восстановить сущности
        entity_manager_ = EntityManager();
        entity_manager_.GetEnemies() = memento.enemies;
        entity_manager_.GetBuildings() = memento.buildings;
        entity_manager_.GetTraps() = memento.traps;
        entity_manager_.GetAllies() = memento.allies;
        entity_manager_.GetTowers() = memento.towers;
        
        is_running_ = true;
    }
    
    void Game::NotifyEvent(GameEventType type, const std::string& description,
                          const std::map<std::string, std::string>& data) {
        event_manager_.Notify(GameEvent(type, description, data));
    }
    
    bool Game::ShouldContinueLevel() const {
        return is_running_ && player_.IsAlive() && 
               !level_manager_->HasWon(player_.GetScore());
    }
    
    void Game::CheckAndHandleLevelCompletion() {
        if (level_manager_->HasWon(player_.GetScore())) {
            CheckLevelCompletion();
            
            if (level_manager_->IsGameComplete()) {
                visualizer_->GetRenderer().RenderGameComplete(player_.GetScore());
                is_running_ = false;

                NotifyEvent(GameEventType::kLevelCompleted,
                           "Игра завершена! Все уровни пройдены!",
                           {{"final_score", std::to_string(player_.GetScore())}});
            } else {
                AdvanceToNextLevel();
            }
        }
    }
    
    void Game::CheckAndHandleGameOver() {
        if (!player_.IsAlive()) {
            HandleGameOver();
        }
    }
}
