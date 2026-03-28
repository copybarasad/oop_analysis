#include "Game.h"
#include <limits>
#include <cctype>
#include <iostream>
#include <random>
#include <stdexcept>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include "Spell.h"
#include "SpellHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"

Game::Game(std::size_t width, std::size_t height)
    : field_(width, height),
      player_(35, 6, 5, 4),
      enemies_(),
      bases_(),
      towers_(),
      player_skip_turn_next_time_(false),
      quit_requested_(false),
      enemies_killed_(0) {
    Initialize();
    EnsureInvariants();
}

GameField &Game::GetField() {
    return field_;
}

void Game::RenderField() {
    PrintField();
}

void Game::RenderStatus() {
    PrintStatus();
}

const GameField &Game::GetField() const {
    return field_;
}

bool Game::GetPlayerPosition(std::size_t &x, std::size_t &y) const {
    return field_.FindPlayer(x, y);
}

bool Game::DealDirectDamage(std::size_t x, std::size_t y, int damage) {
    if (!field_.IsInside(x, y)) {
        return false;
    }

    Cell::Occupant occupant = field_.GetOccupant(x, y);
    int index = field_.GetOccupantIndex(x, y);

    if (occupant == Cell::Occupant::kEnemy &&
        index >= 0 &&
        static_cast<std::size_t>(index) < enemies_.size()) {
        Enemy &enemy = enemies_[static_cast<std::size_t>(index)];
        if (!enemy.IsAlive()) {
            return false;
        }

        enemy.TakeDamage(damage);
        player_.AddScore(damage);

        GameEvent ev{};
        ev.type = GameEventType::DamageDealt;
        ev.source = "Spell";
        ev.target = "Enemy" + std::to_string(index);
        ev.amount = damage;
        ev.x = static_cast<int>(x);
        ev.y = static_cast<int>(y);
        Notify(ev);

        if (!enemy.IsAlive()) {
            field_.ClearCell(x, y);
            std::cout << "Enemy is defeated by spell.\n";
            OnEnemyKilled();

            GameEvent evd{};
            evd.type = GameEventType::EnemyDied;
            evd.target = "Enemy" + std::to_string(index);
            Notify(evd);
        }
        return true;
    }

    if (occupant == Cell::Occupant::kEnemyBase) {
        std::cout << "The base is hit, but it is not destroyable yet.\n";
        return true;
    }

    return false;
}

void Game::DealAreaDamage(std::size_t x, std::size_t y,
                          std::size_t width, std::size_t height,
                          int damage) {
    for (std::size_t dy = 0; dy < height; ++dy) {
        for (std::size_t dx = 0; dx < width; ++dx) {
            std::size_t cx = x + dx;
            std::size_t cy = y + dy;
            if (!field_.IsInside(cx, cy)) {
                continue;
            }
            DealDirectDamage(cx, cy, damage);
        }
    }
}

std::unique_ptr<Spell> Game::CreateRandomSpell() {
    static std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 4);

    switch (dist(gen)) {
        case 0:
            return std::make_unique<DirectDamageSpell>(15, 4, 0);
        case 1:
            return std::make_unique<AreaDamageSpell>(10, 3, 0);
        case 2:
            return std::make_unique<TrapSpell>(12, 4, 0);
        case 3:
            return std::make_unique<SummonSpell>(12, 4, 0);
        case 4:
        default:
            return std::make_unique<BuffSpell>(1, 0);
    }
}

bool Game::FindAllyPosition(std::size_t ally_index,
                            std::size_t &x,
                            std::size_t &y) const {
    const auto size = field_.GetSize();

    for (std::size_t yy = 0; yy < size.height; ++yy) {
        for (std::size_t xx = 0; xx < size.width; ++xx) {
            if (field_.GetOccupant(xx, yy) == Cell::Occupant::kAlly &&
                field_.GetOccupantIndex(xx, yy) == static_cast<int>(ally_index)) {
                x = xx;
                y = yy;
                return true;
            }
        }
    }

    return false;
}

void Game::Initialize() {
    PlacePlayer();
    PlaceEnemyBases();
    PlaceInitialEnemies();
    PlaceEnemyTowers();
    
    SpellHand &hand = player_.GetSpellHand();

    if (!hand.IsFull())
        hand.AddSpell(CreateRandomSpell());

    if (!hand.IsFull())
        hand.AddSpell(CreateRandomSpell());

    std::cout << "You start with two spells in your hand.\n";
}

void Game::PlacePlayer() {
    GameField::Size size = field_.GetSize();
    for (std::size_t y = 0; y < size.height; ++y) {
        for (std::size_t x = 0; x < size.width; ++x) {
            if (field_.GetTerrain(x, y) == Cell::Terrain::kEmpty &&
                field_.GetOccupant(x, y) == Cell::Occupant::kNone) {
                field_.SetOccupant(x, y, Cell::Occupant::kPlayer, 0);
                return;
            }
        }
    }
    throw std::runtime_error("No free cell for player.");
}

void Game::PlaceEnemyBases() {
    GameField::Size size = field_.GetSize();

    bases_.emplace_back(4);
    bases_.emplace_back(6);

    std::size_t placed = 0;
    for (std::size_t y = 1; y < size.height - 1 && placed < bases_.size(); ++y) {
        for (std::size_t x = 1; x < size.width - 1 && placed < bases_.size(); ++x) {
            if (field_.GetTerrain(x, y) == Cell::Terrain::kEmpty &&
                field_.GetOccupant(x, y) == Cell::Occupant::kNone) {
                field_.SetOccupant(
                    x,
                    y,
                    Cell::Occupant::kEnemyBase,
                    static_cast<int>(placed));
                ++placed;
            }
        }
    }
}

