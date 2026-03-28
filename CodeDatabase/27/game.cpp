#include "game.h"

#include "aoe_attack.h"
#include "direct_attack.h"
#include "spellid.h"
#include "trap.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

Game::Game()
    : currentState(GameState::MAIN_MENU),
      currentLevel(1) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Game::Step(const Command& command) {
    lastMessage.clear();

    switch (currentState) {
    case GameState::MAIN_MENU:
        if (command.type == CommandType::START_NEW_GAME) {
            StartNewGame();
            currentState = GameState::PLAYING;
        } else if (command.type == CommandType::LOAD_GAME) {
            try {
                LoadGameFromFile(SAVE_FILE_NAME);
                currentState = GameState::PLAYING;
                SetMessage("Game loaded.");
            } catch (const std::exception& ex) {
                SetMessage(ex.what());
            }
        } else if (command.type == CommandType::EXIT_GAME) {
            currentState = GameState::EXIT;
        }
        break;

    case GameState::PLAYING: {
        if (!gameGrid || !playerCharacter) {
            currentState = GameState::MAIN_MENU;
            SetMessage("Invalid game state. Returned to main menu.");
            break;
        }

        const bool consumed = ApplyPlayingCommand(command);
        if (currentState != GameState::PLAYING) {
            break;
        }

        if (consumed) {
            if (IsGameOver()) {
                currentState = GameState::GAME_OVER;
                break;
            }
            if (CheckLevelCompleted()) {
                currentState = GameState::LEVEL_COMPLETE;
                break;
            }

            HandleEnemiesTurn();
            HandleTowerTurn();

            if (IsGameOver()) {
                currentState = GameState::GAME_OVER;
                break;
            }
            if (CheckLevelCompleted()) {
                currentState = GameState::LEVEL_COMPLETE;
                break;
            }
        }
        break;
    }

    case GameState::LEVEL_COMPLETE:
        if (currentLevel >= MAXIMUM_LEVELS) {
            if (command.type == CommandType::START_NEW_GAME) {
                StartNewGame();
                currentState = GameState::PLAYING;
            } else if (command.type == CommandType::EXIT_GAME) {
                currentState = GameState::EXIT;
            }
        } else {
            ApplyLevelUp(command.type);
            PrepareNextLevel();
            currentState = GameState::PLAYING;
        }
        break;

    case GameState::GAME_OVER:
        if (command.type == CommandType::START_NEW_GAME) {
            StartNewGame();
            currentState = GameState::PLAYING;
        } else if (command.type == CommandType::EXIT_GAME) {
            currentState = GameState::EXIT;
        }
        break;

    case GameState::EXIT:
        break;
    }
}

void Game::StartNewGame() {
    ResetGameState();
    currentLevel = 1;
    StartLevel(currentLevel);
}

void Game::StartLevel(int level) {
    const int size = DEFAULT_GRID_SIZE + (level - 1) * 2;
    gameGrid = std::make_unique<Grid>(size, size);

    constexpr size_t HAND_CAPACITY = 3;
    playerCharacter = std::make_unique<Player>(HAND_CAPACITY);
    playerCharacter->set_parent_grid(gameGrid.get());

    const int centerX = size / 2;
    const int centerY = size / 2;
    if (Point* start = gameGrid->getPoint(centerX, centerY)) {
        playerCharacter->go_to(start);
    }

    enemiesList.clear();
    const int enemyCount = 2 + level;
    for (int i = 0; i < enemyCount; ++i) {
        const int ex = std::rand() % size;
        const int ey = std::rand() % size;
        if (ex == centerX && ey == centerY) {
            --i;
            continue;
        }
        enemiesList.emplace_back(std::make_unique<Enemy>(*gameGrid, ex, ey));
    }

    if (level >= 2) {
        const int tx = std::rand() % size;
        const int ty = std::rand() % size;
        const int radius = 3 + level;
        const int damage = 8 + level * 2;
        enemyTower = std::make_unique<EnemyTower>(*gameGrid, tx, ty, radius, damage);
    } else {
        enemyTower.reset();
    }

    towerAttackToggle = false;
}

void Game::ResetGameState() {
    enemiesList.clear();
    enemyTower.reset();
    playerCharacter.reset();
    gameGrid.reset();
    towerAttackToggle = false;
}

