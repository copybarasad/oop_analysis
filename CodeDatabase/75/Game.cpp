#include "Architecture.h"
#include "Game.h"
#include "VisualEffects.h"
#include "ResourceManager.h"
#include "WorldObject.h"
#include "Enemy.h"
#include "Boss.h" 
#include "Player.h"
#include "GameField.h"
#include "Shield.h"
#include "ThornsArea.h"
#include "Ally.h"
#include "Trap.h"
#include "Tower.h"
#include "GameLevel.h"
#include "Portal.h"
#include "Shop.h"
#include "SaveManager.h" 
#include "GameExceptions.h"
#include <iostream>
#include <cmath>
#include <algorithm> 

Game::Game( const LevelConfig& config, GameMode mode, int currentLevelIndex) 
    : m_dtClock(), m_field(nullptr), m_player(nullptr),
      m_isRunning(true), m_isPaused(false), m_gameState(State::PLAYING), m_gameMode(mode),
      m_currentLevelIndex(currentLevelIndex), m_lastDirection(Bullet::Direction::DOWN),
      m_targetCursorX(0), m_targetCursorY(0), m_lockedTarget(nullptr), m_playerMoveCooldown(0.0f)
{
    initializeFromConfig(config, false); 
    if (m_gameMode == GameMode::STORY && m_currentLevelIndex == 1) {
        if(m_player) m_player->clearHand();
        VisualEffects::printColored("Story Mode: You start with nothing!", sf::Color::Cyan);
    }
}

Game::~Game() { cleanup(false); }

void Game::cleanup(bool keepPlayer) {
    delete m_field; if (!keepPlayer) delete m_player;
    for(auto* e : m_enemies) delete e; for(auto* o : m_worldObjects) delete o;
    for(auto* s : m_shields) delete s; for(auto* t : m_thornsAreas) delete t;
    for(auto* a : m_allies) delete a; for(auto* tr : m_traps) delete tr;
    for(auto* tw : m_towers) delete tw; delete m_portal; m_portal = nullptr;
    delete m_shop; m_shop = nullptr; m_enemies.clear(); m_worldObjects.clear();
    m_shields.clear(); m_thornsAreas.clear(); m_allies.clear(); m_traps.clear();
    m_towers.clear(); Enemy::s_spawnQueue.clear(); 
}



void Game::initializeFromConfig(const LevelConfig& config, bool preservePlayer) {
    m_levelConfig = config;
    m_field = new GameField(config.fieldWidth, config.fieldHeight);
    m_field->setFloorTexture(config.floorTexture);
    
    if (preservePlayer && m_player) {
        m_player->teleport(config.playerStartX, config.playerStartY);
    } else {
        m_player = new Player(config.playerStartX, config.playerStartY);
        m_player->teleport(config.playerStartX, config.playerStartY);
    }
    
    if (config.playerHealthMultiplier > 1.0f) {
        int baseHP = 100;
        int newMax = (int)(baseHP * config.playerHealthMultiplier);
        m_player->setStats(newMax, newMax, m_player->getMana(), m_player->getMaxMana());
    }

    spawnWorld(); spawnTowers(); spawnEnemies();
}