void Game::PlaceEnemyTowers() {
    GameField::Size size = field_.GetSize();

    std::size_t radius = 4;
    std::size_t cooldown = (level_ <= 2) ? 2 : 1;
    int damage = 1 + (level_ - 1);

    towers_.emplace_back(radius, cooldown, damage);
    towers_.emplace_back(radius, cooldown, damage);

    std::size_t placed = 0;
    for (std::size_t y = 1; y < size.height - 1 && placed < towers_.size(); ++y) {
        for (std::size_t x = 1; x < size.width - 1 && placed < towers_.size(); ++x) {
            if (field_.GetTerrain(x, y) == Cell::Terrain::kEmpty &&
                field_.GetOccupant(x, y) == Cell::Occupant::kNone) {
                field_.SetOccupant(
                    x,
                    y,
                    Cell::Occupant::kEnemyTower,
                    static_cast<int>(placed));
                ++placed;
            }
        }
    }
}

void Game::PlaceInitialEnemies() {
    GameField::Size size = field_.GetSize();

    const int initial_enemy_count = 3;
    for (int i = 0; i < initial_enemy_count; ++i) {
        enemies_.emplace_back(8, 2);

        bool placed = false;

        for (std::size_t y = 1; y < size.height - 1 && !placed; ++y) {
            for (std::size_t x = 1; x < size.width - 1 && !placed; ++x) {
                if (field_.GetTerrain(x, y) == Cell::Terrain::kEmpty &&
                    field_.GetOccupant(x, y) == Cell::Occupant::kNone) {
                    field_.SetOccupant(
                        x,
                        y,
                        Cell::Occupant::kEnemy,
                        static_cast<int>(enemies_.size() - 1));
                    placed = true;
                }
            }
        }
    }
}

void Game::Run() {
    while (!quit_requested_) {
        std::cout << "========== Level " << level_ << " ==========\n";

        while (player_.IsAlive() && !quit_requested_ && !level_completed_) {
            PrintField();
            PrintStatus();

            PlayerTurn();
            if (!player_.IsAlive() || quit_requested_ || level_completed_) {
                break;
            }

            AlliesTurn();
            if (level_completed_) {
                break;
            }

            EnemiesTurn();
            if (!player_.IsAlive() || level_completed_) {
                break;
            }

            EnemyBasesTurn();
            EnemyTowersTurn();
        }

        if (!player_.IsAlive() || quit_requested_) {
            break;
        }

        ShowLevelUpMenu();
        StartNextLevel();
    }

    if (!player_.IsAlive()) {
        std::cout << "Game over. Your score: " << player_.GetScore() << std::endl;
    } else if (quit_requested_) {
        std::cout << "You quit the game. Your score: " << player_.GetScore() << std::endl;
    } else {
        std::cout << "Thanks for playing! Your score: " << player_.GetScore() << std::endl;
    }
}

void Game::ShowLevelUpMenu() {
    std::cout << "\n===== LEVEL UP! =====\n";
    std::cout << "You reached level " << level_ << "!\n";
    std::cout << "Choose an upgrade:\n";
    std::cout << "  1) +5 max HP (fully heal)\n";
    std::cout << "  2) +2 melee damage\n";
    std::cout << "  3) +1 ranged range\n";

    int choice = 0;
    bool validChoice = false;

    while (!validChoice) {
        std::cout << "Your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice >= 1 && choice <= 3) {
            validChoice = true;
        } else {
            std::cout << "Invalid choice. Please enter a number between 1 and 3.\n";
        }
    }

    switch (choice) {
        case 1:
            player_.IncreaseMaxHealth(5);
            std::cout << "Max HP increased by 5.\n";
            break;
        case 2:
            player_.IncreaseMeleeDamage(2);
            std::cout << "Melee damage increased by 2.\n";
            break;
        case 3:
            player_.IncreaseRangedRange(1);
            std::cout << "Ranged range increased by 1.\n";
            break;
    }
}

void Game::StartNextLevel() {
    ++level_;
    level_completed_ = false;
    enemies_killed_ = 0;
    enemies_to_kill_for_level_ += 3;
    player_.RestoreFullHealth();

    RemoveHalfSpellsRandom();

    GameField::Size old_size = field_.GetSize();
    std::size_t new_width = std::min<std::size_t>(old_size.width + 2, 25);
    std::size_t new_height = std::min<std::size_t>(old_size.height + 2, 25);

    field_ = GameField(new_width, new_height);

    enemies_.clear();
    allies_.clear();
    bases_.clear();
    towers_.clear();

    PlacePlayer();
    PlaceEnemyBases();
    PlaceInitialEnemies();
    PlaceEnemyTowers();

    std::cout << "\n=== Starting level " << level_
            << " on field " << new_width << "x" << new_height << " ===\n";
}

void Game::RemoveHalfSpellsRandom() {
    SpellHand &hand = player_.GetSpellHand();
    std::size_t size = hand.GetSize();
    if (size <= 1) {
        return;
    }

    std::size_t to_remove = size / 2;
    static std::mt19937 gen{std::random_device{}()};

    for (std::size_t i = 0; i < to_remove && !hand.IsEmpty(); ++i) {
        std::uniform_int_distribution<std::size_t> dist(0, hand.GetSize() - 1);
        std::size_t idx = dist(gen);
        (void) hand.TakeSpell(idx);
    }

    std::cout << "Half of your spells were removed when moving to the next level.\n";
}

bool Game::LevelCompleted() const {
    bool any_enemy_alive = false;
    for (const auto &e: enemies_) {
        if (e.IsAlive()) {
            any_enemy_alive = true;
            break;
        }
    }

    bool any_base_present = !bases_.empty();

    return !any_enemy_alive && !any_base_present;
}

void Game::ResetLevel() {
    GameField::Size size = field_.GetSize();

    enemies_.clear();
    allies_.clear();
    bases_.clear();
    towers_.clear();
    enemies_killed_ = 0;
    player_skip_turn_next_time_ = false;

    field_ = GameField(size.width, size.height);

    Initialize();
}