void Game::ApplyLevelUp(CommandType commandType) {
    if (!playerCharacter) {
        return;
    }

    switch (commandType) {
    case CommandType::LEVEL_UP_HP:
        playerCharacter->increase_max_hp(20);
        playerCharacter->heal(20);
        SetMessage("HP increased.");
        break;
    case CommandType::LEVEL_UP_DAMAGE:
        playerCharacter->increase_damage(5);
        SetMessage("Damage increased.");
        break;
    case CommandType::LEVEL_UP_SKIP:
        break;
    default:
        break;
    }
}

void Game::PrepareNextLevel() {
    if (!playerCharacter) {
        return;
    }

    playerCharacter->heal(playerCharacter->get_max_hp());

    if (Hand* hand = playerCharacter->get_hand()) {
        if (hand->size() > 1) {
            hand->remove_random_half();
            SetMessage("Half of hacks have been removed.");
        }
    }

    ++currentLevel;

    const int size = DEFAULT_GRID_SIZE + (currentLevel - 1) * 2;
    gameGrid = std::make_unique<Grid>(size, size);

    const int centerX = size / 2;
    const int centerY = size / 2;

    if (Point* startPoint = gameGrid->getPoint(centerX, centerY)) {
        playerCharacter->set_parent_grid(gameGrid.get());
        playerCharacter->clear_current_point();
        playerCharacter->go_to(startPoint);
    }

    enemiesList.clear();
    const int enemyCount = 2 + currentLevel;
    for (int i = 0; i < enemyCount; ++i) {
        const int ex = std::rand() % size;
        const int ey = std::rand() % size;

        if (ex == centerX && ey == centerY) {
            --i;
            continue;
        }

        Point* ep = gameGrid->getPoint(ex, ey);
        if (!ep || !ep->is_passable() || ep->is_occupied() || ep->has_trap()) {
            --i;
            continue;
        }

        enemiesList.emplace_back(std::make_unique<Enemy>(*gameGrid, ex, ey));
    }

    if (currentLevel >= 2) {
        const int tx = std::rand() % size;
        const int ty = std::rand() % size;
        const int radius = 3 + currentLevel;
        const int damage = 8 + currentLevel * 2;
        enemyTower = std::make_unique<EnemyTower>(*gameGrid, tx, ty, radius, damage);
    } else {
        enemyTower.reset();
    }

    towerAttackToggle = false;
}

bool Game::ApplyPlayingCommand(const Command& command) {
    if (!playerCharacter || !gameGrid) {
        return false;
    }

    const int x = playerCharacter->get_x();
    const int y = playerCharacter->get_y();

    int targetX = x;
    int targetY = y;

    switch (command.type) {
    case CommandType::MOVE_UP:    targetY--; break;
    case CommandType::MOVE_DOWN:  targetY++; break;
    case CommandType::MOVE_LEFT:  targetX--; break;
    case CommandType::MOVE_RIGHT: targetX++; break;

    case CommandType::SWITCH_WEAPON:
        playerCharacter->switch_weapon();
        return true;

    case CommandType::CAST_SPELL: {
        Hand* hand = playerCharacter->get_hand();
        if (!hand || hand->size() == 0) {
            SetMessage("No hacks.");
            return false;
        }
        if (command.spell_index < 0) {
            SetMessage("Invalid spell index.");
            return false;
        }

        const size_t idx = static_cast<size_t>(command.spell_index);
        const bool ok = hand->cast_spell_by_index(idx, *playerCharacter, *gameGrid, command.target_x, command.target_y);
        SetMessage(ok ? "Hack executed." : "Failed to execute hack.");
        return ok;
    }

    case CommandType::OPEN_SHOP: {
        Hand* hand = playerCharacter->get_hand();
        if (!hand) {
            return false;
        }
        if (!command.buy_spell) {
            return false;
        }
        const bool ok = hand->buy_spell(playerCharacter->get_score());
        if (ok) {
            playerCharacter->add_score(-10);
            SetMessage("Bought!");
        } else {
            SetMessage("Purchase failed.");
        }
        return false;
    }

    case CommandType::SAVE_GAME:
        try {
            SaveGame(SAVE_FILE_NAME);
            SetMessage("Game saved.");
        } catch (const std::exception& ex) {
            SetMessage(ex.what());
        }
        return false;

    case CommandType::LOAD_GAME:
        try {
            LoadGameFromFile(SAVE_FILE_NAME);
            SetMessage("Game loaded.");
        } catch (const std::exception& ex) {
            SetMessage(ex.what());
        }
        return false;

    case CommandType::EXIT_GAME:
        currentState = GameState::MAIN_MENU;
        return false;

    default:
        return false;
    }

    Point* targetPoint = gameGrid->getPoint(targetX, targetY);
    if (targetPoint && targetPoint->is_passable()) {
        playerCharacter->go_to(targetPoint);
        return true;
    }

    SetMessage("Cannot move there.");
    return false;
}

