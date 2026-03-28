#include <iostream>
#include <limits>
#include <climits>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include "Game.h"
#include "GameException.h"
#include "EnemyTower.h"
#include "Command.h"
#include "EventDispatcher.h"
#include "GameEvent.h"

constexpr int kSpellDropChancePercent = 60;
constexpr int kPointsPerKill = 3;
constexpr int kBuildingDestroyPoints = 5;
constexpr int kHitPointLoss = -1;
constexpr int kFieldSizeIncrement = 4;
constexpr int kBuildingsPerLevel = 1;
constexpr int kDefaultEnemyHealth = 4;
constexpr int kDefaultEnemyDamage = 2;
constexpr int kEnemyHealthPerLevel = 2;
constexpr int kEnemyDamagePerLevel = 2;
constexpr int kPlayerHealthPerLevel = 20;
constexpr int kPlayerDamagePerLevel = 2;
constexpr int kMaxLevel = 5;
constexpr int kStartSizeField = 10;
constexpr int kStartCountBuildings = 1;
constexpr int kMaxSaves = 5;
constexpr std::string_view KEYBINDINGS_FILE = "keybindings.txt";

Game::Game(int width, int height, int player_health, int player_damage,
           int num_buildings, int player_x, int player_y)
    : current_enemy_health_(kDefaultEnemyHealth),
      current_enemy_damage_(kDefaultEnemyDamage),
      field_(width, height, num_buildings, current_enemy_health_, current_enemy_damage_),
      player_(player_health, player_damage, player_x, player_y),
      spell_hand_(),
      current_level_(1),
      game_started_(false),
      controller_(),
      visualizer_() {
    if (!field_.IsInside(player_x, player_y)) {
        throw std::invalid_argument("Начальная позиция игрока вне поля");
    }
    if (!field_.IsCellEmpty(player_x, player_y)) {
        throw std::invalid_argument("Начальная клетка занята");
    }
    for (auto& building : field_.GetBuildings()) {
        building.Tick(field_, enemies_);
    }
    
    LoadKeyBindings();
}

void Game::LoadKeyBindings() {
    KeyBindings bindings;
    bindings.LoadFromFile(std::string(KEYBINDINGS_FILE));
    
    controller_.GetInputHandler().SetKeyBindings(bindings);
    visualizer_.GetRenderer().SetKeyBindings(bindings);
}

bool Game::HasRemainingEnemies() {
    if (!enemies_.empty()) return true;
    for (const auto& building : field_.GetBuildings()) {
        if (!building.IsEmpty()) return true;
    }
    return false;
}

void Game::Start() {
    bool running = true;

    while (running) {
        visualizer_.RenderMainMenu();

        auto cmd = controller_.GetMenuChoice();
        int choice = cmd->GetChoice();

        if (choice < 0) {
            visualizer_.ShowError("Некорректный ввод. Попробуйте снова.");
            continue;
        }

        switch (choice) {
            case 1:
                if (game_started_) {
                    RestartGame();
                } else {
                    int count_before = spell_hand_.GetSpellCount();
                    spell_hand_.AddRandomSpell();
                    if (spell_hand_.GetSpellCount() > count_before) {
                        DISPATCH_EVENT(Events::SpellReceived(spell_hand_.GetSpells().back()->GetName()));
                    }
                    game_started_ = true;
                }
                RunLevel();
                break;

            case 2:
                if (LoadGameMenu()) {
                    visualizer_.ShowMessage("Загрузка сохранения...");
                    game_started_ = true;
                    RunLevel();
                }
                break;

            case 3:
                visualizer_.ShowMessage("Выход из игры...");
                running = false;
                break;

            default:
                visualizer_.ShowError("Некорректный ввод. Попробуйте снова.");
                break;
        }
    }
}