void Game::spawnEnemies() {
    bool hasBoss = false;
    for(int t : m_levelConfig.allowedEnemyTypes) if(t == (int)Enemy::Type::BOSS) hasBoss = true;
    if (hasBoss) {
        int x = m_levelConfig.fieldWidth/2, y = 5;
        Boss* boss = new Boss(x, y, m_currentLevelIndex);
        boss->setGameField(m_field); m_enemies.push_back(boss);
        VisualEffects::printColored("WARNING: DEMON LORD SPAWNED", sf::Color::Red);
        return;
    }
    if (m_levelConfig.allowedEnemyTypes.empty()) return;
    for(int i=0; i<m_levelConfig.initialEnemyCount; ++i) {
        int x = rand() % m_field->getWidth(); int y = rand() % m_field->getHeight();
        if(isValidMove(x,y)) {
            int typeIdx = rand() % m_levelConfig.allowedEnemyTypes.size();
            Enemy::Type type = static_cast<Enemy::Type>(m_levelConfig.allowedEnemyTypes[typeIdx]);
            auto* e = new Enemy(x, y, type, "Enemy", m_levelConfig.enemyHealth, m_levelConfig.enemyDamage);
            e->setGameField(m_field); m_enemies.push_back(e);
        }
    }
}
void Game::spawnWorld() {
    for (const auto& p : m_levelConfig.obstaclePlacements) {
        auto* obj = new WorldObject(p.x, p.y, static_cast<WorldObject::Type>(p.type), p.color);
        obj->drawOnField(*m_field); m_worldObjects.push_back(obj);
    }
    for(int y=0; y<m_field->getHeight(); ++y) { 
        m_worldObjects.push_back(new WorldObject(0, y, WorldObject::WALL)); m_worldObjects.push_back(new WorldObject(m_field->getWidth()-1, y, WorldObject::WALL));
        m_field->getCell(0, y).setType(Cell::WALL); m_field->getCell(m_field->getWidth()-1, y).setType(Cell::WALL); 
    }
    for(int x=0; x<m_field->getWidth(); ++x) { 
        m_worldObjects.push_back(new WorldObject(x, 0, WorldObject::WALL)); m_worldObjects.push_back(new WorldObject(x, m_field->getHeight()-1, WorldObject::WALL));
        m_field->getCell(x, 0).setType(Cell::WALL); m_field->getCell(x, m_field->getHeight()-1).setType(Cell::WALL); 
    }
}
void Game::spawnTowers() {
    for (const auto& t : m_levelConfig.towerPlacements) {
        int dmg = 10 + m_currentLevelIndex * 2;
        auto* tower = new Tower(t.x, t.y, dmg, 10, 2000); m_towers.push_back(tower);
        for(const auto& p : tower->getBlocks()) if(m_field->isInBounds(p.first, p.second)) m_field->getCell(p.first, p.second).setType(Cell::WALL);
        VisualEffects::printColored("Enemy Tower spawned!", sf::Color::Red);
    }
}