void Game::HandleEnemiesTurn() {
    if (!gameGrid || !playerCharacter) {
        return;
    }

    for (auto& enemy : enemiesList) {
        if (!enemy || enemy->get_hp() <= 0) {
            continue;
        }

        const int px = playerCharacter->get_x();
        const int py = playerCharacter->get_y();
        const int ex = enemy->get_x();
        const int ey = enemy->get_y();

        const int dx = (px > ex) ? 1 : (px < ex ? -1 : 0);
        const int dy = (py > ey) ? 1 : (py < ey ? -1 : 0);

        Point* p = gameGrid->getPoint(ex + dx, ey + dy);
        if (p && p->is_passable()) {
            enemy->go_to(p);
        }
    }

    enemiesList.erase(
        std::remove_if(
            enemiesList.begin(),
            enemiesList.end(),
            [](const std::unique_ptr<Enemy>& e) { return !e || e->get_hp() <= 0; }),
        enemiesList.end());
}

void Game::HandleTowerTurn() {
    if (!enemyTower || !playerCharacter) {
        return;
    }

    towerAttackToggle = !towerAttackToggle;
    if (!towerAttackToggle) {
        return;
    }

    enemyTower->try_attack(*playerCharacter);
}

bool Game::CheckLevelCompleted() const {
    return enemiesList.empty() && (!enemyTower || enemyTower->get_hp() <= 0);
}

bool Game::IsGameOver() const {
    return !playerCharacter || playerCharacter->get_hp() <= 0;
}

Point* Game::FindAvailablePosition(int centerX, int centerY, int searchRadius) const {
    if (!gameGrid) {
        return nullptr;
    }

    const int sizeX = gameGrid->get_leng();
    const int sizeY = gameGrid->get_width();

    for (int r = 1; r <= searchRadius; ++r) {
        for (int dx = -r; dx <= r; ++dx) {
            for (int dy = -r; dy <= r; ++dy) {
                const int nx = centerX + dx;
                const int ny = centerY + dy;
                if (nx < 0 || ny < 0 || nx >= sizeX || ny >= sizeY) {
                    continue;
                }

                Point* p = gameGrid->getPoint(nx, ny);
                if (p && p->is_passable() && !p->is_occupied() && !p->has_trap()) {
                    return p;
                }
            }
        }
    }
    return nullptr;
}

unsigned long long Game::CalculateHash(const std::string& data) const {
    unsigned long long hash = 1469598103934665603ull;
    for (unsigned char c : data) {
        hash ^= static_cast<unsigned long long>(c);
        hash *= 1099511628211ull;
    }
    return hash;
}

// --- Save/Load ---

void Game::SaveGame(std::string_view fileName) {
    if (!gameGrid || !playerCharacter) {
        throw SaveGameException("Cannot save: game is not initialized.");
    }

    std::ostringstream payload;

    payload << "LEVEL " << currentLevel << "\n";
    payload << "GRID " << gameGrid->get_leng() << " " << gameGrid->get_width() << "\n";
    payload << "PLAYER " << playerCharacter->get_x() << " " << playerCharacter->get_y()
            << " " << playerCharacter->get_hp() << " " << playerCharacter->get_max_hp()
            << " " << playerCharacter->get_damage() << " " << playerCharacter->get_score()
            << " " << playerCharacter->get_disk_type() << "\n";

    if (Hand* hand = playerCharacter->get_hand()) {
        payload << "HAND " << hand->size() << " " << hand->max_size() << "\n";
        for (const auto& sp : hand->get_spells()) {
            payload << "SPELL " << static_cast<int>(sp->id()) << "\n";
        }
    } else {
        payload << "HAND 0 0\n";
    }

    payload << "ENEMIES " << enemiesList.size() << "\n";
    for (const auto& e : enemiesList) {
        payload << "ENEMY " << e->get_x() << " " << e->get_y() << " " << e->get_hp()
                << " " << e->get_damage() << "\n";
    }

    if (enemyTower && enemyTower->get_hp() > 0) {
        payload << "TOWER 1 " << enemyTower->get_x() << " " << enemyTower->get_y()
                << " " << enemyTower->get_hp() << " " << enemyTower->get_damage() << "\n";
    } else {
        payload << "TOWER 0\n";
    }

    const std::string data = payload.str();
    const unsigned long long hash = CalculateHash(data);

    std::ofstream file(std::string(fileName), std::ios::binary);
    if (!file) {
        throw SaveGameException("Cannot open save file for writing.");
    }

    file << "HASH " << hash << "\n";
    file << data;

    if (!file) {
        throw SaveGameException("Failed while writing save file.");
    }
}

