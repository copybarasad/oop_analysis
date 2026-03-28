#include "Map.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <algorithm>

Map::Map(Vec2 size)
{
    setSize(size);
}

Map::Map(const Map &map)
{
    if (this == &map)
        return;

    mObjects.clear();
    mObjects.resize(map.getSize().y, vector<Cell>(map.getSize().x));
    for (size_t y = 0; y < mObjects.size(); y++)
    {
        for (size_t x = 0; x < mObjects[y].size(); x++)
        {
            Cell buff;
            buff.type = map.mObjects[y][x].type;
            if (map.mObjects[y][x].ent)
            {
                buff.ent = make_shared<Entity>(*map.mObjects[y][x].ent);
            }
            mObjects[y][x] = buff;
        }
    }
}

Map::Map(Map &&map)
{
    if (this == &map)
        return;
    mObjects.clear();
    mObjects.resize(map.getSize().y, vector<Cell>(map.getSize().x));
    for (size_t y = 0; y < mObjects.size(); y++)
    {
        for (size_t x = 0; x < mObjects[y].size(); x++)
        {
            mObjects[y][x] = map.mObjects[y][x];
            map.mObjects[y][x].ent.reset();
        }
    }
}

void Map::setSize(Vec2 size)
{
    if (size.x < 10 || size.x > 25 || size.y < 10 || size.y > 25)
        throw invalid_argument("Wrog map size");
    mObjects.clear();

    std::random_device rseed;
    std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> r(0, 10);
    // srand(0);
    mObjects.resize(size.y, vector<Cell>(size.x));
    for (size_t y = 0; y < mObjects.size(); y++)
    {
        for (size_t x = 0; x < mObjects[y].size(); x++)
        {
            // int res = rand() % 10;
            int res = r(rng);
            if (res < 1)
                mObjects[y][x].type = CellType::Block;
            else if (res < 5)
                mObjects[y][x].type = CellType::Slow;
            else
                mObjects[y][x].type = CellType::Simple;
        }
    }
    mObjects[0][0].type = CellType::Simple;
}

Vec2 Map::getSize() const
{
    return Vec2(mObjects[0].size(), mObjects.size());
}

void Map::putCharacter(shared_ptr<Entity> ch)
{
    mObjects[ch->getPosition().y][ch->getPosition().x].ent = ch;
}

Map &Map::operator=(const Map &map)
{
    if (this == &map)
        return *this;

    mObjects.clear();
    mObjects.resize(map.getSize().y, vector<Cell>(map.getSize().x));
    for (size_t y = 0; y < mObjects.size(); y++)
    {
        for (size_t x = 0; x < mObjects[y].size(); x++)
        {
            Cell buff;
            buff.type = map.mObjects[y][x].type;
            if (map.mObjects[y][x].ent)
            {
                buff.ent = make_shared<Entity>(*map.mObjects[y][x].ent);
            }
            mObjects[y][x] = buff;
        }
    }
    return *this;
}

Map &Map::operator=(Map &&m)
{
    if (this == &m)
        return *this;

    mObjects.clear();
    mObjects.resize(m.getSize().y, vector<Cell>(m.getSize().x));
    for (size_t y = 0; y < mObjects.size(); y++)
    {
        for (size_t x = 0; x < mObjects[y].size(); x++)
        {
            mObjects[y][x] = m.mObjects[y][x];
            m.mObjects[y][x].ent.reset();
        }
    }
    return *this;
}

void Map::moveCharacterTo(shared_ptr<Character> ch, Vec2 dest)
{
    // Vec2 from(ch->getPosition().x, ch->getPosition().y);

    if (dest.x < 0 || dest.y < 0 || dest.x >= getSize().x || dest.y >= getSize().y || ch.get()->getPosition() == dest)
        return;

    if (mObjects[dest.y][dest.x].ent.get() != nullptr)
    {
        if (!mObjects[dest.y][dest.x].ent.get()->isDead() && ch.get()->getHitType() == HitType::CloseCombat)
        {
            mObjects[dest.y][dest.x].ent.get()->dealDamage(ch.get()->getMeleeDamage());
            if (mObjects[dest.y][dest.x].ent.get()->isDead())
            {
                ch.get()->addExp(mObjects[dest.y][dest.x].ent->getExpForKill());
                mObjects[dest.y][dest.x].ent.reset();
            }
        }
    }
    if ((mObjects[dest.y][dest.x].ent.get() == nullptr || mObjects[dest.y][dest.x].ent.get()->isDead()) && mObjects[dest.y][dest.x].type != CellType::Block)
    {
        if (mObjects[dest.y][dest.x].type == CellType::Slow)
            ch->setSlowDown(true);
        mObjects[dest.y][dest.x].ent = ch;
        mObjects[ch.get()->getPosition().y][ch.get()->getPosition().x].ent.reset();
        ch->setPosition(dest);
        // switch (mObjects[dest.y][dest.x].type)
        // {
        // case CellType::Slow:
        //     ch->setSlowDown(true);
        //     mObjects[dest.y][dest.x].ent = ch;
        //     mObjects[from.y][from.x].ent = nullptr;
        //     ch->setPosition(dest);

        //     break;
        // case CellType::Simple:
        //     mObjects[dest.y][dest.x].ent = ch;
        //     mObjects[from.y][from.x].ent = nullptr;
        //     ch->setPosition(dest);
        //     break;

        // default:
        //     break;
        // };
    }
}

Map::~Map()
{
    // for (size_t i = 0; i < mObjects.size(); i++)
    // {
    //     for (size_t j = 0; j < mObjects[i].size(); j++)
    //     {
    //         if (mObjects[i][j].ent != nullptr)
    //         {
    //             delete mObjects[i][j].ent;
    //         }
    //     }
    // }
    mObjects.clear();
}