void Game::update(float dt) {
    // FIX: Обновляем таймер кулдауна движения
    if (m_playerMoveCooldown > 0.0f) m_playerMoveCooldown -= dt;
    if (m_gameState == State::PLAYING && m_enemies.empty() && m_levelConfig.initialEnemyCount > 0) {
        m_gameState = State::VICTORY;
        VisualEffects::printColored("VICTORY!", sf::Color::Green);
        return;
    }
    if (!m_player->isAlive() && (m_gameState == State::PLAYING || m_gameState == State::HUB)) {
        m_gameState = State::GAME_OVER;
        VisualEffects::printColored("YOU DIED", sf::Color::Red);
        return;
    }
    
    // Process Spawn Queue
    while(!Enemy::s_spawnQueue.empty()) {
        Enemy* e = Enemy::s_spawnQueue.back(); Enemy::s_spawnQueue.pop_back();
        if (isValidMove(e->getX(), e->getY())) {
            e->setGameField(m_field); m_enemies.push_back(e);
            VisualEffects::printColored("Minion Spawned!", sf::Color::Red);
        } else { delete e; }
    }

    m_player->update(dt);
    
    if (m_player->isTargeting()) {
        if (m_lockedTarget && m_lockedTarget->isAlive()) {
            m_targetCursorX = m_lockedTarget->getX(); m_targetCursorY = m_lockedTarget->getY();
        } else {
            float minDst = 999.0f; m_lockedTarget = nullptr;
            for(auto* e : m_enemies) {
                if(e->isAlive()) {
                    float d = std::sqrt(std::pow(e->getX() - m_player->getX(), 2) + std::pow(e->getY() - m_player->getY(), 2));
                    if(d < minDst) { minDst = d; m_lockedTarget = e; }
                }
            }
            if(m_lockedTarget) { m_targetCursorX = m_lockedTarget->getX(); m_targetCursorY = m_lockedTarget->getY(); }
        }
    }
    
    for(auto it = m_visualEvents.begin(); it != m_visualEvents.end();) {
        it->lifetime -= 0.016f; if(it->lifetime <= 0) it = m_visualEvents.erase(it); else ++it;
    }
    
    for (auto* ally : m_allies) { 
        if (ally->isAlive()) { 
            ally->setGameField(m_field); 
            ally->update(dt); 
            ally->updateAI(m_enemies, this); 
        } 
    }

    for (auto* t : m_towers) { t->setGameField(m_field); t->update(dt); if(t->isInRange(m_player->getX(), m_player->getY()) && t->canAttack()) { if (t->attackPlayer(m_player->getX(), m_player->getY())) { m_player->getDamaged(t->getDamage()); addVisualEvent(VisualEvent::BEAM, t->getX(), t->getY(), m_player->getX(), m_player->getY(), sf::Color::Red); } } }
    
    for(auto* e : m_enemies) {
        if(e->isAlive()) {
            e->update(dt);
            // --- RESTORED ENEMY MOVEMENT ---
            if (!e->hasStatus(Character::Status::FROZEN)) {
                e->moveTowards(m_player->getX(), m_player->getY());
            }
            
            // Shoot logic
            if (e->getType() == Enemy::Type::RANGED || e->getType() == Enemy::Type::MAGE || e->getType() == Enemy::Type::BOSS) {
               if(rand() % 200 < 5) e->shootAtPlayer(m_player->getX(), m_player->getY());
            } else {
               // Melee collision logic (FIXED)
               int dx = std::abs(e->getX() - m_player->getX());
               int dy = std::abs(e->getY() - m_player->getY());
               // Если враг рядом (включая диагонали)
               if(dx <= 1 && dy <= 1) {
                   // Проверяем кулдаун атаки (чтобы не убивал за секунду)
                   if (e->m_attackTimer >= 1.0f) {
                        m_player->getDamaged(e->getDamage());
                        e->m_attackTimer = 0; // Сброс таймера
                        addVisualEvent(VisualEvent::BLOOD, m_player->getX(), m_player->getY(), 0, 0, sf::Color::Red);
                   }
               }
            }
        }
    }
    
    auto it = std::remove_if(m_enemies.begin(), m_enemies.end(), [this](Enemy* e) {
        if (!e->isAlive()) { m_player->addRandomSpell(); m_player->addScore(100); m_player->addKill(); delete e; return true; } return false;
    }); m_enemies.erase(it, m_enemies.end());

    for (auto* t : m_traps) {
        if (t->isActive()) { t->update(dt); for (auto* e : m_enemies) { if (e->isAlive() && e->getX() == t->getX() && e->getY() == t->getY()) { e->getDamaged(t->getDamage()); t->trigger(); addVisualEvent(VisualEvent::EXPLOSION, t->getX(), t->getY(), t->getX(), t->getY(), sf::Color::Red); } } }
    }
    for(auto* t : m_thornsAreas) t->update(this); for(auto* s : m_shields) s->update();
    auto s_it = std::remove_if(m_shields.begin(), m_shields.end(), [](Shield* s){ if(!s->isActive()) { delete s; return true; } return false; }); m_shields.erase(s_it, m_shields.end());

    for(auto* b : m_player->getBullets()) {
        if(!b->isActive()) continue;
        if (!m_field->isInBounds(b->getX(), b->getY())) { b->deactivate(); continue; }
        for(auto* e : m_enemies) {
            if (!e->isAlive()) continue; 
            int sz = e->getSize();
            if (b->getX() >= e->getX() && b->getX() < e->getX() + sz && b->getY() >= e->getY() && b->getY() < e->getY() + sz) { 
                e->getDamaged(b->getDamage()); b->deactivate(); addVisualEvent(VisualEvent::BLOOD, b->getX(), b->getY(), 0, 0, sf::Color::Red); break; 
            }
        }
        if(b->isActive() && m_field->getCell(b->getX(), b->getY()).getType() == Cell::WALL) b->deactivate();
    }
    
    for(auto* e : m_enemies) {
        for(auto* b : e->getBullets()) {
            if(!b->isActive()) continue;
            if(!m_field->isInBounds(b->getX(), b->getY())) { b->deactivate(); continue; }
            if(m_field->getCell(b->getX(), b->getY()).getType() == Cell::OBSTACLE) {
                for(auto* s : m_shields) { if(s->contains(b->getX(), b->getY())) { b->deactivate(); VisualEffects::printColored("Blocked by Shield!", sf::Color::Cyan); addVisualEvent(VisualEvent::NOVA, b->getX(), b->getY(), 0, 0, sf::Color::Cyan); break; } }
            }
            if(!b->isActive()) continue;
            if(b->getX() == m_player->getX() && b->getY() == m_player->getY()) { m_player->getDamaged(b->getDamage()); b->deactivate(); addVisualEvent(VisualEvent::BLOOD, m_player->getX(), m_player->getY(), 0, 0, sf::Color::Red); continue; }
            for(auto* ally : m_allies) { if (ally->isAlive() && b->getX() == ally->getX() && b->getY() == ally->getY()) { ally->getDamaged(b->getDamage()); b->deactivate(); break; } }
        }
    }
}

