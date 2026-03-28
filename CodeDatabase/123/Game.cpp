#include "Game.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <stdexcept>

#define HELP_TEXT "0 - Surrender; 1 - Show player info; 2 - Wait; 3 - Move; 4 - Hit; 5 - Switch hit"

// #define CELL_TEXTURE_PATH "Textures/Cell.bmp"
// #define CHARACTER_TEXTURE_PATH "Textures/Characters.png"

Game::Game()
{
    mPlayer = make_shared<Player>();
    mBuilding = make_shared<Building>((int)10, (1));
    mMap = make_shared<Map>();
    mLevel = make_shared<size_t>();
    mEnemies = make_shared<vector<shared_ptr<Enemy>>>();
}

void Game::randomEnemyAction(size_t i)
{
    enum class Act
    {
        Wait,
        Move,
        Hit,
        SwitchHit
    };

    std::random_device rseed;
    std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> dist(0, 3);

    int act = dist(rng);
    Vec2 pos(1, 1);
    int angle = 0;
    Entity *buff;
    std::uniform_int_distribution<int> d(0, 2);

    switch ((Act)act)
    {
    case Act::Move:
        mMap->moveCharacterTo(mEnemies->at(i), mEnemies->at(i)->getPosition() + getRandomDirection(true));
        break;

    case Act::Hit:
        if (mEnemies->at(i)->getHitType() == HitType::CloseCombat)
        {
            mMap->moveCharacterTo(mEnemies->at(i), mEnemies->at(i)->getPosition() + getRandomDirection(true));
        }
        else
        {
            std::uniform_int_distribution<int> x(0, mMap->getSize().x - 1);
            std::uniform_int_distribution<int> y(0, mMap->getSize().y - 1);
            pos = Vec2(x(rng), y(rng));
            if (mMap->operator[](pos.y)[pos.x].ent)
                mMap->operator[](pos.y)[pos.x].ent.get()->dealDamage(mEnemies->at(i)->getLongRangeCombatDamage());
        }
        break;

    case Act::SwitchHit:
        mEnemies->at(i)->setHitType((HitType)(d(rng) % 2));
        break;

    default:
        break;
    }
}

bool Game::gameOver() const
{
    return mPlayer->isDead() || (mBuilding.get()->isDead() && all_of(mEnemies->begin(), mEnemies->end(), [](shared_ptr<Enemy> x)
                                                                     { return x.get()->isDead(); }));
}

bool Game::isWin() const
{
    return !mPlayer->isDead();
}

void Game::enemyMove()
{
    for (size_t i = 0; i < mEnemies->size(); i++)
    {
        if (mEnemies->at(i)->isSlowed())
        {
            mEnemies->at(i)->setSlowDown(false);
            continue;
        }
        if (!mEnemies->at(i)->isDead())
            randomEnemyAction(i);
    }
    mBuilding->doMove();
    if (!mBuilding->isDead())
    {
        if (mBuilding->isTimeForSpawn())
        {
            shared_ptr<Enemy> newEnemy = mBuilding->createNewEnemy();
            newEnemy.get()->setPosition(mBuilding->getPosition() + getRandomDirection(false));
            mMap->putCharacter(newEnemy);
            mEnemies->push_back(newEnemy);
            mBuilding->restoreCurentSpawn();
        }
    }
}


Vec2 Game::getSize() const
{
    return mMap->getSize();
    // return Vec2(mMap->getSize().x * CELL_SPRITE_SIZE, mMap->getSize().y * CELL_SPRITE_SIZE);
}

void Game::setNewLevel()
{
    mEnemies->clear();
    mBuilding.reset();

    std::random_device rseed;
    std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> size(10, 25);
    Vec2 mapSize = Vec2(size(rng), size(rng));
    mMap->setSize(mapSize);
    mPlayer.get()->setPosition(Vec2(0, 0));
    if (mPlayer.get()->getHand()->getSize() < 1)
        mPlayer.get()->getHand()->addSpell();
    mPlayer.get()->setLife(mPlayer.get()->getMaxLife());
    mMap->putCharacter(mPlayer);

    std::uniform_int_distribution<int> enemyCap(2, 5);
    for (size_t i = 0; i < enemyCap(rng); i++)
    {
        float coef = 1.5f * (*mLevel.get()) + 1;
        std::uniform_int_distribution<int> life(10 * coef, 10 * (coef + 1));
        std::uniform_int_distribution<int> melee(3 * coef, 3 * (coef + 1));
        std::uniform_int_distribution<int> longRange(2 * coef, 2 * (coef + 1));
        shared_ptr<Enemy> buff = make_shared<Enemy>(life(rng), melee(rng), longRange(rng));
        Vec2 pos(1, 1);
        do
        {
            std::uniform_int_distribution<int> x(0, mapSize.x - 1);
            std::uniform_int_distribution<int> y(0, mapSize.y - 1);
            pos = Vec2(x(rng), y(rng));
        } while (pos == Vec2(0, 0) || mMap->operator[](pos.y)[pos.x].type == CellType::Block);
        buff.get()->setPosition(pos);
        mEnemies->push_back(buff);
        mMap->putCharacter(buff);
    }
    // std::random_device rseed;
    // std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> life(10, 40);

    mBuilding = make_shared<Building>(life(rng), 5);
    // mBuilding = shared_ptr<Building>();
    mBuilding->setPosition(Vec2(mMap->getSize().x / 2 - 1, (int)(mMap->getSize().y / 2 - 1)));
    // mMap[mBuilding.get()->getPosition().y][mBuilding.get()->getPosition().x].type = CellType::Simple;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            Vec2 shift(i, j);
            shift += mBuilding.get()->getPosition();
            mMap->operator[](shift.y)[shift.x].type = CellType::Simple;
        }
    }

    mMap->putCharacter(mBuilding);

    (*mLevel.get())++;
}

