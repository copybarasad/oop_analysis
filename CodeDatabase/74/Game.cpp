#include "Game.h"
#include "Input.h"
#include "Logger.h"
#include <cctype>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "NormalInputHandler.h"
#include "TargetingInputHandler.h"
#include "SplashTargetingInputHandler.h"
#include "TrapTargetingInputHandler.h"

Game::Game(int width, int height, int numOfEnemies, int numOfTowers, int numOfUnwalkableCells)
    : running(true),
    turn(Turn::Player),
    field(width, height, numOfUnwalkableCells),
    actionMode(PlayerActionMode::Normal),
    move(STARTING_MOVE)
{
    srand(static_cast<unsigned>(time(nullptr)));

    int px, py;
    do {
        px = rand() % width;
        py = rand() % height;
    } while (!field.isCellWalkable(px, py));

    player = Player(PLAYER_MAX_HEALTH, PLAYER_MELEE_DAMAGE, px, py);
    aimPos = player.getCoords();

    // --- ����� ---
    for (int i = 0; i < numOfEnemies; ++i) {
        int ex, ey;
        do {
            ex = rand() % width;
            ey = rand() % height;
        } while (!field.isCellWalkable(ex, ey) || isCellOccupied(ex, ey));

        enemies.emplace_back(Enemy(DEFAULT_ENEMY_HP, DEFAULT_ENEMY_DAMAGE, ex, ey));
    }

    // --- ����� ---
    for (int i = 0; i < numOfTowers; ++i) {
        int tx, ty;
        do {
            tx = rand() % width;
            ty = rand() % height;
        } while (!field.isCellWalkable(tx, ty) || isCellOccupied(tx, ty));

        int range = DEFAULT_TOWER_RANGE;   // ������ ����� 4
        int damage = 2 + rand() % 3;  // ���� 2-4
        int skip = DEFAULT_TOWER_SKIP;    // ������� ����� 3 ����

        Tower tower({ tx, ty }, range, damage, skip);
        field.addTower(tower);
    }
    updateInputHandler();
}

bool Game::isCellOccupied(int x, int y) {
    if (player.getCoords().x == x && player.getCoords().y == y) return true;
    for (const auto& e : enemies)
        if (e.getCoords().x == x && e.getCoords().y == y)
            return true;
    return false;
}

bool Game::isCellOccupiedByEnemy(int x, int y) const {
    for (const auto& e : enemies)
        if (e.getCoords().x == x && e.getCoords().y == y)
            return true;
    return false;
}

void Game::start() {
    running = true;
    setCurrentMessage("");
}

void Game::handleInput(Command cmd) {
    if (inputHandler) {
        inputHandler->handle(*this, cmd);
    }
    updateInputHandler();
}


void Game::castSpell(int index) {
    auto spellPtr = player.getHand().getSpell(index);
    if (!spellPtr) return;

    CastInfo info = spellPtr->getCastInfo();
    currentSpellIndex = index;
    switch (info.type) {
    case SpellType::Damage: {
        Area spellArea = {
            player.getCoords().x - info.distance,
            player.getCoords().y - info.distance,
            player.getCoords().x + info.distance,
            player.getCoords().y + info.distance
        };
        bool hit = false;
        for (auto& e : enemies) {
            if (e.isAlive() && spellArea.contains(e.getCoords())) {
                e.takeDamage(info.damage);
                hit = true;
                break;
            }
        }
        player.getHand().useSpell(index);
        setCurrentMessage(hit ? "Damage spell hit!" : "No enemies in range!");
        switchTurn();
        break;
    }

    case SpellType::Splash:
        setShowHand(false);
        setAim(true, player.getCoords(), true, info.radius, false);
        setCurrentMessage("Select splash area (WASD + E).");
        actionMode = PlayerActionMode::SplashTargeting;
        break;

    case SpellType::Trap:
        setShowHand(false);
        setAim(true, player.getCoords(), false, 0, true);
        setCurrentMessage("Select trap location (WASD + E).");
        actionMode = PlayerActionMode::TrapTargeting;
        break;
    }
}

int Game::selectSpellFromHand() {
    auto& hand = player.getHand();
    int count = hand.getCount();
    if (count == 0) {
        setCurrentMessage("No spells in hand!");
        return -1;
    }

    std::string msg = "Select spell:\n";
    for (int i = 0; i < count; ++i) {
        CastInfo info = hand.getSpell(i)->getCastInfo();
        msg += std::to_string(i) + ": ";
        switch (info.type) {
        case SpellType::Damage: msg += "Damage"; break;
        case SpellType::Splash: msg += "Splash"; break;
        case SpellType::Trap: msg += "Trap"; break;
        }
        msg += " (" + std::to_string(info.damage) + " dmg)\n";
    }

    setCurrentMessage(msg);

    char input;
    while (true) {
        input = getInput();
        if (input >= '0' && input < '0' + count)
            return input - '0';
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Game::applySplashSpell(std::shared_ptr<Spell> spell, const Coords& center) {
    if (!spell) return;
    CastInfo info = spell->getCastInfo();

    Area area = {
        center.x - info.radius,
        center.y - info.radius,
        center.x + info.radius,
        center.y + info.radius
    };

    bool hit = false;
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && area.contains(enemy.getCoords())) {
            enemy.takeDamage(info.damage);
            hit = true;
        }
    }

    setCurrentMessage(hit ? "Splash spell hit enemies!" : "Splash spell casted, no enemies in range.");
    setAim(true, center, true, info.radius, false);
}