void Game::RunLevel() {
    field_.SetCellType(player_.GetX(), player_.GetY(), CellType::PLAYER);
    bool level_active = true;

    while (level_active) {
        RunResult res = Run();

        if (res == RunResult::QUIT) {
            visualizer_.ShowMessage("Возврат в главное меню...");
            return;
        }
        else if (res == RunResult::LOSE) {
            visualizer_.ShowPrompt("Попробовать снова? (y/n): ");
            bool asking = true;
            while (asking) {
                auto answer = controller_.GetYesNo();
                if (answer->GetType() == CommandType::YES) {
                    RestartGame();
                    asking = false;
                } else if (answer->GetType() == CommandType::NO) {
                    visualizer_.ShowMessage("Выход из игры...");
                    level_active = false;
                    asking = false;
                } else {
                    visualizer_.ShowError("Введите y или n.");
                    visualizer_.ShowPrompt("Попробовать снова? (y/n): ");
                }
            }
        }
        else {
            NextLevel();
            level_active = false;
        }
    }
}

RunResult Game::Run() {
    while (HasRemainingEnemies() && player_.GetHealth() > 0) {
        visualizer_.RenderGameState(field_, player_, spell_hand_.GetSpellCount());

        auto cmd = controller_.GetGameCommand();
        if (!cmd->IsValid()) {
            visualizer_.ShowError("Некорректный ввод команды!");
            continue;
        }

        int action = HandlePlayerAction(cmd.get());

        if (action == -1) {
            visualizer_.ShowMessage("Все данные будут потеряны!");
            visualizer_.ShowPrompt("Уверены, что хотите выйти? (y/n): ");

            bool asking = true;
            while (asking) {
                auto answer = controller_.GetYesNo();
                if (answer->GetType() == CommandType::YES) {
                    visualizer_.ShowMessage("Выход из игры...");
                    return RunResult::QUIT;
                } else if (answer->GetType() == CommandType::NO) {
                    asking = false;
                } else {
                    visualizer_.ShowError("Введите y или n.");
                    visualizer_.ShowPrompt("Уверены, что хотите выйти? (y/n): ");
                }
            }
            continue;
        }

        if (action == 0) continue;

        UpdateEnemies();
        for (auto& building : field_.GetBuildings()) building.Tick(field_, enemies_);
        
        for (auto& tower : field_.GetTowers()) {
            int hp_before = player_.GetHealth();
            tower.Tick(player_);
            int damage_dealt = hp_before - player_.GetHealth();
            if (damage_dealt > 0) {
                DISPATCH_EVENT(Events::PlayerDamaged(damage_dealt, player_.GetHealth()));
            }
        }
    }

    if (player_.GetHealth() <= 0) {
        visualizer_.ShowDefeat();
        return RunResult::LOSE;
    }

    visualizer_.ShowVictory();
    return RunResult::WIN;
}

int Game::GetSpellIndexFromPlayer() {
    std::optional<int> index;
    do {
        visualizer_.ShowPrompt("Введите номер заклинания: ");
        index = controller_.GetSpellIndex(spell_hand_.GetSpellCount());

        if (!index.has_value()) {
            visualizer_.ShowError("Введите корректный номер заклинания!");
        }
    } while (!index.has_value());
    
    return index.value();
}

int Game::GetSaveSlot(int save_count) {
    std::optional<int> slot;
    do {
        visualizer_.ShowPrompt("Введите номер сохранения: ");
        slot = controller_.GetSaveSlot(save_count);

        if (!slot.has_value()) {
            visualizer_.ShowError("Введите корректный номер сохранения!");
        }
    } while (!slot.has_value());
    
    return slot.value();
}

std::string Game::GetFileNameFromUser() {
    std::unique_ptr<TextInputCommand> cmd;
    bool valid_input = false;
    
    do {
        visualizer_.ShowPrompt("Введите имя файла сохранения (только английские буквы, цифры, _, -, пробел): ");
        cmd = controller_.GetFileName();
        valid_input = cmd->IsValid() && !cmd->GetText().empty();

        if (!valid_input) {
            visualizer_.ShowError("Некорректное имя файла!");
        }
    } while (!valid_input);
    
    return cmd->GetText();
}