void Game::SaveToFile(const std::string &filename) const {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Cannot open save file for writing: " + filename);
    }

    GameField::Size size = field_.GetSize();

    out << "TB_SAVE_V1\n";
    out << size.width << ' ' << size.height << '\n';

    out << player_.GetHealth() << ' '
            << player_.GetMaxHealth() << ' '
            << player_.GetScore() << ' '
            << static_cast<int>(player_.GetCombatMode()) << '\n';

    out << enemies_killed_ << ' '
            << (player_skip_turn_next_time_ ? 1 : 0) << '\n';

    for (std::size_t y = 0; y < size.height; ++y) {
        for (std::size_t x = 0; x < size.width; ++x) {
            out << static_cast<int>(field_.GetTerrain(x, y)) << ' '
                    << static_cast<int>(field_.GetOccupant(x, y)) << ' '
                    << field_.GetOccupantIndex(x, y) << ' ';
        }
        out << '\n';
    }

    out << enemies_.size() << '\n';
    for (const auto &e: enemies_) {
        out << e.GetHealth() << ' ' << e.GetDamage() << '\n';
    }

    out << allies_.size() << '\n';
    for (const auto &a: allies_) {
        out << a.GetHealth() << ' ' << a.GetDamage() << '\n';
    }

    out << bases_.size() << '\n';
    for (const auto &b: bases_) {
        out << b.GetSpawnCooldown() << ' ' << b.GetCurrentTimer() << '\n';
    }

    out << towers_.size() << '\n';
    for (const auto &t: towers_) {
        out << t.GetAttackRadius() << ' '
                << t.GetDamage() << ' '
                << t.GetCooldown() << ' '
                << t.GetCurrentTimer() << '\n';
    }

    if (!out) {
        throw std::runtime_error("Error while writing save file: " + filename);
    }
}

void Game::LoadFromFile(const std::string &filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Cannot open save file for reading: " + filename);
    }

    std::string header;
    std::getline(in, header);
    if (header != "TB_SAVE_V1") {
        throw std::runtime_error("Invalid save file format: " + header);
    }

    std::size_t w = 0, h = 0;
    in >> w >> h;
    if (!in) throw std::runtime_error("Invalid field size in save");

    field_ = GameField(w, h);

    int hp = 0, max_hp = 0, score = 0, mode_int = 0;
    in >> hp >> max_hp >> score >> mode_int;
    if (!in) throw std::runtime_error("Invalid player block in save");

    player_.SetMaxHealth(max_hp);
    player_.SetHealth(hp);
    player_.SetScore(score);
    player_.SetCombatMode(static_cast<Player::CombatMode>(mode_int));

    int killed = 0, slowed = 0;
    in >> killed >> slowed;
    enemies_killed_ = killed;
    player_skip_turn_next_time_ = (slowed != 0);

    for (std::size_t y = 0; y < h; ++y) {
        for (std::size_t x = 0; x < w; ++x) {
            int terr = 0, occ = 0, idx = 0;
            in >> terr >> occ >> idx;
            if (!in) throw std::runtime_error("Invalid cell data in save");

            field_.SetTerrain(x, y, static_cast<Cell::Terrain>(terr));
            field_.SetOccupant(x, y, static_cast<Cell::Occupant>(occ), idx);
        }
    }

    enemies_.clear();
    std::size_t enemies_count = 0;
    in >> enemies_count;
    enemies_.reserve(enemies_count);
    for (std::size_t i = 0; i < enemies_count; ++i) {
        int ehp = 0, edmg = 0;
        in >> ehp >> edmg;
        int enemy_hp = 8 + 2 * (level_ - 1);
        int enemy_damage = 2 + (level_ - 1);
        enemies_.emplace_back(enemy_hp, enemy_damage);
    }

    allies_.clear();
    std::size_t allies_count = 0;
    in >> allies_count;
    allies_.reserve(allies_count);
    for (std::size_t i = 0; i < allies_count; ++i) {
        int ahp = 0, admg = 0;
        in >> ahp >> admg;
        allies_.emplace_back(ahp, admg);
    }

    bases_.clear();
    std::size_t bases_count = 0;
    in >> bases_count;
    bases_.reserve(bases_count);
    for (std::size_t i = 0; i < bases_count; ++i) {
        int cooldown = 0, timer = 0;
        in >> cooldown >> timer;
        bases_.emplace_back(cooldown);
        bases_.back().SetCurrentTimer(timer);
    }

    towers_.clear();
    std::size_t towers_count = 0;
    in >> towers_count;
    towers_.reserve(towers_count);
    for (std::size_t i = 0; i < towers_count; ++i) {
        int radius = 0, dmg = 0, cooldown = 0, timer = 0;
        in >> radius >> dmg >> cooldown >> timer;
        towers_.emplace_back(radius, cooldown, dmg);
        towers_.back().SetCurrentTimer(timer);
    }

    if (!in) {
        throw std::runtime_error("Unexpected end of save file: " + filename);
    }

    SpellHand &hand = player_.GetSpellHand();
    hand.Clear();
    if (!hand.IsFull()) hand.AddSpell(CreateRandomSpell());
    if (!hand.IsFull()) hand.AddSpell(CreateRandomSpell());
}

void Game::ClearScreen() const {
    std::cout << "\033[2J\033[H";
}