void Game::placeTrap(std::shared_ptr<Spell> spell, const Coords& pos, int currentSpellIndex) {
    if (!spell) {
        Logger::log("No trap to place!\n");
        return;
    }
    CastInfo info = spell->getCastInfo();

    if (!field.isCellWalkable(pos.x, pos.y)) {
        setCurrentMessage("Cannot place trap here!");
        setAim(false, pos, false, 0, true);
        return;
    }
    player.getHand().useSpell(currentSpellIndex);
    field.spawnTrap(pos, info.damage);
    setCurrentMessage("Trap placed!");
    setAim(false, pos, false, 0, true);
}

void Game::enemiesTurn() {
    Coords playerPos = player.getCoords();

    // --- ���� ������ ---
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        Coords ePos = enemy.getCoords();
        int dx = playerPos.x - ePos.x;
        int dy = playerPos.y - ePos.y;
        int moveX = 0, moveY = 0;

        if (std::abs(dx) > std::abs(dy)) moveX = (dx > 0) ? 1 : -1;
        else if (dy != 0) moveY = (dy > 0) ? 1 : -1;

        int newX = ePos.x + moveX;
        int newY = ePos.y + moveY;

        if (newX == playerPos.x && newY == playerPos.y) {
            setCurrentMessage("Player was hit by an enemy!");
        }
        else if (field.isCellWalkable(newX, newY) && !isCellOccupied(newX, newY)) {
            enemy.move(moveX, moveY);
        }

        int trapDamage = field.checkTrapDamage(enemy.getCoords());
        if (trapDamage > 0) {
            setCurrentMessage("Enemy stepped on a trap!");
        }
    }

    // --- ����� ����� ---
    for (auto& tower : field.getTowers()) {
        tower.tryAttack(player, move);
    }
}

bool Game::allEnemiesDead() const {
    for (const auto& e : enemies)
        if (e.isAlive()) return false;
    return true;
}

void Game::switchTurn() {
    if (allEnemiesDead()) {
        setCurrentMessage("All enemies defeated! You win!");
        stop();
        return;
    }
    if (turn == Turn::Player) { 
        turn = Turn::Enemies; 
        enemiesTurn();
    }
    else { turn = Turn::Player; move++; }
}

void Game::stop() {
    running = false;
    std::cout << "\n=== Game over ===\n";
}


void Game::updateInputHandler() {
    switch (actionMode) {
    case PlayerActionMode::Normal:
        inputHandler = std::make_unique<NormalInputHandler>();
        break;
    case PlayerActionMode::Targeting:
        inputHandler = std::make_unique<TargetingInputHandler>();
        break;
    case PlayerActionMode::SplashTargeting:
        inputHandler = std::make_unique<SplashTargetingInputHandler>();
        break;
    case PlayerActionMode::TrapTargeting:
        inputHandler = std::make_unique<TrapTargetingInputHandler>();
        break;
    }
}

json Game::serialize() const {
    json j;
    j["levelIndex"] = currentLevelIndex;
    j["running"] = running;
    j["turn"] = static_cast<int>(turn);
    j["actionMode"] = static_cast<int>(actionMode);
    j["currentSpellIndex"] = currentSpellIndex;

    j["aimPos"] = { {"x", aimPos.x}, {"y", aimPos.y} };
    j["move"] = move;

    j["field"] = field.serialize();
    j["player"] = player.serialize();

    j["enemies"] = json::array();
    for (const auto& e : enemies)
        j["enemies"].push_back(e.serialize());

    return j;
}

void Game::deserialize(const json& j) {
    running = j.at("running").get<bool>();
    turn = static_cast<Turn>(j.at("turn").get<int>());
    actionMode = static_cast<PlayerActionMode>(j.at("actionMode").get<int>());
    currentSpellIndex = j.at("currentSpellIndex").get<int>();

    aimPos.x = j.at("aimPos").at("x").get<int>();
    aimPos.y = j.at("aimPos").at("y").get<int>();
    move = j.at("move").get<int>();

    field.deserialize(j.at("field"));
    player.deserialize(j.at("player"));

    enemies.clear();
    for (auto& ej : j.at("enemies")) {
        Enemy e(0, 0, 0, 0);
        e.deserialize(ej);
        enemies.push_back(e);
    }
    currentLevelIndex = j["levelIndex"];

    updateInputHandler();
}

void Game::save() const {
    try {
        std::ofstream file("save.json");
        if (!file.is_open()) {
            Logger::log("Cannot write to save.json");
        }
        else {
            json j = serialize();
            file << j.dump(4);
        }
    }
    catch (const std::exception& e) {
        Logger::log(std::string("Unexpected error while saving: ") + e.what());
    }
}