void Game::LoadGameFromFile(std::string_view fileName) {
    std::ifstream file(std::string(fileName), std::ios::binary);
    if (!file) {
        throw LoadGameException("Cannot open save file.");
    }

    std::string token;
    unsigned long long expectedHash = 0;
    if (!(file >> token >> expectedHash) || token != "HASH") {
        throw InvalidSaveDataException("Save file is corrupted (no HASH).");
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::ostringstream rest;
    rest << file.rdbuf();
    const std::string data = rest.str();
    const unsigned long long realHash = CalculateHash(data);
    if (realHash != expectedHash) {
        throw InvalidSaveDataException("Save file is corrupted (HASH mismatch).");
    }

    std::istringstream in(data);

    int level = 1;
    int gridX = 0;
    int gridY = 0;

    if (!(in >> token >> level) || token != "LEVEL") {
        throw InvalidSaveDataException("Invalid save: missing LEVEL.");
    }
    if (!(in >> token >> gridX >> gridY) || token != "GRID") {
        throw InvalidSaveDataException("Invalid save: missing GRID.");
    }

    ResetGameState();
    currentLevel = level;
    gameGrid = std::make_unique<Grid>(gridX, gridY);

    int px = 0, py = 0, php = 0, pmax = 0, patk = 0, pscore = 0, pdisk = 0;
    if (!(in >> token >> px >> py >> php >> pmax >> patk >> pscore >> pdisk) || token != "PLAYER") {
        throw InvalidSaveDataException("Invalid save: missing PLAYER.");
    }

    constexpr size_t DEFAULT_HAND_CAPACITY = 3;
    playerCharacter = std::make_unique<Player>(DEFAULT_HAND_CAPACITY);
    playerCharacter->set_parent_grid(gameGrid.get());
    playerCharacter->set_max_hp(pmax);
    playerCharacter->set_hp(php);
    playerCharacter->set_damage(patk);
    playerCharacter->add_score(pscore);
    playerCharacter->set_disk_type(pdisk);

    if (Point* p = gameGrid->getPoint(px, py)) {
        playerCharacter->go_to(p);
    }

    size_t handSize = 0;
    size_t handCap = 0;
    if (!(in >> token >> handSize >> handCap) || token != "HAND") {
        throw InvalidSaveDataException("Invalid save: missing HAND.");
    }

    if (Hand* hand = playerCharacter->get_hand()) {
        hand->clear_spells();
        for (size_t i = 0; i < handSize; ++i) {
            int spellId = 0;
            if (!(in >> token >> spellId) || token != "SPELL") {
                throw InvalidSaveDataException("Invalid save: bad SPELL entry.");
            }
            hand->add_spell(Hand::CreateSpellForId(static_cast<SpellId>(spellId)));
        }
    }

    size_t enemyCount = 0;
    if (!(in >> token >> enemyCount) || token != "ENEMIES") {
        throw InvalidSaveDataException("Invalid save: missing ENEMIES.");
    }

    enemiesList.clear();
    for (size_t i = 0; i < enemyCount; ++i) {
        int ex = 0, ey = 0, ehp = 0, eatk = 0;
        if (!(in >> token >> ex >> ey >> ehp >> eatk) || token != "ENEMY") {
            throw InvalidSaveDataException("Invalid save: bad ENEMY entry.");
        }
        auto e = std::make_unique<Enemy>(*gameGrid, ex, ey);
        e->set_hp(ehp);
        e->set_damage(eatk);
        enemiesList.emplace_back(std::move(e));
    }

    int hasTower = 0;
    if (!(in >> token >> hasTower) || token != "TOWER") {
        throw InvalidSaveDataException("Invalid save: missing TOWER.");
    }

    if (hasTower != 0) {
        int tx = 0, ty = 0, thp = 0, tatk = 0;
        if (!(in >> tx >> ty >> thp >> tatk)) {
            throw InvalidSaveDataException("Invalid save: bad TOWER entry.");
        }
        enemyTower = std::make_unique<EnemyTower>(*gameGrid, tx, ty, 3 + currentLevel, tatk);
        enemyTower->set_hp(thp);
    } else {
        enemyTower.reset();
    }

    currentState = GameState::PLAYING;
}
