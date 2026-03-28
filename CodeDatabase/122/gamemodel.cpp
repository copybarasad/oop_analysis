#include "gamemodel.h"
#include "enemyspellconfig.h"

GameModel::GameModel() : m_board(new GameBoard()), m_building(new DarkBuilding()), m_tower(new DarkTower()) {}

void GameModel::spawnPlayer(int health, int countMana, int xPos, int yPos, int meleeDamage, int rangedDamage)
{
    m_players.push_back(std::make_unique<Player>(health, countMana, xPos, yPos, meleeDamage, rangedDamage));
    m_board->UpdateBoardAfterMove(xPos, yPos);
    EventDispatcher::instance().notify(MoveEvent(Actor::PLAYER, CellType::ORIGINAL, {xPos, yPos}, {xPos, yPos}));
}

void GameModel::spawnBuilding(int xPos, int yPos)
{
    for (int dx = 0; dx < 2; dx++)
    {
        for (int dy = 0; dy < 2; dy++)
        {
            m_board->UpdateBoardAfterBlock(xPos+dx, yPos+dy);
        }
    }
}

void GameModel::spawnEnemy(int health, int xPos, int yPos, int meleeDamage)
{
    m_enemies.push_back(std::make_unique<Enemy>(health, xPos, yPos, meleeDamage));
    m_board->UpdateBoardAfterMove(xPos, yPos);
    EventDispatcher::instance().notify(MoveEvent(Actor::ENEMY, CellType::ORIGINAL, {xPos, yPos}, {xPos, yPos}));
}

void GameModel::spawnArcher(int xPos, int yPos, int health, int meleeDamage)
{
    m_archers.push_back(std::make_unique<Archer>(health, xPos, yPos, meleeDamage));
    m_board->UpdateBoardAfterMove(xPos, yPos);
    EventDispatcher::instance().notify(MoveEvent(Actor::ARCHER, CellType::ORIGINAL, {xPos, yPos}, {xPos, yPos}));
}

bool GameModel::moveCharacter(Entity& entity, GameBoard& board, int dx, int dy)
{
    int oldXPos = entity.getXPos();
    int oldYPos = entity.getYPos();

    int newXPos = oldXPos + dx;
    int newYPos = oldYPos + dy;

    if (board.isValidCell(newXPos, newYPos)) {
        auto movable = dynamic_cast<IMovable*>(&entity);
        if (!movable) return false;
        movable->move(board.currentCell(newXPos, newYPos), dx, dy);
        board.UpdateBoardAfterMove(oldXPos, oldYPos, newXPos, newYPos);
        return true;
    }
    return false;
}

void GameModel::attackEnemies()
{
    for (auto& player : m_players)
    {
        for (auto& enemy : m_enemies)
        {
            int oldHealth = enemy->getHealth();
            int damage = player->attack(*enemy);
            EventDispatcher::instance().notify(AttackEvent(Actor::PLAYER, Actor::ENEMY, oldHealth, damage));
        }
    }
}

void GameModel::updateBuildings()
{
    m_building->buildingUpdate();
    if (m_building->readyToSpawn())
    {
        spawnEnemy(CharacterConfig::Health, m_board->getSize()-1, 2, damageEnemy);
    }

    m_tower->towerUpdate();
    if (!m_tower->isAlive())
    {
        EventDispatcher::instance().notify((DeadEvent(Actor::DARK_TOWER, {m_tower->getXPos(), m_tower->getYPos()})));
    }
    if (m_tower->readyToAttack())
    {
        auto& player = m_players.front();
        int oldHealth = player->getHealth();
        EnemySpellConfig config(this, {player->getXPos(), player->getYPos()});
        m_tower->attackSpell(config);
        int newHealth = player->getHealth();
        EventDispatcher::instance().notify(AttackEvent(Actor::DARK_TOWER, Actor::PLAYER, oldHealth, newHealth - oldHealth));
    }
}