bool Game::LoadGameMenu() {
    namespace fs = std::filesystem;

    std::vector<std::string> saves;
    std::vector<std::string> save_names;

    for (const auto& entry : fs::directory_iterator("saves")) {
        if (entry.path().extension() == ".json") {
            save_names.push_back(entry.path().filename().string());
            saves.push_back(entry.path().string());
        }
    }

    if (saves.empty()) {
        visualizer_.ShowMessage("Сохранений нет.");
        return false;
    }

    visualizer_.RenderSavesList(save_names);
    LoadGame(saves[GetSaveSlot(static_cast<int>(saves.size()))]);
    return true;
}

int Game::HandlePlayerAction(Command* cmd) {
    if (player_.IsSlowed()) {
        visualizer_.ShowMessage("Вы пропускаете ход!");
        player_.SetSlowed(false);
        return 1;
    }

    int dx = 0;
    int dy = 0;

    switch (cmd->GetType()) {
        case CommandType::MOVE_UP:
        case CommandType::MOVE_DOWN:
        case CommandType::MOVE_LEFT:
        case CommandType::MOVE_RIGHT: {
            auto* move = dynamic_cast<MoveCommand*>(cmd);
            if (move) {
                dx = move->GetDX();
                dy = move->GetDY();
            }
            break;
        }
        case CommandType::ATTACK: {
            std::vector<std::pair<std::string, int>> enemy_hp_before;
            for (const auto& e : enemies_) {
                enemy_hp_before.push_back({e.GetDescription(), e.GetHealth()});
            }
            
            player_.AttackEnemies(enemies_);
            
            for (size_t i = 0; i < enemies_.size(); ++i) {
                int damage_dealt = enemy_hp_before[i].second - enemies_[i].GetHealth();
                if (damage_dealt > 0) {
                    DISPATCH_EVENT(Events::PlayerAttacked(enemies_[i].GetDescription(), damage_dealt));
                    DISPATCH_EVENT(Events::EnemyDamaged(enemies_[i].GetDescription(), damage_dealt, enemies_[i].GetHealth()));
                }
            }
            return 1;
        }
        case CommandType::SWITCH_WEAPON:
            player_.SwitchAttack();
            return 1;
        case CommandType::USE_SPELL:
            if (spell_hand_.ShowSpells()) {
                int spell_idx = GetSpellIndexFromPlayer();
                std::string spell_name = spell_hand_.GetSpells()[spell_idx]->GetName();
                
                std::vector<std::pair<std::string, int>> enemy_hp_before;
                for (const auto& e : enemies_) {
                    enemy_hp_before.push_back({e.GetDescription(), e.GetHealth()});
                }
                std::vector<std::pair<std::pair<int,int>, int>> building_hp_before;
                for (const auto& b : field_.GetBuildings()) {
                    building_hp_before.push_back({{b.GetX(), b.GetY()}, b.GetHealth()});
                }
                
                bool spell_used = spell_hand_.UseSpell(spell_idx, enemies_, field_.GetBuildings(),
                                                       player_.GetX(), player_.GetY(), field_);
                
                if (spell_used) {
                    DISPATCH_EVENT(Events::SpellUsed(spell_name, player_.GetX(), player_.GetY()));
                    
                    for (size_t i = 0; i < enemies_.size() && i < enemy_hp_before.size(); ++i) {
                        int damage_dealt = enemy_hp_before[i].second - enemies_[i].GetHealth();
                        if (damage_dealt > 0) {
                            DISPATCH_EVENT(Events::EnemyDamaged(enemies_[i].GetDescription(), damage_dealt, enemies_[i].GetHealth()));
                        }
                    }
                    
                    auto& buildings = field_.GetBuildings();
                    for (size_t i = 0; i < buildings.size() && i < building_hp_before.size(); ++i) {
                        int damage_dealt = building_hp_before[i].second - buildings[i].GetHealth();
                        if (damage_dealt > 0) {
                            DISPATCH_EVENT(Events::BuildingDamaged(buildings[i].GetX(), buildings[i].GetY(), damage_dealt, buildings[i].GetHealth()));
                        }
                    }
                    return 1;
                }
                return 0;
            } else {
                return 0;
            }
        case CommandType::VIEW_SPELLS:
            spell_hand_.ShowSpells();
            return 0;
        case CommandType::SAVE_GAME:
            visualizer_.ShowMessage("Сохранение игры...");
            SaveGame(GetFileNameFromUser());
            return 0;
        case CommandType::LOAD_GAME:
            if (LoadGameMenu()) {
                visualizer_.ShowMessage("Загрузка сохранения...");
            }
            return 0;
        case CommandType::QUIT:
            return -1;
        default:
            visualizer_.ShowError("Неверная команда!");
            return 0;
    }

    int old_x = player_.GetX();
    int old_y = player_.GetY();
    int new_x = player_.GetX() + dx;
    int new_y = player_.GetY() + dy;

    if (!field_.IsInside(new_x, new_y)) {
        visualizer_.ShowError("Вы не можете выйти за пределы карты!");
        return 0;
    }

    for (auto& enemy : enemies_) {
        if (new_x == enemy.GetX() && new_y == enemy.GetY()) {
            int hp_before = player_.GetHealth();
            enemy.Attack(player_);
            int damage_dealt = hp_before - player_.GetHealth();
            if (damage_dealt > 0) {
                DISPATCH_EVENT(Events::PlayerDamaged(damage_dealt, player_.GetHealth()));
            }
            return 1;
        }
    }

    CellType cell_type = field_.GetCellType(new_x, new_y);

    if (cell_type == CellType::WALL || cell_type == CellType::BUILDING || cell_type == CellType::TOWER) {
        visualizer_.ShowError("Эта клетка непроходима!");
        return 0;
    }

    if (cell_type == CellType::TRAP) {
        visualizer_.ShowError("Осторожно, там твоя ловушка!");
        return 0;
    }

    if (player_.GetWasOnSlow()) {
        field_.SetCellType(old_x, old_y, CellType::SLOW);
        player_.SetWasOnSlow(false);
    } else {
        field_.ClearCell(old_x, old_y);
    }

    if (cell_type == CellType::SLOW) {
        player_.SetSlowed(true);
        player_.SetWasOnSlow(true);
        visualizer_.ShowMessage("Вы наступили на замедляющую клетку! Следующий ход будет пропущен.");
    }

    player_.SetPosition(new_x, new_y);
    field_.SetCellType(new_x, new_y, CellType::PLAYER);
    DISPATCH_EVENT(Events::PlayerMoved(new_x, new_y));
    return 1;
}