void Game::PrintField() const {
    ClearScreen();

    GameField::Size size = field_.GetSize();

    const char *kReset = "\033[0m";
    const char *kWallColor = "\033[90m";
    const char *kSlowColor = "\033[36m";
    const char *kPlayerColor = "\033[32m";
    const char *kEnemyColor = "\033[31m";
    const char *kBaseColor = "\033[35m";
    const char *kTowerColor = "\033[33m";
    const char *kTrapColor = "\033[93m";
    const char *kGridColor = "\033[37m";

    std::cout << kGridColor;

    std::cout << "    ";
    for (std::size_t x = 0; x < size.width; ++x) {
        std::cout << (x % 10) << ' ';
    }
    std::cout << kReset << '\n';

    std::cout << kGridColor << "   +";
    for (std::size_t x = 0; x < size.width; ++x) {
        std::cout << "--";
    }
    std::cout << "+" << kReset << '\n';

    for (std::size_t y = 0; y < size.height; ++y) {
        std::cout << kGridColor << ' ' << (y % 10) << " |" << kReset << ' ';

        for (std::size_t x = 0; x < size.width; ++x) {
            Cell::Terrain terrain = field_.GetTerrain(x, y);
            Cell::Occupant occupant = field_.GetOccupant(x, y);

            char symbol = '.';
            const char *color = kGridColor;

            if (terrain == Cell::Terrain::kBlocked) {
                symbol = '#';
                color = kWallColor;
            } else if (terrain == Cell::Terrain::kSlow) {
                symbol = '~';
                color = kSlowColor;
            } else {
                symbol = '.';
                color = kGridColor;
            }

            if (occupant == Cell::Occupant::kPlayer) {
                symbol = 'P';
                color = kPlayerColor;
            } else if (occupant == Cell::Occupant::kEnemy) {
                symbol = 'E';
                color = kEnemyColor;
            } else if (occupant == Cell::Occupant::kEnemyBase) {
                symbol = 'B';
                color = kBaseColor;
            } else if (occupant == Cell::Occupant::kEnemyTower) {
                symbol = 'T';
                color = kTowerColor;
            } else if (occupant == Cell::Occupant::kAlly) {
                symbol = 'A';
                color = "\033[94m";
            }

            if (field_.HasTrap(x, y) &&
                occupant == Cell::Occupant::kNone &&
                terrain != Cell::Terrain::kBlocked) {
                symbol = '^';
                color = kTrapColor;
            }

            std::cout << color << symbol << ' ' << kReset;
        }

        std::cout << kGridColor << "|" << kReset << '\n';
    }

    std::cout << kGridColor << "   +";
    for (std::size_t x = 0; x < size.width; ++x) {
        std::cout << "--";
    }
    std::cout << "+" << kReset << "\n\n";

    std::cout << kPlayerColor << "P" << kReset << " - player, "
            << kEnemyColor << "E" << kReset << " - enemy, "
            << kBaseColor << "B" << kReset << " - enemy base, "
            << kTowerColor << "T" << kReset << " - enemy tower, "
            << kWallColor << "#" << kReset << " - wall, "
            << kSlowColor << "~" << kReset << " - slow cell, "
            << kTrapColor << "^" << kReset << " - trap\n\n";
}

void Game::PrintStatus() const {
    std::cout << "----------------------------------------\n";
    std::cout << "Health: " << player_.GetHealth()
            << "/" << player_.GetMaxHealth()
            << " | Score: " << player_.GetScore()
            << " | Mode: "
            << (player_.GetCombatMode() == Player::CombatMode::kMelee ? "Melee" : "Ranged")
            << " (damage " << player_.GetDamage() << ")\n";

    std::cout << "Enemies: ";
    bool any_enemy_alive = false;
    for (std::size_t i = 0; i < enemies_.size(); ++i) {
        std::size_t ex = 0, ey = 0;
        bool on_field = FindEnemyPosition(i, ex, ey);
        if (!enemies_[i].IsAlive()) {
            std::cout << "[E" << i << ": DEAD] ";
            continue;
        }
        any_enemy_alive = true;
        if (on_field) {
            std::cout << "[E" << i << ": HP=" << enemies_[i].GetHealth()
                    << " at (" << ex << "," << ey << ")] ";
        } else {
            std::cout << "[E" << i << ": HP=" << enemies_[i].GetHealth()
                    << " (off field)] ";
        }
    }
    if (enemies_.empty()) {
        std::cout << "none";
    }
    std::cout << "\n";

    std::cout << "Allies: ";
    if (allies_.empty()) {
        std::cout << "none";
    } else {
        for (std::size_t i = 0; i < allies_.size(); ++i) {
            std::size_t ax = 0, ay = 0;
            bool on_field = FindAllyPosition(i, ax, ay);
            if (!allies_[i].IsAlive()) {
                std::cout << "[A" << i << ": DEAD] ";
                continue;
            }
            if (on_field) {
                std::cout << "[A" << i << ": HP=" << allies_[i].GetHealth()
                        << " at (" << ax << "," << ay << ")] ";
            } else {
                std::cout << "[A" << i << ": HP=" << allies_[i].GetHealth()
                        << " (off field)] ";
            }
        }
    }
    std::cout << "\n";

    std::cout << "Enemies killed: " << enemies_killed_ << "\n";

    std::cout << "Commands: W/A/S/D - move, "
            << "M - switch mode, "
            << "F - ranged attack, "
            << "C - cast spell, "
            << "K - save, "
            << "L - load, "
            << "Q - quit\n";
    std::cout << "----------------------------------------\n";
}

