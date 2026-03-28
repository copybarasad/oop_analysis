#include <sstream>
#include "../../Headers/Realizations/StraightTower.h"

StraightTower::StraightTowerData::StraightTowerData(const StraightTower* tower) : _position{ tower->_position },
                                                                                  _damage{ tower->_spell->GetDamage() }
{
}

StraightTower::StraightTowerData::StraightTowerData(std::string& s, int index)
{
    std::istringstream stream{ s };
    std::string line;
    while (line != "STOWER_S" + std::to_string(index) && std::getline(stream, line))
    {
    }
    std::getline(stream, line);
    std::istringstream positionStream{ line };
    _position = Cell();
    positionStream >> _position._x >> _position._y;
    if (_position == Cell())
        return;
    std::getline(stream, line);
    _damage = std::stoi(line);
}

std::string StraightTower::StraightTowerData::ToString(int index) const
{
    std::string stringIndex = std::to_string(index);
    std::string s = "STOWER_S" + stringIndex + '\n' + std::to_string(_position._x) + ' ' +
        std::to_string(_position._y) + '\n' + std::to_string(_damage) + "\nSTOWER_E" + stringIndex;
    return s;
}

StraightTower::StraightTower(const FieldContainer* container, const MortalContainer* mortalContainer,
                             StraightDamage* spell, Cell position) : PositionEntity(position), _container{ container },
                                                                     _mortalContainer{ mortalContainer }, _spell{ spell }
{
}

void StraightTower::ChangeDamageByValue(int value)
{
    _spell->ChangeDamageByValue(value);
}

int StraightTower::GetDamage()
{
    return _spell->GetDamage();
}

void StraightTower::Update()
{
    if (_stepCount == 0)
    {
        int distance = _spell->GetMaxDistance();
        for (int i = -distance; i < distance + 1; i++)
        {
            for (int i1 = -distance; i1 < distance + 1; i1++)
            {
                Cell currPosition = Cell(_position._x + i, _position._y + i1);
                if (i == 0 && i1 == 0 || _container->GetDistance(_position, currPosition) > distance)
                    continue;
                if (PositionEntity* entity = _container->GetEntity(currPosition))
                {
                    IMortal* mortal = _mortalContainer->GetEntity(entity);
                    bool exists = false;
                    for (IMortal* victim : _victims)
                    {
                        if (victim == mortal)
                        {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists)
                        continue;
                    _spell->SetTarget(entity);
                    _spell->Start(_position);
                    _spell->DeleteTarget();
                }
            }
        }
        _stepCount = 3;
    }
    _stepCount--;
}

void StraightTower::AddVictim(IMortal* victim)
{
    _victims.push_back(victim);
}

StraightDamage* StraightTower::GetSpell()
{
    return _spell;
}