std::pair<int, int> Game::ChooseEnemyMove(Enemy& enemy) const {
    int old_x = enemy.GetX();
    int old_y = enemy.GetY();
    int prev_x = enemy.GetPrevX();
    int prev_y = enemy.GetPrevY();

    int target_x = player_.GetX();
    int target_y = player_.GetY();

    int min_way = INT_MAX;
    int ans_x = -1, ans_y = -1;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if ((dx == 0 && dy == 0) || (abs(dx) + abs(dy) == 2)) continue;

            int new_x = old_x + dx;
            int new_y = old_y + dy;

            if (!field_.IsInside(new_x, new_y)) continue;

            CellType type_cell = field_.GetCellType(new_x, new_y);
            if (type_cell == CellType::WALL || type_cell == CellType::BUILDING
                || type_cell == CellType::ENEMY || type_cell == CellType::TOWER) {
                continue;
            }

            int dist = abs(target_x - new_x) + abs(target_y - new_y);

            if ((target_x - old_x) * dx + (target_y - old_y) * dy < 0) dist += 1;
            if (new_x == prev_x && new_y == prev_y) dist += 2;

            if (dist < min_way || (dist == min_way && rand() % 2 == 0)) {
                min_way = dist;
                ans_x = new_x;
                ans_y = new_y;
            }
        }
    }

    return {ans_x, ans_y};
}

