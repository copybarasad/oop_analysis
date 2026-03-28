#include "game.h"
#include "game_defines.h"
#include "event_dispatcher.h"

#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>


Game::Game()
    : field(nullptr),
      running(true),
      inGame(false),
      currentLevel(1),
      dispatcher(nullptr)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

Game::~Game() {
    cleanup();
}


void Game::cleanup() {
    delete field;
    field = nullptr;
    handSpell.reset();
    player.reset();
    enemies.clear();
}


bool Game::isRunning() const { return running; }
bool Game::isInGame() const { return inGame; }

Player* Game::getPlayerPtr() { return player.get(); }
GameField* Game::getFieldPtr() { return field; }

std::vector<Enemy>& Game::getEnemies() { return enemies; }
const std::vector<Enemy>& Game::getEnemies() const { return enemies; }


void Game::setEventDispatcher(EventDispatcher* d) {
    dispatcher = d;
}


void Game::startNewGame() {
    cleanup();
    inGame = true;
    currentLevel = 1;

    handSpell = std::make_unique<HandSpell>(book, MAX_SIZE_START);
    player    = std::make_unique<Player>(*handSpell);
    field     = new GameField(SIZE_FIELD, book);

    enemies.clear();
    for (int i = 0; i < NUM_ENEMIES; ++i) {
        enemies.emplace_back(
            std::rand() % SIZE_FIELD,
            std::rand() % SIZE_FIELD
        );
    }
}

void Game::gameOver() {
    inGame = false;
    running = false;

    if (dispatcher) {
        Event ev;
        ev.type = EventType::PLAYER_DEATH;
        dispatcher->notify(ev);
    }
}


void Game::returnToMenu() {
    inGame = false;
}


void Game::movePlayer(Direction dir) {
    int x = player->getX();
    int y = player->getY();

    int nx = x, ny = y;
    if (dir == Direction::Up)    ny--;
    if (dir == Direction::Down)  ny++;
    if (dir == Direction::Left)  nx--;
    if (dir == Direction::Right) nx++;

    if (!field->canMoveTo(*player, enemies, nx, ny))
        return;

    player->move(nx, ny);
    player->collectSpell(*field, book, *handSpell);

    if (dispatcher) {
        Event ev;
        ev.type   = EventType::PLAYER_MOVE;
        ev.source = "Player";
        ev.x      = nx;
        ev.y      = ny;
        dispatcher->notify(ev);
    }
}

void Game::castSpell() {
    int tx, ty;
    std::cin >> tx >> ty;
    castSpellAt(tx, ty);
}

void Game::castSpellAt(int tx, int ty) {
    Spell* spell = handSpell->getSpellForName(player->getCurrentSpell());
    if (!spell || !field->inField(tx, ty))
        return;

    if (dispatcher) {
        Event ev;
        ev.type   = EventType::SPELL_CAST;
        ev.source = "Player";
        ev.target = spell->getName();
        dispatcher->notify(ev);
    }

    if (spell->getName() == "Enlightening") {
        spell->cast(
            player->getX(), player->getY(),
            player->getX(), player->getY(),
            enemies, *player
        );
        return;
    }

    if (spell->getName() == "Illusion") {
        if (field->canMoveTo(*player, enemies, tx, ty)) {
            field->addTrap(Trap(tx, ty, spell->getName(), spell->getDamage()));

            if (dispatcher) {
                Event ev;
                ev.type = EventType::TRAP_TRIGGER;
                ev.x    = tx;
                ev.y    = ty;
                dispatcher->notify(ev);
            }
        }
        return;
    }

    bool hit = false;

    for (auto& enemy : enemies) {
        if (!enemy.isAlive())
            continue;

        if (enemy.getX() == tx && enemy.getY() == ty) {
            enemy.takeDamage(spell->getDamage());
            hit = true;

            if (dispatcher) {
                Event ev;
                ev.type   = EventType::ENEMY_DAMAGE;
                ev.source = spell->getName();
                ev.value  = spell->getDamage();
                ev.x      = tx;
                ev.y      = ty;
                dispatcher->notify(ev);
            }

            if (!enemy.isAlive() && dispatcher) {
                Event ev;
                ev.type = EventType::ENEMY_DEATH;
                ev.x    = tx;
                ev.y    = ty;
                dispatcher->notify(ev);
            }
        }
    }

    if (!hit && dispatcher) {
        Event ev;
        ev.type = EventType::SPELL_MISS;
        dispatcher->notify(ev);
    }
}

void Game::chooseSpellByIndex(int index) {
    if (index < 0 || index >= handSpell->getCurrentSize())
        return;

    player->switchSpell(
        handSpell->getSpell(index)->getName()
    );
}


void Game::enemiesMove() {
    for (auto& e : enemies) {
        if (!e.isAlive())
            continue;

        int oldX = e.getX();
        int oldY = e.getY();

        e.move(*player, enemies, *field);

        if (dispatcher && (oldX != e.getX() || oldY != e.getY())) {
            Event ev;
            ev.type = EventType::ENEMY_MOVE;
            ev.x    = e.getX();
            ev.y    = e.getY();
            dispatcher->notify(ev);
        }

        if (std::abs(e.getX() - player->getX()) <= ENEMY_ATTACK_RANGE &&
            std::abs(e.getY() - player->getY()) <= ENEMY_ATTACK_RANGE) {

            e.attack(*player);

            if (!player->isAlive()) {
                gameOver();
                return;
            }


            if (dispatcher) {
                Event ev;
                ev.type   = EventType::PLAYER_DAMAGE;
                ev.source = "Enemy";
                ev.value  = e.getDamage();
                dispatcher->notify(ev);
            }
        }
    }
}

void Game::towersUpdate() {
    field->updateTowers(*player, enemies);

    if (!player->isAlive()) {
        gameOver();
    }

}


bool Game::allEnemiesDead() const {
    for (const auto& e : enemies) {
        if (e.isAlive())
            return false;
    }
    return true;
}


std::vector<std::string> Game::getAvailableSpells() const {
    std::vector<std::string> res;
    for (int i = 0; i < handSpell->getCurrentSize(); ++i) {
        res.push_back(handSpell->getSpell(i)->getName());
    }
    return res;
}

bool Game::loadGame() {
    startNewGame(); 
    return true;
}

bool Game::saveGame() const {
    return true; 
}