void GameModel::updateGameState()
{
    if (!m_logicActive)
    {
        m_logicActive = true;
        return;
    }

    updateArchers();
    updateEnemies();
    updateBuildings();

    for (auto it = m_enemies.begin(); it != m_enemies.end();) {
        if (!(*it)->isAlive()) {
            EventDispatcher::instance().notify(DeadEvent(Actor::ENEMY, {(*it)->getXPos(), (*it)->getYPos()}));
            m_board->currentCell((*it)->getXPos(), (*it)->getYPos()).free();
            it = m_enemies.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = m_players.begin(); it != m_players.end();) {
        if (!(*it)->isAlive()) {
            EventDispatcher::instance().notify(DeadEvent(Actor::PLAYER, {(*it)->getXPos(), (*it)->getYPos()}));
            m_board->currentCell((*it)->getXPos(), (*it)->getYPos()).free();
            it = m_players.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = m_archers.begin(); it != m_archers.end();) {
        if (!(*it)->isAlive()) {
            EventDispatcher::instance().notify(DeadEvent(Actor::ARCHER, {(*it)->getXPos(), (*it)->getYPos()}));
            m_board->currentCell((*it)->getXPos(), (*it)->getYPos()).free();
            it = m_archers.erase(it);
        } else {
            ++it;
        }
    }

    m_board->removeDestroyedObservers();
}

bool GameModel::isLevelComplete()
{
    if (!m_tower && m_currentLevel == 1) return false;
    return ((!m_tower->isAlive()) && m_enemies.empty());
}

bool GameModel::isLevelFailed()
{
    return m_inGame && (m_players.empty());
}

void GameModel::loadLevel(int healthPlayer, int countMana)
{
    m_players.clear();
    m_enemies.clear();
    m_archers.clear();

    int boardSize = BoardConfig::Size + m_currentLevel * 5;
    damageEnemy = CharacterConfig::MeleeDamage + m_currentLevel * 5;

    EventDispatcher::instance().notify(LevelLoadedEvent(boardSize));

    spawnPlayer(healthPlayer, countMana, 0, boardSize - 1);
    m_players.front()->getHands().deleteRandomCards();

    m_board = std::make_unique<GameBoard>(boardSize);
    m_board->generateCells();
    m_board->registerDefaultObservers(m_players);
    m_building = std::make_unique<DarkBuilding>();
    m_tower = std::make_unique<DarkTower>();
    spawnBuilding(boardSize - 2, 0);
    spawnBuilding(0, 0);

    m_inGame = true;
}

void GameModel::newLevel(int healthPlayer, int countMana)
{
    m_currentLevel += 1;
    if (m_currentLevel == 4)
    {
        m_currentLevel = 0;
        m_inGame = false;
        EventDispatcher::instance().notify(WinEvent(m_players.front()->getHands().getMana()));
        return;
    }
    loadLevel(healthPlayer, countMana);
}

void GameModel::updateEnemies()
{
    if (m_enemies.empty()) return;

    for (auto& enemy : m_enemies)
    {
        if (enemy->getEffect() == Enemy::DELAYED)
        {
            enemy->setEffect(Enemy::NO_EFFECT);
            continue;
        }
        int xPosPlayer = m_players.front()->getXPos();
        int yPosPlayer = m_players.front()->getYPos();
        int xPosEnemy = enemy->getXPos();
        int yPosEnemy = enemy->getYPos();
        std::pair<int, int> result = enemy->calculateMovement(xPosEnemy, yPosEnemy, xPosPlayer, yPosPlayer);
        if (moveCharacter(*enemy, *m_board, result.first, result.second))
        {
            EventDispatcher::instance().notify(MoveEvent(Actor::ENEMY, m_board->currentCell(xPosEnemy, yPosEnemy).getType(), {xPosEnemy, yPosEnemy}, {xPosEnemy + result.first, yPosEnemy + result.second}));
        }
    }

    for (auto& enemy : m_enemies)
    {
        for (auto& player : m_players)
        {
            int oldHealth = player->getHealth();
            int damage = enemy->attack(*player);
            EventDispatcher::instance().notify(AttackEvent(Actor::ENEMY, Actor::PLAYER, oldHealth, damage));
        }
    }

    if (m_archers.empty()) return;

    for (auto& enemy : m_enemies)
    {
        for (auto& archer : m_archers)
        {
            int oldHealth = archer->getHealth();
            int damage = enemy->attack(*archer);
            EventDispatcher::instance().notify(AttackEvent(Actor::ENEMY, Actor::ARCHER, oldHealth, damage));
        }
    }
}

void GameModel::updateArchers()
{
    if (m_archers.empty() || m_enemies.empty()) return;

    for (auto& archer : m_archers)
    {
        if (archer->getEffect() == Archer::DELAYED)
        {
            archer->setEffect(Archer::NO_EFFECT);
            continue;
        }
        int xPosEnemy = m_enemies.front()->getXPos();
        int yPosEnemy = m_enemies.front()->getYPos();
        int xPosArcher = archer->getXPos();
        int yPosArcher = archer->getYPos();
        std::pair<int, int> result = archer->calculateMovement(xPosArcher, yPosArcher, xPosEnemy, yPosEnemy);
        if (moveCharacter(*archer, *m_board, result.first, result.second))
        {
            EventDispatcher::instance().notify(MoveEvent(Actor::ARCHER, m_board->currentCell(xPosArcher, yPosArcher).getType(), {xPosArcher, yPosArcher}, {xPosArcher + result.first, yPosArcher + result.second}));
        }
    }

    for (auto& archer : m_archers)
    {
        for (auto& enemy : m_enemies)
        {
            int oldHealth = enemy->getHealth();
            int damage = archer->attack(*enemy);
            EventDispatcher::instance().notify(AttackEvent(Actor::ARCHER, Actor::ENEMY, oldHealth, damage));
        }
    }
}