void Game::UpdateEnemies() {
    int dead_enemies = 0;

    for (auto& enemy : enemies_) {
        if (enemy.IsSlowed()) {
            enemy.SetSlowed(false);
            continue;
        }
        auto [ans_x, ans_y] = ChooseEnemyMove(enemy);
        if (ans_x == -1 && ans_y == -1) {
            continue;
        }
        if (ans_x == player_.GetX() && ans_y == player_.GetY()) {
            if (enemy.GetHealth() > 0) {
                int hp_before = player_.GetHealth();
                enemy.Attack(player_);
                int damage_dealt = hp_before - player_.GetHealth();
                if (damage_dealt > 0) {
                    DISPATCH_EVENT(Events::PlayerDamaged(damage_dealt, player_.GetHealth()));
                }
                player_.AddScore(kHitPointLoss);
            }
            continue;
        }

        bool was_on_slow = enemy.GetWasOnSlow();
        field_.SetCellType(enemy.GetX(), enemy.GetY(), was_on_slow ? CellType::SLOW : CellType::EMPTY);

        CellType target_type = field_.GetCellType(ans_x, ans_y);

        if (target_type == CellType::TRAP) {
            enemy.TakeDamage(TrapSpell::kTrapDamage);
            visualizer_.ShowMessage("Враг попал в ловушку и получил " + std::to_string(TrapSpell::kTrapDamage) + " единицы урона!");
            DISPATCH_EVENT(Events::TrapTriggered(enemy.GetDescription(), TrapSpell::kTrapDamage));
            DISPATCH_EVENT(Events::EnemyDamaged(enemy.GetDescription(), TrapSpell::kTrapDamage, enemy.GetHealth()));
            field_.SetCellType(ans_x, ans_y, CellType::EMPTY);
        }

        if (enemy.GetHealth() == 0) {
            continue;
        }

        enemy.SetWasOnSlow(target_type == CellType::SLOW);
        enemy.SetPosition(ans_x, ans_y);
        field_.SetCellType(ans_x, ans_y, CellType::ENEMY);
    }

    for (int i = static_cast<int>(enemies_.size()) - 1; i >= 0; --i) {
        if (enemies_[i].GetHealth() == 0) {
            visualizer_.ShowMessage(enemies_[i].GetDescription() + " погиб!");
            DISPATCH_EVENT(Events::EnemyKilled(enemies_[i].GetDescription()));
            field_.ClearCell(enemies_[i].GetX(), enemies_[i].GetY());
            field_.SetCellType(enemies_[i].GetX(), enemies_[i].GetY(),
                               enemies_[i].GetWasOnSlow() ? CellType::SLOW : CellType::EMPTY);
            enemies_.erase(enemies_.begin() + i);
            dead_enemies++;
        }
    }

    auto& buildings = field_.GetBuildings();

    for (int i = static_cast<int>(buildings.size()) - 1; i >= 0; --i) {
        if (buildings[i].GetHealth() == 0) {
            visualizer_.ShowMessage("Вражеское здание разрушено!");
            DISPATCH_EVENT(Events::BuildingDestroyed(buildings[i].GetX(), buildings[i].GetY()));
            field_.ClearCell(buildings[i].GetX(), buildings[i].GetY());
            buildings.erase(buildings.begin() + i);
            player_.AddScore(kBuildingDestroyPoints);
        }
    }

    if (dead_enemies > 0) {
        int chance = rand() % 100;
        if (chance < kSpellDropChancePercent) {
            int count_before = spell_hand_.GetSpellCount();
            spell_hand_.AddRandomSpell();
            if (spell_hand_.GetSpellCount() > count_before) {
                DISPATCH_EVENT(Events::SpellReceived(spell_hand_.GetSpells().back()->GetName()));
            }
        }
    }
    player_.AddScore(dead_enemies * kPointsPerKill);
}

