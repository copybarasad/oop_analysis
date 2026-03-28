#include "Hand.h"
#include <random>
#include <algorithm>

Hand::Hand(int spellCap)
{
    mMaxSize = spellCap;
    // addSpell();
    mSelection = 0;
}

Hand::Hand(const Hand &h)
{
    if (&h != this)
    {
        mSpellVector.clear();
        for (size_t i = 0; i < h.getSize(); i++)
        {
            mSpellVector.push_back(h.mSpellVector[i]->clone());
        }
    }
}

void Hand::addSpell()
{
    if (mSpellVector.size() < mMaxSize)
    {
        Spell *buff;
        bool it = true;
        do
        {
            it = true;
            std::random_device rseed;
            std::mt19937 rng(rseed());
            std::uniform_int_distribution<int> d(0, SPELL_CAPASITY - 1);
            int type = d(rng);
            if (type == 0)
                buff = new DirectDamageSpell();
            else if (type == 1)
                buff = new SquareDamageSpell();
            for (size_t i = 0; i < mSpellVector.size(); i++)
            {
                it *= (typeid(*mSpellVector[i]) != typeid(*buff));
            }

        } while (!it);

        mSpellVector.push_back(buff);
    }
}

void Hand::addSpecificSpell(Spell *sp)
{
    mSpellVector.push_back(sp);
}

void Hand::deleteSpell(int i)
{
    mSelection = 0;
    mSpellVector.erase(mSpellVector.begin() + i);
}

Spell *Hand::getSpell(int i)
{
    return mSpellVector[i];
}

int Hand::getSize() const
{
    return mSpellVector.size();
}

Hand &Hand::operator=(const Hand &h)
{
    if (this == &h)
        return *this;
    for (auto *spell : mSpellVector)
    {
        delete spell;
    }
    mSpellVector.clear();
    for (size_t i = 0; i < h.getSize(); i++)
    {
        mSpellVector.push_back(h.mSpellVector[i]->clone());
    }
    return *this;
}

Hand &Hand::operator=(Hand &&h)
{
    if (this == &h)
        return *this;
    for (auto *spell : mSpellVector)
    {
        delete spell;
    }
    mSpellVector.clear();
    for (size_t i = 0; i < h.getSize(); i++)
    {
        mSpellVector.push_back(h.mSpellVector[i]->clone());
    }
    return *this;
}

void Hand::setSelection(int sel)
{
    mSelection = sel;
}

int Hand::getSelection() const
{
    return mSelection;
}

Hand::~Hand()
{
    for (size_t i = 0; i < mSpellVector.size(); i++)
    {
        delete mSpellVector[i];
    }
}