void Game::PlayerTurn() {
    char command = 0;
    bool turn_complited = false;

    while (!turn_complited) {
        if (player_skip_turn_next_time_) {
            std::cout << "You are slowed: you cannot MOVE this turn, "
                    "but you may SWITCH mode, use RANGED attack or CAST a spell.\n";
        }

        std::cout << "Command [W/A/S/D, M, F, C, Q]: ";
        if (!(std::cin >> command)) {
            return;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        command = static_cast<char>(std::toupper(static_cast<unsigned char>(command)));

        if (command == 'Q') {
            quit_requested_ = true;
            turn_complited = true;
            return;
        }

        if (command == 'M') {
            SwitchPlayerMode();
            if (player_skip_turn_next_time_) {
                player_skip_turn_next_time_ = false;
            }
            turn_complited = true;
            return;
        }

        if (command == 'F') {
            PlayerRangedAttack();
            if (player_skip_turn_next_time_) {
                player_skip_turn_next_time_ = false;
            }
            turn_complited = true;
            return;
        }

        if (command == 'C') {
            PlayerCastSpell();
            if (player_skip_turn_next_time_) {
                player_skip_turn_next_time_ = false;
            }
            turn_complited = true;
            return;
        }
        if (command == 'K') {
            try {
                SaveToFile("save.txt");
                std::cout << "Game saved.\n";
            } catch (const std::exception &e) {
                std::cout << "Save error: " << e.what() << "\n";
            }
            turn_complited = true;
            return;
        }

        if (command == 'L') {
            try {
                LoadFromFile("save.txt");
                std::cout << "Game loaded.\n";
            } catch (const std::exception &e) {
                std::cout << "Load error: " << e.what() << "\n";
            }
            turn_complited = true;
            return;
        }

        Direction dir;
        bool is_move = true;

        if (command == 'W') {
            dir = Direction::kUp;
        } else if (command == 'S') {
            dir = Direction::kDown;
        } else if (command == 'A') {
            dir = Direction::kLeft;
        } else if (command == 'D') {
            dir = Direction::kRight;
        } else {
            is_move = false;
        }

        if (!is_move) {
            std::cout << "Unknown command.\n";
            continue;
        }

        if (player_skip_turn_next_time_) {
            std::cout << "You are slowed and cannot move this turn.\n";
            player_skip_turn_next_time_ = false;
            turn_complited = true;
            return;
        }

        MovePlayer(dir);
        turn_complited = true; // return;
    }
}

std::size_t Game::CountAliveEnemies() const {
    std::size_t count = 0;
    for (const auto &e: enemies_) {
        if (e.IsAlive()) {
            ++count;
        }
    }
    return count;
}

void Game::PlayerCastSpell() {
    SpellHand &hand = player_.GetSpellHand();

    if (hand.IsEmpty()) {
        std::cout << "You have no spells in your hand.\n";
        return;
    }

    std::cout << "Spells in hand:\n";
    for (std::size_t i = 0; i < hand.GetSize(); ++i) {
        std::cout << " [" << i << "] " << hand.GetSpellName(i) << "\n";
    }

    std::cout << "Choose spell index (-1 to cancel): ";
    int index = -1;
    if (!(std::cin >> index)) {
        return;
    }
    if (index < 0 || static_cast<std::size_t>(index) >= hand.GetSize()) {
        std::cout << "Spell casting cancelled.\n";
        return;
    }

    std::size_t spell_index = static_cast<std::size_t>(index);
    std::unique_ptr<Spell> spell = hand.TakeSpell(spell_index);

    std::size_t target_x = 0;
    std::size_t target_y = 0;
    std::cout << "Enter target coordinates (x y): ";
    if (!(std::cin >> target_x >> target_y)) {
        std::cout << "Invalid input. Spell is not used.\n";
        hand.AddSpell(std::move(spell));
        return;
    }

    if (!spell->CanCast(*this, target_x, target_y)) {
        std::cout << "You cannot cast this spell at the chosen position.\n";
        hand.AddSpell(std::move(spell));
        return;
    }

    bool success = spell->Cast(*this, target_x, target_y);
    if (!success) {
        std::cout << "Spell had no effect.\n";
        return;
    }

    std::cout << "Spell cast successfully.\n";
}

void Game::PlayerRangedAttack() {
    if (player_.GetCombatMode() != Player::CombatMode::kRanged) {
        std::cout << "You are not in ranged mode. Press M to switch.\n";
        return;
    }

    std::size_t player_x = 0;
    std::size_t player_y = 0;
    if (!field_.FindPlayer(player_x, player_y)) {
        throw std::logic_error("Player position not found.");
    }

    int max_range = player_.GetRange();

    std::vector<std::size_t> target_indices;
    for (std::size_t i = 0; i < enemies_.size(); ++i) {
        if (!enemies_[i].IsAlive()) {
            continue;
        }

        std::size_t ex = 0;
        std::size_t ey = 0;
        if (!FindEnemyPosition(i, ex, ey)) {
            continue;
        }

        int dx = static_cast<int>(ex) - static_cast<int>(player_x);
        int dy = static_cast<int>(ey) - static_cast<int>(player_y);
        int dist = std::abs(dx) + std::abs(dy);

        if (dist <= max_range) {
            target_indices.push_back(i);
        }
    }

    if (target_indices.empty()) {
        std::cout << "No enemies in ranged attack distance.\n";
        return;
    }

    std::cout << "Enemies in range:\n";
    for (std::size_t idx = 0; idx < target_indices.size(); ++idx) {
        std::size_t enemy_index = target_indices[idx];
        std::size_t ex = 0;
        std::size_t ey = 0;
        FindEnemyPosition(enemy_index, ex, ey);

        std::cout << " [" << idx << "] "
                << "Enemy #" << enemy_index
                << " at (" << ex << "," << ey << ")"
                << " HP=" << enemies_[enemy_index].GetHealth()
                << "\n";
    }

    std::cout << "Choose target index (-1 to cancel): ";
    int choice = -1;
    if (!(std::cin >> choice)) {
        return;
    }
    if (choice < 0 || static_cast<std::size_t>(choice) >= target_indices.size()) {
        std::cout << "Ranged attack cancelled.\n";
        return;
    }

    std::size_t chosen_enemy_index = target_indices[static_cast<std::size_t>(choice)];

    int damage = player_.GetDamage();
    enemies_[chosen_enemy_index].TakeDamage(damage);
    player_.AddScore(damage);

    GameEvent ev{};
    ev.type = GameEventType::DamageDealt;
    ev.source = "PlayerRanged";
    ev.target = "Enemy" + std::to_string(chosen_enemy_index);
    ev.amount = damage;
    Notify(ev);

    std::size_t ex = 0;
    std::size_t ey = 0;
    FindEnemyPosition(chosen_enemy_index, ex, ey);

    std::cout << "You hit enemy #" << chosen_enemy_index
            << " for " << damage << " damage.\n";

    if (!enemies_[chosen_enemy_index].IsAlive()) {
        std::cout << "Enemy is defeated by ranged attack.\n";
        field_.ClearCell(ex, ey);
        OnEnemyKilled();

        GameEvent evd{};
        evd.type = GameEventType::EnemyDied;
        evd.target = "Enemy" + std::to_string(chosen_enemy_index);
        Notify(evd);
    }
}

void Game::EnemiesTurn() {
    for (std::size_t i = 0; i < enemies_.size(); ++i) {
        if (!enemies_[i].IsAlive()) {
            continue;
        }
        if (!player_.IsAlive()) {
            return;
        }
        MoveSingleEnemy(i);
    }
    CleanupDead();
}

void Game::AlliesTurn() {
    for (std::size_t i = 0; i < allies_.size(); ++i) {
        std::size_t x, y;
        if (!FindAllyPosition(i, x, y)) continue;
        allies_[i].Move(*this, x, y);
    }
    CleanupDead();
}

void Game::EnemyBasesTurn() {
    const std::size_t kMaxAliveEnemies = 6;

    for (std::size_t i = 0; i < bases_.size(); ++i) {
        if (!bases_[i].Tick()) {
            continue;
        }
        if (CountAliveEnemies() >= kMaxAliveEnemies) {
            continue;
        }

        std::size_t base_x = 0, base_y = 0;
        if (!FindBasePosition(i, base_x, base_y)) {
            continue;
        }
        SpawnEnemyNearBase(i, base_x, base_y);
    }
}

void Game::EnemyTowersTurn() {
    std::size_t player_x = 0;
    std::size_t player_y = 0;
    if (!field_.FindPlayer(player_x, player_y)) {
        return;
    }

    for (std::size_t i = 0; i < towers_.size(); ++i) {
        std::size_t tx = 0;
        std::size_t ty = 0;
        if (!FindTowerPosition(i, tx, ty)) {
            continue;
        }

        bool ready = towers_[i].Tick();
        if (!ready) {
            continue;
        }

        int dx = static_cast<int>(player_x) - static_cast<int>(tx);
        int dy = static_cast<int>(player_y) - static_cast<int>(ty);
        int dist = std::max(std::abs(dx), std::abs(dy));

        if (dist <= static_cast<int>(towers_[i].GetAttackRadius())) {
            int damage = towers_[i].GetDamage();
            player_.TakeDamage(damage);
            std::cout << "Enemy tower hits you for "
                    << damage << " damage.\n";

            GameEvent ev{};
            ev.type = GameEventType::DamageDealt;
            ev.source = "Tower" + std::to_string(i);
            ev.target = "Player";
            ev.amount = damage;
            Notify(ev);

            towers_[i].ResetCooldown();
        }
    }
}

void Game::MovePlayer(Direction direction) {
    std::size_t player_x = 0;
    std::size_t player_y = 0;
    if (!field_.FindPlayer(player_x, player_y)) {
        throw std::logic_error("Player position not found.");
    }

    int dx = 0;
    int dy = 0;
    if (direction == Direction::kUp) {
        dy = -1;
    } else if (direction == Direction::kDown) {
        dy = 1;
    } else if (direction == Direction::kLeft) {
        dx = -1;
    } else if (direction == Direction::kRight) {
        dx = 1;
    }

    int new_x_int = static_cast<int>(player_x) + dx;
    int new_y_int = static_cast<int>(player_y) + dy;

    GameField::Size size = field_.GetSize();
    if (new_x_int < 0 || new_y_int < 0 ||
        new_x_int >= static_cast<int>(size.width) ||
        new_y_int >= static_cast<int>(size.height)) {
        std::cout << "You cannot move outside the field.\n";
        return;
    }

    std::size_t new_x = static_cast<std::size_t>(new_x_int);
    std::size_t new_y = static_cast<std::size_t>(new_y_int);

    Cell::Terrain terrain = field_.GetTerrain(new_x, new_y);
    if (terrain == Cell::Terrain::kBlocked) {
        std::cout << "The cell is blocked.\n";
        return;
    }

    Cell::Occupant occupant = field_.GetOccupant(new_x, new_y);
    if (occupant == Cell::Occupant::kEnemy) {
        int enemy_index = field_.GetOccupantIndex(new_x, new_y);
        if (enemy_index >= 0 &&
            static_cast<std::size_t>(enemy_index) < enemies_.size() &&
            enemies_[static_cast<std::size_t>(enemy_index)].IsAlive()) {
            int damage = player_.GetDamage();
            enemies_[static_cast<std::size_t>(enemy_index)].TakeDamage(damage);
            player_.AddScore(damage);
            std::cout << "You hit an enemy for " << damage << " damage.\n";

            GameEvent ev{};
            ev.type = GameEventType::DamageDealt;
            ev.source = "Player";
            ev.target = "Enemy" + std::to_string(enemy_index);
            ev.amount = damage;
            Notify(ev);

            if (!enemies_[static_cast<std::size_t>(enemy_index)].IsAlive()) {
                std::cout << "Enemy is defeated.\n";
                field_.ClearCell(new_x, new_y);
                OnEnemyKilled();

                GameEvent evd{};
                evd.type = GameEventType::EnemyDied;
                evd.target = "Enemy" + std::to_string(enemy_index);
                Notify(evd);
            }
        }
        return;
    } else if (occupant == Cell::Occupant::kEnemyBase) {
        std::cout << "You cannot move into an enemy base.\n";
        return;
    } else if (occupant == Cell::Occupant::kEnemyTower) {
        std::cout << "You cannot move into an enemy tower.\n";
        return;
    }

    field_.ClearCell(player_x, player_y);
    field_.SetOccupant(new_x, new_y, Cell::Occupant::kPlayer, 0);
    ProcessPlayerCell(new_x, new_y);

    GameEvent ev{};
    ev.type = GameEventType::PlayerMoved;
    ev.x = static_cast<int>(new_x);
    ev.y = static_cast<int>(new_y);
    Notify(ev);
}

void Game::SwitchPlayerMode() {
    player_.SwitchMode();
    std::cout << "You switched to "
            << (player_.GetCombatMode() == Player::CombatMode::kMelee ? "melee" : "ranged")
            << " mode.\n";
}

void Game::MoveSingleEnemy(std::size_t enemy_index) {
    std::size_t ex = 0, ey = 0;
    if (!FindEnemyPosition(enemy_index, ex, ey)) {
        return;
    }

    std::size_t px = 0, py = 0;
    if (!field_.FindPlayer(px, py)) {
        return;
    }

    GameField::Size size = field_.GetSize();
    int damage = enemies_[enemy_index].GetDamage();

    static const int dirs[4][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    for (auto &d: dirs) {
        int nx_i = (int) ex + d[0];
        int ny_i = (int) ey + d[1];

        if (nx_i < 0 || ny_i < 0 ||
            nx_i >= (int) size.width ||
            ny_i >= (int) size.height)
            continue;

        std::size_t nx = (std::size_t) nx_i;
        std::size_t ny = (std::size_t) ny_i;

        Cell::Occupant occ = field_.GetOccupant(nx, ny);
        int occ_index = field_.GetOccupantIndex(nx, ny);

        if (occ == Cell::Occupant::kPlayer) {
            player_.TakeDamage(damage);
            std::cout << "Enemy hits you for " << damage << " damage.\n";

            GameEvent ev{};
            ev.type = GameEventType::DamageDealt;
            ev.source = "Enemy" + std::to_string(enemy_index);
            ev.target = "Player";
            ev.amount = damage;
            Notify(ev);
            return;
        }

        if (occ == Cell::Occupant::kAlly &&
            occ_index >= 0 &&
            (size_t) occ_index < allies_.size()) {
            allies_[occ_index].TakeDamage(damage);
            std::cout << "Enemy hits ally #" << occ_index
                    << " for " << damage << " damage.\n";

            GameEvent ev{};
            ev.type = GameEventType::DamageDealt;
            ev.source = "Enemy" + std::to_string(enemy_index);
            ev.target = "Ally" + std::to_string(occ_index);
            ev.amount = damage;
            Notify(ev);

            if (!allies_[occ_index].IsAlive()) {
                std::cout << "Ally #" << occ_index << " died.\n";
                field_.ClearCell(nx, ny);

                GameEvent eva{};
                eva.target = "Ally" + std::to_string(occ_index);
                Notify(eva);
            }
            return;
        }
    }

    int dx = 0, dy = 0;
    if (ex < px) dx = 1;
    else if (ex > px) dx = -1;
    else if (ey < py) dy = 1;
    else if (ey > py) dy = -1;

    if (dx == 0 && dy == 0) {
        return;
    }

    int tx_i = (int) ex + dx;
    int ty_i = (int) ey + dy;

    if (tx_i < 0 || ty_i < 0 ||
        tx_i >= (int) size.width ||
        ty_i >= (int) size.height) {
        return;
    }

    std::size_t tx = (std::size_t) tx_i;
    std::size_t ty = (std::size_t) ty_i;

    if (field_.GetTerrain(tx, ty) == Cell::Terrain::kBlocked) {
        return;
    }

    Cell::Occupant occupant = field_.GetOccupant(tx, ty);
    if (occupant != Cell::Occupant::kNone) {
        return;
    }

    if (field_.HasTrap(tx, ty)) {
        int trap_dmg = field_.GetTrapDamage(tx, ty);
        enemies_[enemy_index].TakeDamage(trap_dmg);

        std::cout << "Enemy stepped on a trap and took "
                << trap_dmg << " damage.\n";

        GameEvent ev{};
        ev.type = GameEventType::DamageDealt;
        ev.source = "Trap";
        ev.target = "Enemy" + std::to_string(enemy_index);
        ev.amount = trap_dmg;
        ev.x = static_cast<int>(tx);
        ev.y = static_cast<int>(ty);
        Notify(ev);

        field_.ClearTrap(tx, ty);

        if (!enemies_[enemy_index].IsAlive()) {
            std::cout << "Enemy died in a trap.\n";
            OnEnemyKilled();
            field_.ClearCell(ex, ey);

            GameEvent evd{};
            evd.type = GameEventType::EnemyDied;
            evd.target = "Enemy" + std::to_string(enemy_index);
            Notify(evd);
            return;
        }
    }

    field_.ClearCell(ex, ey);
    field_.SetOccupant(
        tx, ty,
        Cell::Occupant::kEnemy,
        (int) enemy_index
    );
}

bool Game::FindEnemyPosition(std::size_t enemy_index,
                             std::size_t &x,
                             std::size_t &y) const {
    GameField::Size size = field_.GetSize();
    for (std::size_t row = 0; row < size.height; ++row) {
        for (std::size_t col = 0; col < size.width; ++col) {
            if (field_.GetOccupant(col, row) == Cell::Occupant::kEnemy &&
                field_.GetOccupantIndex(col, row) == static_cast<int>(enemy_index)) {
                x = col;
                y = row;
                return true;
            }
        }
    }
    return false;
}

void Game::CleanupDead() {
    for (std::size_t i = 0; i < enemies_.size(); ++i) {
        if (!enemies_[i].IsAlive()) {
            std::size_t x, y;
            if (FindEnemyPosition(i, x, y)) {
                field_.ClearCell(x, y);
            }
        }
    }

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
                       [](const Enemy &e) { return !e.IsAlive(); }),
        enemies_.end()
    );

    for (std::size_t i = 0; i < allies_.size(); ++i) {
        if (!allies_[i].IsAlive()) {
            std::size_t x, y;
            if (FindAllyPosition(i, x, y)) {
                field_.ClearCell(x, y);
            }
        }
    }

    allies_.erase(
        std::remove_if(allies_.begin(), allies_.end(),
                       [](const Ally &a) { return !a.IsAlive(); }),
        allies_.end()
    );
}