void Game::newGame()
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<int> life(30, 50);
    std::uniform_int_distribution<int> mellee(8, 12);
    std::uniform_int_distribution<int> longRange(3, 7);
    *mLevel.get() = 0;

    // mPlayer.reset();
    mPlayer = make_shared<Player>(life(rng), mellee(rng), longRange(rng));
    // mPlayer->setMaxLife(life(rng));
    // mPlayer->setLife(life(rng));
    // mPlayer->setMeleeDamage(mellee(rng));
    // mPlayer->setLongRangeCombatDamage(longRange(rng));
    // mPlayer->setPosition({0, 0});
    setNewLevel();

    // mHand.setHand(mPlayer->getHand());
}

size_t Game::getLevel() const
{
    return *mLevel.get();
}

shared_ptr<Player> Game::getPlayer()
{
    return mPlayer;
}

Map *Game::getMap() const
{
    return mMap.get();
}

Cell Game::getCell(Vec2 pos)
{
    return mMap->operator[](pos.y)[pos.x];
}

void Game::moveCharacterTo(shared_ptr<Character> ch, Vec2 dest)
{
    if (dest.x < 0 || dest.y < 0 || dest.x >= getSize().x || dest.y >= getSize().y || ch.get()->getPosition() == dest)
        return;

    if (mMap->operator[](dest.y)[dest.x].ent.get() != nullptr)
    {
        if (!mMap->operator[](dest.y)[dest.x].ent.get()->isDead() && ch.get()->getHitType() == HitType::CloseCombat)
        {
            mMap->operator[](dest.y)[dest.x].ent.get()->dealDamage(ch.get()->getMeleeDamage());
            if (mMap->operator[](dest.y)[dest.x].ent.get()->isDead())
            {
                ch->addExp(mMap->operator[](dest.y)[dest.x].ent->getExpForKill());
                mMap->operator[](dest.y)[dest.x].ent.reset();
            }
        }
    }
    if ((mMap->operator[](dest.y)[dest.x].ent.get() == nullptr || mMap->operator[](dest.y)[dest.x].ent.get()->isDead()) && mMap->operator[](dest.y)[dest.x].type != CellType::Block)
    {
        if (mMap->operator[](dest.y)[dest.x].type == CellType::Slow)
            ch->setSlowDown(true);
        mMap->operator[](dest.y)[dest.x].ent = ch;
        mMap->operator[](ch->getPosition().y)[ch->getPosition().x].ent.reset();
        ch->setPosition(dest);
    }
}

#include "GameSaveMediator.h"

void Game::setMediator(GameSaveMediator *mediator)
{
    mediator->setGameComponents(&mMap, &mPlayer, mEnemies, &mBuilding, mLevel);
}

Vec2 Game::getRandomDirection(bool isCross) const
{
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right,
        UpRight,
        UpLeft,
        DownRight,
        DownLeft
    };
    int edge = 3;
    if (!isCross)
        edge = 7;

    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<int> dist(0, edge);
    int ans = 0;
    switch ((Direction)dist(rng))
    {
    case Direction::Up:
        ans = 90;
        break;
    case Direction::Down:
        ans = 270;
        break;
    case Direction::Left:
        ans = 180;
        break;
    case Direction::Right:
        ans = 0;
        break;
    case Direction::UpRight:
        ans = 45;
        break;
    case Direction::UpLeft:
        ans = 135;
        break;
    case Direction::DownRight:
        ans = 315;
        break;
    case Direction::DownLeft:
        ans = 225;
        break;
    }
    return Vec2(round(cos(ans)), round(sin(ans)));
}

void Game::putEverythingOnTheMap()
{
    mMap->putCharacter(mPlayer);
    for (size_t i = 0; i < mEnemies->size(); i++)
    {
        mMap->putCharacter(mEnemies->at(i));
    }

    mMap->putCharacter(mBuilding);
    // mHand.setHand(mPlayer.get()->getHand());
}