void Game::enterHub() {
    m_gameState = State::HUB;
    m_player->heal(m_player->getMaxHealth());
    
    int spellsToRemove = m_player->getHand().getSize() / 2;
    for(int i=0; i<spellsToRemove; ++i) m_player->getHand().removeRandomSpell();
    if (spellsToRemove > 0) VisualEffects::printColored("The portal drained " + std::to_string(spellsToRemove) + " spells!", sf::Color::Red);

    LevelConfig hubConfig;
    hubConfig.name = "WIZARD'S SANCTUM";
    hubConfig.floorTexture = "floor_stone"; 
    hubConfig.fieldWidth = 22; hubConfig.fieldHeight = 16;
    hubConfig.playerStartX = 11; hubConfig.playerStartY = 13;
    hubConfig.initialEnemyCount = 0; 
    
    cleanup(true); 
    initializeFromConfig(hubConfig, true);
    
    for(int y=3; y<13; ++y) {
        m_worldObjects.push_back(new WorldObject(10, y, WorldObject::RUG, sf::Color(100, 0, 100, 150)));
        m_worldObjects.push_back(new WorldObject(11, y, WorldObject::RUG, sf::Color(100, 0, 100, 150)));
    }
    m_worldObjects.push_back(new WorldObject(10, 2, WorldObject::RUNE, sf::Color::Cyan));
    m_portal = new Portal(10, 2);
    m_shop = new Shop(4, 7);
    addVisualEvent(VisualEvent::FIRE_STORM, 4, 4, 0, 0, sf::Color::Yellow);
    addVisualEvent(VisualEvent::FIRE_STORM, 17, 4, 0, 0, sf::Color::Yellow);
    VisualEffects::printColored("Welcome Home!", sf::Color::Cyan);
}

void Game::handlePauseMenu(sf::Keyboard::Key key) {
    // DEPRECATED: Logic moved to executeCommand via script patch
}

void Game::togglePause() { m_isPaused = !m_isPaused; if(m_isPaused) VisualEffects::printColored("PAUSED. [F5] Save, [F9] Load, [Q] Quit, [P] Resume", sf::Color::Yellow); }

bool Game::teleportPlayer(int distance) {
    int px = m_player->getX(); int py = m_player->getY(); int dx = 0, dy = 0;
    switch(m_lastDirection) { case Bullet::Direction::UP: dy = -distance; break; case Bullet::Direction::DOWN: dy = distance; break; case Bullet::Direction::LEFT: dx = -distance; break; case Bullet::Direction::RIGHT: dx = distance; break; }
    int tx = px + dx; int ty = py + dy;
    if (tx < 1) tx = 1; if (tx >= m_field->getWidth()-1) tx = m_field->getWidth()-2; if (ty < 1) ty = 1; if (ty >= m_field->getHeight()-1) ty = m_field->getHeight()-2;
    if (isValidMove(tx, ty)) { m_player->teleport(tx, ty); addVisualEvent(VisualEvent::NOVA, tx, ty, 0, 0, sf::Color::Cyan); return true; }
    VisualEffects::printColored("Cannot teleport there!", sf::Color::Red); return false;
}

void Game::castNova(int damage, int radius) {
    int px = m_player->getX(); int py = m_player->getY(); addVisualEvent(VisualEvent::EXPLOSION, px, py, 0, 0, sf::Color::Yellow);
    for (auto* e : m_enemies) { if (!e->isAlive()) continue; int dx = e->getX() - px; int dy = e->getY() - py; if (std::sqrt(dx*dx + dy*dy) <= radius) { e->getDamaged(damage); addVisualEvent(VisualEvent::LIGHTNING, px, py, e->getX(), e->getY(), sf::Color::Yellow); } }
}