bool Game::FindBasePosition(std::size_t base_index,
                            std::size_t &x,
                            std::size_t &y) const {
    GameField::Size size = field_.GetSize();
    for (std::size_t row = 0; row < size.height; ++row) {
        for (std::size_t col = 0; col < size.width; ++col) {
            if (field_.GetOccupant(col, row) == Cell::Occupant::kEnemyBase &&
                field_.GetOccupantIndex(col, row) == static_cast<int>(base_index)) {
                x = col;
                y = row;
                return true;
            }
        }
    }
    return false;
}

bool Game::FindTowerPosition(std::size_t tower_index,
                             std::size_t &x,
                             std::size_t &y) const {
    GameField::Size size = field_.GetSize();
    for (std::size_t row = 0; row < size.height; ++row) {
        for (std::size_t col = 0; col < size.width; ++col) {
            if (field_.GetOccupant(col, row) == Cell::Occupant::kEnemyTower &&
                field_.GetOccupantIndex(col, row) == static_cast<int>(tower_index)) {
                x = col;
                y = row;
                return true;
            }
        }
    }
    return false;
}

void Game::SpawnEnemyNearBase(std::size_t,
                              std::size_t base_x,
                              std::size_t base_y) {
    GameField::Size size = field_.GetSize();

    const int directions[4][2] = {
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1}
    };

    for (int i = 0; i < 4; ++i) {
        int new_x_int = static_cast<int>(base_x) + directions[i][0];
        int new_y_int = static_cast<int>(base_y) + directions[i][1];

        if (new_x_int < 0 || new_y_int < 0 ||
            new_x_int >= static_cast<int>(size.width) ||
            new_y_int >= static_cast<int>(size.height)) {
            continue;
        }

        std::size_t new_x = static_cast<std::size_t>(new_x_int);
        std::size_t new_y = static_cast<std::size_t>(new_y_int);

        if (field_.GetTerrain(new_x, new_y) == Cell::Terrain::kBlocked) {
            continue;
        }

        if (field_.GetOccupant(new_x, new_y) != Cell::Occupant::kNone) {
            continue;
        }

        int enemy_hp = 8 + 2 * (level_ - 1);
        int enemy_damage = 2 + (level_ - 1);
        enemies_.emplace_back(enemy_hp, enemy_damage);
        int new_enemy_index = static_cast<int>(enemies_.size() - 1);
        field_.SetOccupant(
            new_x,
            new_y,
            Cell::Occupant::kEnemy,
            new_enemy_index);
        std::cout << "Enemy spawned near a base.\n";

        GameEvent ev{};
        ev.type = GameEventType::EnemySpawned;
        ev.target = "Enemy" + std::to_string(new_enemy_index);
        ev.x = static_cast<int>(new_x);
        ev.y = static_cast<int>(new_y);
        Notify(ev);
        return;
    }
}