void Game::RestartGame() {
    current_level_ = 1;

    current_enemy_health_ = kDefaultEnemyHealth;
    current_enemy_damage_ = kDefaultEnemyDamage;

    enemies_.clear();
    field_ = Field(kStartSizeField, kStartSizeField, kStartCountBuildings,
                   current_enemy_health_, current_enemy_damage_);
    player_ = Player(player_.GetBaseHealth(), player_.GetBaseDamage(), 0, 0);
    field_.SetCellType(player_.GetX(), player_.GetY(), CellType::PLAYER);
    spell_hand_.Clear();
    spell_hand_.AddRandomSpell();
    if (spell_hand_.GetSpellCount() > 0) {
        DISPATCH_EVENT(Events::SpellReceived(spell_hand_.GetSpells().back()->GetName()));
    }
}


void Game::NextLevel() {
    DISPATCH_EVENT(Events::LevelCompleted(current_level_));
    current_level_++;

    if (current_level_ > kMaxLevel) {
        visualizer_.ShowGameComplete();
        return;
    }
    LevelUpMenu();

    visualizer_.ShowLevelTransition(current_level_);
    DISPATCH_EVENT(Events::LevelStarted(current_level_));

    int new_width = field_.GetWidth() + kFieldSizeIncrement;
    int new_height = field_.GetHeight() + kFieldSizeIncrement;
    int new_num_buildings = field_.GetNumBuildings() + kBuildingsPerLevel;
    current_enemy_health_ += kEnemyHealthPerLevel;
    current_enemy_damage_ += kEnemyDamagePerLevel;

    if (new_width > 25) new_width = 25;
    if (new_height > 25) new_height = 25;

    field_ = Field(new_width, new_height, new_num_buildings,
                   current_enemy_health_, current_enemy_damage_);

    player_.SetPosition(0, 0);
    player_.SetHealth(player_.GetMaxHealth());
    spell_hand_.RemoveHalfSpells();

    RunLevel();
}

void Game::LevelUpMenu() {
    visualizer_.RenderLevelUpMenu();

    bool choosing = true;
    while (choosing) {
        auto cmd = controller_.GetMenuChoice();
        int choice = cmd->GetChoice();

        if (choice < 0) {
            visualizer_.ShowError("Некорректный ввод. Попробуйте снова.");
            visualizer_.ShowPrompt("Выберите улучшение: ");
            continue;
        }

        switch (choice) {
            case 1:
                player_.SetMaxHealth(player_.GetMaxHealth() + kPlayerHealthPerLevel);
                visualizer_.ShowMessage("Ваше здоровье увеличено!");
                choosing = false;
                break;

            case 2:
                player_.SetDamage(player_.GetDamage() + kPlayerDamagePerLevel);
                visualizer_.ShowMessage("Ваш урон увеличен!");
                choosing = false;
                break;

            case 3: {
                int count_before = spell_hand_.GetSpellCount();
                spell_hand_.AddRandomSpell();
                if (spell_hand_.GetSpellCount() > count_before) {
                    DISPATCH_EVENT(Events::SpellReceived(spell_hand_.GetSpells().back()->GetName()));
                }
                visualizer_.ShowMessage("Вы получили новое заклинание!");
                choosing = false;
                break;
            }

            default:
                visualizer_.ShowError("Неверный выбор. Попробуйте снова.");
                visualizer_.ShowPrompt("Выберите улучшение: ");
                break;
        }
    }
}