void Game::castLifeDrain(Character* target, int damage) {
    if (!target) return; target->getDamaged(damage); m_player->heal(damage); addVisualEvent(VisualEvent::BEAM, m_player->getX(), m_player->getY(), target->getX(), target->getY(), sf::Color::Magenta); VisualEffects::printColored("Life Drained!", sf::Color::Magenta);
}

void Game::createShield(int x, int y, int duration, int radius) { m_shields.push_back(new Shield(x, y, duration, radius, m_field)); VisualEffects::printColored("Shield created!", sf::Color::Cyan); addVisualEvent(VisualEvent::NOVA, x, y, x, y, sf::Color::Cyan); }
void Game::createThornsArea(int x, int y, int radius, int duration, int dps) { m_thornsAreas.push_back(new ThornsArea(x, y, radius, duration, dps)); VisualEffects::printColored("Thorns created!", sf::Color::Green); }
void Game::summonAlly(int x, int y) { if(isValidMove(x+1, y)) { auto* ally = new Ally(x+1, y, "Ally", 100, 10); ally->setGameField(m_field); m_allies.push_back(ally); VisualEffects::printColored("Ally summoned!", sf::Color::Yellow); } }
void Game::createTrap(int x, int y, int damage, int duration) { if(isValidMove(x, y)) { m_traps.push_back(new Trap(x, y, damage, duration)); VisualEffects::printColored("Trap placed!", sf::Color::Magenta); } }

void Game::loadNextLevel() {
    int nextIdx = m_currentLevelIndex + 1;
    if (nextIdx > 10) { m_gameState = State::GAME_COMPLETED; return; }
    GameLevel::LevelType type = GameLevel::LevelType::FOREST;
    if (nextIdx >= 4) type = GameLevel::LevelType::DUNGEON;
    if (nextIdx >= 7) type = GameLevel::LevelType::CASTLE;
    if (nextIdx == 10) type = GameLevel::LevelType::BOSS_ARENA;
    GameLevel nextLevel(type, nextIdx);
    cleanup(true); m_currentLevelIndex = nextIdx; m_gameState = State::PLAYING;
    initializeFromConfig(nextLevel.getConfig(), true);
    VisualEffects::printColored("Level " + std::to_string(nextIdx) + " Started!", sf::Color::Cyan);
}

void Game::restartLevel() {
    GameLevel::LevelType type = GameLevel::LevelType::FOREST;
    if (m_currentLevelIndex >= 4) type = GameLevel::LevelType::DUNGEON;
    if (m_currentLevelIndex >= 7) type = GameLevel::LevelType::CASTLE;
    if (m_currentLevelIndex == 10) type = GameLevel::LevelType::BOSS_ARENA;
    GameLevel level(type, m_currentLevelIndex);
    cleanup(true); m_player->heal(m_player->getMaxHealth()); m_gameState = State::PLAYING;
    initializeFromConfig(level.getConfig(), true);
}

void Game::addVisualEvent(VisualEvent::Type type, float sx, float sy, float ex, float ey, sf::Color color) {
    VisualEvent ev; ev.type = type; ev.startX = sx; ev.startY = sy; ev.endX = ex; ev.endY = ey; ev.color = color; ev.maxLifetime = 0.5f; ev.lifetime = ev.maxLifetime; m_visualEvents.push_back(ev);
}