void Game::ProcessPlayerCell(std::size_t x, std::size_t y) {
    Cell::Terrain terrain = field_.GetTerrain(x, y);
    if (terrain == Cell::Terrain::kSlow) {
        player_skip_turn_next_time_ = true;
        std::cout << "You step on a slow cell. "
                "Next turn you will not be able to move.\n";
    }
}

Player &Game::GetPlayer() {
    return player_;
}

const Player &Game::GetPlayer() const {
    return player_;
}

void Game::OnEnemyKilled() {
    ++enemies_killed_;

    SpellHand &hand = player_.GetSpellHand();
    if (enemies_killed_ % 3 == 0 && !hand.IsFull()) {
        player_.AddSpellToHand(CreateRandomSpell());
        std::cout << "You received a new spell card!\n";

        GameEvent ev{};
        ev.type = GameEventType::SpellReceived;
        ev.target = "Player";
        ev.extra = "RandomSpell";
        Notify(ev);
    }

    if (enemies_killed_ >= enemies_to_kill_for_level_) {
        std::cout << "\n*** You have killed enough enemies ("
                << enemies_killed_ << "). Level completed! ***\n";
        level_completed_ = true;
    }
}

void Game::EnsureInvariants() const {
    GameField::Size size = field_.GetSize();
    if (size.width < 10 || size.width > 25 ||
        size.height < 10 || size.height > 25) {
        throw std::logic_error("Game field size invariant violated.");
    }
}