void Game::SaveGame(const std::string& filename) const {
    try {
        std::filesystem::create_directories("saves");
        std::string path = "saves/" + filename + ".json";

        if (std::filesystem::exists(path)) {
            throw FileWriteException("Файл уже существует: " + path);
        }

        std::vector<std::filesystem::directory_entry> saves;
        for (auto& entry : std::filesystem::directory_iterator("saves")) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                saves.push_back(entry);
            }
        }

        if (saves.size() >= kMaxSaves) {
            auto oldest = std::min_element(saves.begin(), saves.end(),
                [](const auto& a, const auto& b) {
                    return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
                });
            std::filesystem::remove(oldest->path());
            std::cerr << "Заменен старейший файл сохранения: " << oldest->path() << "\n";
        }

        json j;
        j["version"] = 1;

        j["field"]["width"] = field_.GetWidth();
        j["field"]["height"] = field_.GetHeight();
        j["field"]["num_buildings"] = field_.GetNumBuildings();
        j["field"]["grid"] = json::array();
        for (int y = 0; y < field_.GetHeight(); ++y) {
            json row = json::array();
            for (int x = 0; x < field_.GetWidth(); ++x) {
                row.push_back(field_.CellTypeToInt(field_.GetCellType(x, y)));
            }
            j["field"]["grid"].push_back(row);
        }

        json jp;
        jp["x"] = player_.GetX();
        jp["y"] = player_.GetY();
        jp["max_health"] = player_.GetMaxHealth();
        jp["health"] = player_.GetHealth();
        jp["damage"] = player_.GetDamage();
        jp["score"] = player_.GetScore();
        jp["attack_type"] = static_cast<int>(player_.GetAttackType());
        jp["slowed"] = player_.IsSlowed();
        jp["was_on_slow"] = player_.GetWasOnSlow();
        jp["base_health"] = player_.GetBaseHealth();
        jp["base_damage"] = player_.GetBaseDamage();
        j["player"] = jp;

        j["enemies"] = json::array();
        for (const auto& e : enemies_) {
            j["enemies"].push_back({
                {"x", e.GetX()},
                {"y", e.GetY()},
                {"health", e.GetHealth()},
                {"damage", e.GetDamage()},
                {"prev_x", e.GetPrevX()},
                {"prev_y", e.GetPrevY()},
                {"slowed", e.IsSlowed()},
                {"was_on_slow", e.GetWasOnSlow()}
            });
        }

        j["buildings"] = json::array();
        for (const auto& b : field_.GetBuildings()) {
            j["buildings"].push_back({
                {"x", b.GetX()},
                {"y", b.GetY()},
                {"health", b.GetHealth()},
                {"spawn_cooldown", b.GetSpawnCooldown()},
                {"cooldown_counter", b.GetCooldownCounter()},
                {"spawned_enemies", b.GetSpawnedEnemies()},
                {"max_enemies", b.GetMaxEnemies()}
            });
        }

        j["towers"] = json::array();
        for (const auto& t : field_.GetTowers()) {
            j["towers"].push_back({
                {"x", t.GetX()},
                {"y", t.GetY()},
                {"can_attack", t.GetCanAttack()},
                {"attack_radius", t.GetAttackRadius()}
            });
        }

        j["spells"] = json::array();
        for (const auto& spell : spell_hand_.GetSpells()) {
            j["spells"].push_back({{"type", spell->GetName()}});
        }

        j["current_level"] = current_level_;
        j["current_enemy_health"] = current_enemy_health_;
        j["current_enemy_damage"] = current_enemy_damage_;

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        j["meta"]["timestamp"] = std::string(std::ctime(&now_c));

        std::string tmpname = path + ".tmp";
        {
            std::ofstream ofs(tmpname, std::ios::trunc);
            if (!ofs.is_open()) throw FileOpenException(tmpname);

            ofs << std::setw(4) << j;
            if (!ofs.good()) throw FileWriteException(tmpname);
        }

        if (std::rename(tmpname.c_str(), path.c_str()) != 0) {
            std::remove(tmpname.c_str());
            throw FileWriteException("Не удалось переименовать временный файл в " + path);
        }

        std::cerr << "Игра сохранена в " << path << "\n";
        DISPATCH_EVENT(Events::GameSaved(path));

    } catch (const SaveGameException& ex) {
        std::cerr << ex.what() << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Неожиданная ошибка при сохранении: " << ex.what() << "\n";
    }
}