void Game::handleNormalMode(sf::Keyboard::Key key) {
    if ((key == sf::Keyboard::Key::W || key == sf::Keyboard::Key::S || key == sf::Keyboard::Key::A || key == sf::Keyboard::Key::D) && m_playerMoveCooldown > 0) return; 
    int dx=0, dy=0;
    switch(key) {
        case sf::Keyboard::Key::W: dy=-1; m_lastDirection=Bullet::Direction::UP; break;
        case sf::Keyboard::Key::S: dy=1; m_lastDirection=Bullet::Direction::DOWN; break;
        case sf::Keyboard::Key::A: dx=-1; m_lastDirection=Bullet::Direction::LEFT; break;
        case sf::Keyboard::Key::D: dx=1; m_lastDirection=Bullet::Direction::RIGHT; break;
        case sf::Keyboard::Key::E: 
            if (m_gameState == State::HUB) {
                if (m_portal && m_player->getX() == m_portal->getX() && m_player->getY() == m_portal->getY()) { loadNextLevel(); return; }
                if (m_shop) {
                    int dist = std::abs(m_player->getX() - m_shop->getX()) + std::abs(m_player->getY() - m_shop->getY());
                    if (dist <= 1) { m_isShopOpen = true; VisualEffects::printColored("Welcome to the Shop!", sf::Color::Yellow); return; }
                }
            }
            m_player->switchAttackMode(); break;
        case sf::Keyboard::Key::Space: 
            if (m_gameState == State::HUB) return;
            if (m_player->isRangedMode()) m_player->attack(m_lastDirection);
            else {
                int atkX = m_player->getX(); int atkY = m_player->getY();
                if (m_lastDirection == Bullet::Direction::UP) atkY--;
                else if (m_lastDirection == Bullet::Direction::DOWN) atkY++;
                else if (m_lastDirection == Bullet::Direction::LEFT) atkX--;
                else if (m_lastDirection == Bullet::Direction::RIGHT) atkX++;
                addVisualEvent(VisualEvent::SLASH, m_player->getX(), m_player->getY(), atkX, atkY, sf::Color::White);
                for(auto* e : m_enemies) {
                    int sz = e->getSize();
                    bool overlap = (m_player->getX() >= e->getX() && m_player->getX() < e->getX() + sz && m_player->getY() >= e->getY() && m_player->getY() < e->getY() + sz);
                    bool aimedHit = (atkX >= e->getX() && atkX < e->getX() + sz && atkY >= e->getY() && atkY < e->getY() + sz);
                    if (e->isAlive() && (overlap || aimedHit)) {
                        e->getDamaged(m_player->getMeleeDamage());
                        addVisualEvent(VisualEvent::BLOOD, e->getX(), e->getY(), 0, 0, sf::Color::Red);
                    }
                }
            }
            break;
        case sf::Keyboard::Key::Num1: 
            if (m_gameState == State::HUB) return;
            if(m_player->getHand().getSize() > 0) {
                 const Spell* spell = m_player->getHand().getSpell(m_player->getSelectedSpellIndex());
                 (void)spell;
                 if (spell->getTargetType() == SpellTarget::SELF || spell->getTargetType() == SpellTarget::NO_TARGET) spell->cast(m_player, m_player, this);
                 else { m_player->startTargeting(); m_lockedTarget = nullptr; m_targetCursorX = m_player->getX(); m_targetCursorY = m_player->getY(); }
            }
            break;
        case sf::Keyboard::Key::Num2: m_player->nextSpell(); break;
        case sf::Keyboard::Key::Num3: m_player->prevSpell(); break;
        default: break;
    }
    if(dx!=0 || dy!=0) handlePlayerMovement(m_player->getX()+dx, m_player->getY()+dy);
}
void Game::handleShopMode(sf::Keyboard::Key key) {
    if (!m_shop) return;
    switch(key) {
        case sf::Keyboard::Key::Escape: m_isShopOpen = false; break;
        case sf::Keyboard::Key::Num1: m_shop->tryBuySpell(m_player); break;
        case sf::Keyboard::Key::Num2: m_shop->tryUpgradeSpell(m_player); break;
        default: break;
    }
}
void Game::handleTargetingMode(sf::Keyboard::Key key) {
    if(key == sf::Keyboard::Key::W || key == sf::Keyboard::Key::S || key == sf::Keyboard::Key::A || key == sf::Keyboard::Key::D) m_lockedTarget = nullptr;
    switch(key) {
        case sf::Keyboard::Key::W: m_targetCursorY--; break;
        case sf::Keyboard::Key::S: m_targetCursorY++; break;
        case sf::Keyboard::Key::A: m_targetCursorX--; break;
        case sf::Keyboard::Key::D: m_targetCursorX++; break;
        case sf::Keyboard::Key::Space: 
        case sf::Keyboard::Key::Enter: {
            const Spell* spell = m_player->getHand().getSpell(m_player->getSelectedSpellIndex());
                 (void)spell;
            Character* target = nullptr;
            for(auto* e : m_enemies) {
                int sz = e->getSize();
                if(e->isAlive() && m_targetCursorX >= e->getX() && m_targetCursorX < e->getX() + sz && m_targetCursorY >= e->getY() && m_targetCursorY < e->getY() + sz) { target = e; break; }
            }
            if(!target && spell->getTargetType() == SpellTarget::ENEMY) VisualEffects::printColored("No target!", sf::Color::Red);
            else { spell->cast(m_player, target, this); m_player->cancelTargeting(); m_lockedTarget = nullptr; }
            break;
        }
        case sf::Keyboard::Key::Escape: m_player->cancelTargeting(); m_lockedTarget = nullptr; break;
        default: break;
    }
}
void Game::handlePlayerMovement(int x, int y) {
    if(isValidMove(x, y)) {
        m_player->setPosition(x, y);
        for(auto* o : m_worldObjects) { if(o->getType() == WorldObject::WATER && o->getX() == x && o->getY() == y) m_playerMoveCooldown = 0.18f; }
    }
}
bool Game::isValidMove(int x, int y) const {
    if(!m_field->isInBounds(x, y)) return false;
    for(auto* t : m_towers) { for(auto& p : t->getBlocks()) { if (p.first == x && p.second == y) return false; } }
    for(auto* o : m_worldObjects) { if (o->isSolid() && o->getX() == x && o->getY() == y) return false; }
    return m_field->getCell(x, y).getType() == Cell::EMPTY;
}