bool Game::IsPlayerAlive() const {
    return player_.IsAlive();
}

bool Game::IsQuitRequested() const {
    return quit_requested_;
}

void Game::MovePlayerUp() {
    MovePlayer(Direction::kUp);
}

void Game::MovePlayerDown() {
    MovePlayer(Direction::kDown);
}

void Game::MovePlayerLeft() {
    MovePlayer(Direction::kLeft);
}

void Game::MovePlayerRight() {
    MovePlayer(Direction::kRight);
}

void Game::RequestQuit() {
    quit_requested_ = true;
}

void Game::PublicSwitchPlayerMode() {
    SwitchPlayerMode();
}

void Game::PublicPlayerRangedAttack() {
    PlayerRangedAttack();
}

void Game::PublicPlayerCastSpell() {
    PlayerCastSpell();
}

void Game::ProcessNonPlayerTurn() {
    if (!player_.IsAlive() || quit_requested_) {
        return;
    }

    AlliesTurn();
    if (quit_requested_ || !player_.IsAlive() || level_completed_) {
        return;
    }

    EnemiesTurn();
    if (quit_requested_ || !player_.IsAlive() || level_completed_) {
        return;
    }

    EnemyBasesTurn();
    EnemyTowersTurn();
    if (quit_requested_ || !player_.IsAlive() || level_completed_) {
        return;
    }

    if (LevelCompleted()) {
        std::cout << "You have killed enough enemies " << enemies_killed_ << ". Level completed!\n";
        level_completed_ = true;

        GameEvent ev{};
        ev.extra = "Level " + std::to_string(level_);
        Notify(ev);

        ShowLevelUpMenu();
        StartNextLevel();
    }
}
void Game::AddObserver(IGameObserver* observer) {
    observers_.push_back(observer);
}

void Game::Notify(const GameEvent& event) {
    for (auto* obs : observers_) {
        if (obs) {
            obs->OnGameEvent(event);
        }
    }
}