void Game::LoadGame(const std::string& filename) {
    try {
        if (!std::filesystem::exists(filename))
            throw LoadGameException("Файл не существует: " + filename);

        std::ifstream ifs(filename);
        if (!ifs.is_open())
            throw LoadGameException("Не удалось открыть файл: " + filename);

        json j;
        try {
            ifs >> j;
        } catch (const std::exception& e) {
            throw JsonParseException(filename + " (" + e.what() + ")");
        }

        int version = j.value("version", 0);
        if (version != 1) throw UnsupportedVersionException(version);

        if (!j.contains("player") || !j["player"].is_object())
            throw LoadGameException("Файл поврежден: отсутствует объект 'player'");
        if (!j.contains("field") || !j["field"].is_object())
            throw LoadGameException("Файл поврежден: отсутствует объект 'field'");

        auto jp = j["player"];
        player_.SetPosition(jp.value("x", 0), jp.value("y", 0));
        player_.SetMaxHealth(jp.value("max_health", 100));
        player_.SetHealth(jp.value("health", 100));
        player_.SetDamage(jp.value("damage", 10));
        player_.SetScore(jp.value("score", 0));
        player_.SetAttackType(static_cast<AttackType>(jp.value("attack_type", 0)));
        player_.SetSlowed(jp.value("slowed", false));
        player_.SetWasOnSlow(jp.value("was_on_slow", false));
        player_.SetBaseHealth(jp.value("base_health", 100));
        player_.SetBaseDamage(jp.value("base_damage", 10));

        int w = j["field"].value("width", 10);
        int h = j["field"].value("height", 10);
        field_ = Field(w, h, 0, j.value("current_enemy_health", 1), j.value("current_enemy_damage", 1));

        const auto& grid = j["field"]["grid"];
        for (int y = 0; y < h && y < (int)grid.size(); ++y) {
            for (int x = 0; x < w && x < (int)grid[y].size(); ++x) {
                int type = grid[y][x];
                CellType cell_type = field_.IntToCellType(type);
                if (cell_type == CellType::ENEMY) cell_type = CellType::EMPTY;
                field_.SetCellType(x, y, cell_type);
            }
        }

        enemies_.clear();
        for (const auto& e : j["enemies"]) {
            Enemy enemy(e.value("health", 1), e.value("damage", 1),
                        e.value("x", 0), e.value("y", 0));
            enemy.SetPrevPosition(e.value("prev_x", enemy.GetX()), e.value("prev_y", enemy.GetY()));
            enemy.SetSlowed(e.value("slowed", false));
            enemy.SetWasOnSlow(e.value("was_on_slow", false));
            enemies_.push_back(enemy);
            field_.SetCellType(enemy.GetX(), enemy.GetY(), CellType::ENEMY);
        }

        field_.GetBuildings().clear();
        for (const auto& b : j["buildings"]) {
            EnemyBuilding eb(b.value("x",0), b.value("y",0), b.value("spawn_cooldown",1),
                             b.value("max_enemies",1));
            eb.SetCooldownCounter(b.value("cooldown_counter",0));
            eb.SetSpawnedEnemies(b.value("spawned_enemies",0));
            field_.GetBuildings().push_back(eb);
        }

        field_.GetTowers().clear();
        for (const auto& t : j["towers"]) {
            EnemyTower et(t.value("x",0), t.value("y",0));
            et.SetCanAttack(t.value("can_attack", true));
            et.SetAttackRadius(t.value("attack_radius",1));
            field_.GetTowers().push_back(et);
        }

        spell_hand_.Clear();
        for (const auto& s : j["spells"]) {
            std::string type = s.value("type", "");
            if (!type.empty()) spell_hand_.AddSpell(type);
        }

        current_level_ = j.value("current_level", 1);
        current_enemy_health_ = j.value("current_enemy_health", 1);
        current_enemy_damage_ = j.value("current_enemy_damage", 1);

        std::cerr << "Игра успешно загружена из " << filename << "\n";
        DISPATCH_EVENT(Events::GameLoaded(filename));

    } catch (const LoadGameException& ex) {
        std::cerr << ex.what() << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Неожиданная ошибка при загрузке: " << ex.what() << "\n";
    }
}