void Game::executeCommand(const GameCommand& cmd) {
    if (cmd.type == CommandType::Exit) m_isRunning = false;
    
    if (m_isPaused) {
        if (cmd.type == CommandType::Pause) togglePause();
        if (cmd.type == CommandType::Quit) m_isRunning = false;
        
        // LOGIC PATCHED BY SCRIPT
        if (cmd.type == CommandType::Save) {
            if (m_gameMode == GameMode::STORY) {
                try {
                    SaveManager::saveGame(*this);
                    VisualEffects::printColored("GAME SAVED!", sf::Color::Green);
                    m_isPaused = false; 
                } catch (const GameException& e) {
                    VisualEffects::printColored(e.what(), sf::Color::Red);
                }
            } else VisualEffects::printColored("Can't save in Free Play!", sf::Color::Red);
        }
        if (cmd.type == CommandType::Load) {
            if (m_gameMode == GameMode::STORY) {
                try {
                    SaveManager::loadGame(*this);
                    VisualEffects::printColored("GAME LOADED!", sf::Color::Green);
                    m_isPaused = false;
                } catch (const GameException& e) {
                    VisualEffects::printColored(e.what(), sf::Color::Red);
                }
            }
        }
        return;
    }

    if (cmd.type == CommandType::Pause) { togglePause(); return; }

    if (cmd.type == CommandType::Action || cmd.type == CommandType::TargetSelect) {
        if (m_gameState == State::VICTORY) {
            if (m_gameMode == GameMode::STORY) enterHub();
            else loadNextLevel();
            return;
        }
        if (m_gameState == State::GAME_OVER) {
            restartLevel();
            return;
        }
    }

    if (m_gameState == State::PLAYING || m_gameState == State::HUB) {
        // Движение (WASD + Диагонали)
        if (cmd.type == CommandType::Move || 
            cmd.type == CommandType::MoveUp || cmd.type == CommandType::MoveDown || 
            cmd.type == CommandType::MoveLeft || cmd.type == CommandType::MoveRight) {
            
            if (m_playerMoveCooldown > 0) return;

            int dx = 0, dy = 0;

            if (cmd.type == CommandType::Move) {
                dx = cmd.dataX;
                dy = cmd.dataY;
            } else {
                if (cmd.type == CommandType::MoveUp) dy = -1;
                if (cmd.type == CommandType::MoveDown) dy = 1;
                if (cmd.type == CommandType::MoveLeft) dx = -1;
                if (cmd.type == CommandType::MoveRight) dx = 1;
            }

            if (dy < 0) m_lastDirection = Bullet::Direction::UP;
            if (dy > 0) m_lastDirection = Bullet::Direction::DOWN;
            if (dx < 0) m_lastDirection = Bullet::Direction::LEFT;
            if (dx > 0) m_lastDirection = Bullet::Direction::RIGHT;
            
            if (dx != 0 || dy != 0) {
                if (m_player->isTargeting()) {
                     m_targetCursorX += dx; m_targetCursorY += dy;
                     m_playerMoveCooldown = 0.15f;
                } else {
                     handlePlayerMovement(m_player->getX() + dx, m_player->getY() + dy);
                     // FIX: Скорость 0.14f
                     if (m_playerMoveCooldown <= 0) m_playerMoveCooldown = 0.15f; 
                }
            }
        }

        if (cmd.type == CommandType::Attack) {
             if (m_player->isTargeting()) {
                 const Spell* spell = m_player->getHand().getSpell(m_player->getSelectedSpellIndex());
                 (void)spell;
                  Character* target = nullptr;
                  for(auto* e : m_enemies) {
                      int sz = e->getSize();
                      if(e->isAlive() && m_targetCursorX >= e->getX() && m_targetCursorX < e->getX() + sz && m_targetCursorY >= e->getY() && m_targetCursorY < e->getY() + sz) { 
                          target = e; break; 
                      }
                  }
                  if(!target && spell->getTargetType() == SpellTarget::ENEMY) VisualEffects::printColored("No target!", sf::Color::Red);
                  else { spell->cast(m_player, target, this); m_player->cancelTargeting(); m_lockedTarget = nullptr; }
             } else {
                 if (m_gameState == State::HUB) return; 
                 if (m_player->isRangedMode()) m_player->attack(m_lastDirection);
                 else {
                     int atkX = m_player->getX(); int atkY = m_player->getY();
                     if (m_lastDirection == Bullet::Direction::UP) atkY--;
                     else if (m_lastDirection == Bullet::Direction::DOWN) atkY++;
                     else if (m_lastDirection == Bullet::Direction::LEFT) atkX--;
                     else if (m_lastDirection == Bullet::Direction::RIGHT) atkX++;
                     addVisualEvent(VisualEvent::SLASH, m_player->getX(), m_player->getY(), atkX, atkY, sf::Color::White);
                     
                     for(auto* e : m_enemies) {
                        int sz = e->getSize();
                        bool overlap = (m_player->getX() >= e->getX() && m_player->getX() < e->getX() + sz && m_player->getY() >= e->getY() && m_player->getY() < e->getY() + sz);
                        bool aimedHit = (atkX >= e->getX() && atkX < e->getX() + sz && atkY >= e->getY() && atkY < e->getY() + sz);
                        if (e->isAlive() && (overlap || aimedHit)) {
                            e->getDamaged(m_player->getMeleeDamage());
                            addVisualEvent(VisualEvent::BLOOD, e->getX(), e->getY(), 0, 0, sf::Color::Red);
                        }
                    }
                 }
             }
        }
        
        if (cmd.type == CommandType::SwitchMode) {
             if (m_gameState == State::HUB) {
                 if (m_portal && m_player->getX() == m_portal->getX() && m_player->getY() == m_portal->getY()) { loadNextLevel(); return; }
                 if (m_shop) { 
                    int dist = std::abs(m_player->getX() - m_shop->getX()) + std::abs(m_player->getY() - m_shop->getY());
                    if (dist <= 1) { m_isShopOpen = true; VisualEffects::printColored("Welcome to the Shop!", sf::Color::Yellow); return; }
                 }
             }
             m_player->switchAttackMode();
        }

        if (cmd.type == CommandType::CastSpell) {
             if (m_player->getHand().getSize() > 0) {
                 const Spell* spell = m_player->getHand().getSpell(m_player->getSelectedSpellIndex());
                 if (spell->getTargetType() == SpellTarget::SELF || spell->getTargetType() == SpellTarget::NO_TARGET) 
                    spell->cast(m_player, m_player, this);
                 else { 
                    m_player->startTargeting(); m_targetCursorX = m_player->getX(); m_targetCursorY = m_player->getY(); 
                 }
             }
        }
        
        if (cmd.type == CommandType::NextSpell) m_player->nextSpell();
        if (cmd.type == CommandType::PrevSpell) m_player->prevSpell();
    }
}