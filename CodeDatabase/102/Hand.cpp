#include <iostream>
#include "../../Headers/Containers/Hand.h"
#include <sstream>

Hand::HandData::HandData(const Hand* hand)
{
    auto iter = hand->_spells.begin();
    auto end = hand->_spells.end();
    while (iter != end)
    {
        _spells.push_back(iter->first);
        ++iter;
    }
}

Hand::HandData::HandData(std::string& s)
{
    std::istringstream stream{ s };
    std::string line;
    while (line != "HAND_S" && std::getline(stream, line))
    {
    }
    while (std::getline(stream, line) && line != "HAND_E")
        _spells.push_back(line);
}

std::string Hand::HandData::ToString() const
{
    std::string s = "HAND_S\n";
    for (const std::string& _spell : _spells)
        s += _spell + '\n';
    s += "HAND_E";
    return s;
}

Hand::Hand(int maxCount) : _maxCount{ maxCount }
{
}

void Hand::AddSpell(std::string& name, ISpell* spell)
{
    if (_spells.size() == _maxCount)
    {
        std::cout << "Невозможно добавление нового заклинания: рука заклинаний заполнена." << std::endl;
        return;
    }
    if (_spells.contains(name))
    {
        std::cout << "Невозможно добавление нового заклинания: это заклинание уже существует." << std::endl;
        return;
    }
    _spells[name] = spell;
}

ISpell* Hand::GetSpell(std::string& name)
{
    if (_spells.contains(name))
        return _spells.at(name);
    return nullptr;
}