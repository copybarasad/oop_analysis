#include <iostream>
#include "../../Headers/Controllers/SystemLoopController.h"

#include <random>

SystemLoopController::SystemLoopController() : _currentRoot1{}, _currentRoot2{}, _player{}
{
}

void SystemLoopController::PrintIntroduction()
{
	std::cout << "=========ИГРА=========" << std::endl;
    std::cout << "[Доступные команды] (без скобок).\nХодить по направлению: [a] - налево, [w] - наверх, "
              "[d] - направо, [s] - вниз." << std::endl;
    std::cout << "Задействовать инвентарь: [c] - переключить режим боя, [h] - достать руку заклинаний."
				 "\nВыйти в меню: [m]. \nЗакончить игру: [любой символ или строка]." << std::endl;
    std::cout << "Индексы заклинаний: StraightDamage - 0, AreaDamage - 1, Trap - 2." << std::endl;
}

void SystemLoopController::Initialize()
{
	if (_currentRoot1 != nullptr)
	{
		delete _currentRoot1;
		delete _currentRoot2;
		_enemies.clear();
		_towers.clear();
	}
	std::vector<Cell> walls { Cell(1, 1), Cell(1, 2), Cell(2, 1), Cell(5, 3) };
    _currentRoot1 = new CompositionRootN1{ 17, 17, walls };
    _currentRoot2 = new CompositionRootN2{*_currentRoot1};
    _player = _currentRoot1->_playerCreator.Create(Cell(8, 16), 1, 300, 20,
        10);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist3(0, 2);
    _spells =
    {
        "StraightDamage",
        "AreaDamage",
        "Trap"
    };
    int i = dist3(rng);
    std::cout << "Игроку выпало заклинание: " << _spells[i] << std::endl;
    _currentRoot2->_playerHand.AddSpell(_spells[i], _currentRoot2->_spellCreator.Create(_spells[i]));
    _enemies =
    {
        _currentRoot1->_enemyCreator.Create(Cell(16, 16), 1, 100, 10),
        _currentRoot1->_enemyCreator.Create(Cell(15, 8), 1, 100, 10)
    };
    _towers =
    {
        _currentRoot2->_towerCreator.Create(Cell(15, 7), 10)
    };
    _towers[0]->AddVictim(_player);
}

void SystemLoopController::Initialize(GameData& data)
{
	if (_currentRoot1 != nullptr)
	{
		delete _currentRoot1;
		delete _currentRoot2;
		_enemies.clear();
		_towers.clear();
	}
    _currentRoot1 = new CompositionRootN1{ data._field._fieldWidth, data._field._fieldHeight, data._field._walls };
    _currentRoot2 = new CompositionRootN2{ *_currentRoot1 };
    _player = _currentRoot1->_playerCreator.Create(data._player._position, data._player._speed,
        data._player._maxHealth, data._player._nearDamage, data._player._farDamage);
	_spells =
	{
		"StraightDamage",
		"AreaDamage",
		"Trap"
	};
    for (std::string& spell : data._hand._spells)
        _currentRoot2->_playerHand.AddSpell(spell, _currentRoot2->_spellCreator.Create(spell));
    for (Enemy::EnemyData& enemy : data._enemies)
    {
        _enemies.push_back(_currentRoot1->_enemyCreator.Create(enemy._position, enemy._speed, enemy._maxHealth,
           enemy._damage));
    }
    for (StraightTower::StraightTowerData& tower : data._towers)
    {
        StraightTower* towerInstance = _currentRoot2->_towerCreator.Create(tower._position, tower._damage);
        _towers.push_back(towerInstance);
        towerInstance->AddVictim(_player);
    }
}

void SystemLoopController::PrintField()
{
	int width = _currentRoot1->_container.GetWidth();
	int height = _currentRoot1->_container.GetHeight();
	for (size_t i = 0; i < width; i++)
	{
		for (size_t i1 = 0; i1 < height; i1++)
		{
			Cell cell = _currentRoot1->_container.GetCell(i, i1);
			if (cell != Cell())
			{
				PositionEntity* entity = _currentRoot1->_container.GetEntity(_currentRoot1
					->_container.GetCell(i, i1));
				if (entity == _player)
					std::cout << "P";
				else if (entity == nullptr)
					std::cout << ".";
				else
				{
					for (Enemy* enemy : _enemies)
					{
						if (enemy == entity)
						{
							std::cout << "E";
							break;
						}
					}
					for (StraightTower* tower : _towers)
					{
						if (tower == entity)
						{
							std::cout << "S";
							break;
						}
					}
				}
			}
			else
				std::cout << " ";
		}
		std::cout << "\n";
	}
}

void SystemLoopController::PrintVictory()
{
	std::cout << '\n';
	std::cout << "##############      ##############      ##############      ##############               ###                  #########" << std::endl;
	std::cout << "##############      ##############      ##############      ##############              #####               #############" << std::endl;
	std::cout << "###        ###      ###        ###      ###                 ###                        #######             ###         ###" << std::endl;
	std::cout << "###        ###      ###        ###      ###                 ###                       ###   ###            ###         ###" << std::endl;
	std::cout << "###        ###      ###        ###      ##############      ##############           ###     ###           ###############" << std::endl;
	std::cout << "###        ###      ###        ###      ##############      ##############          ###       ###          ###############" << std::endl;
	std::cout << "###        ###      ###        ###      ###        ###      ###                 #####################      ###         ###" << std::endl;
	std::cout << "###        ###      ###        ###      ###        ###      ###                 #####################      ###         ###" << std::endl;
	std::cout << "###        ###      ##############      ##############      ##############      ###               ###      ###         ###" << std::endl;
	std::cout << "###        ###      ##############      ##############      ##############      ###               ###      ###         ###" << std::endl;
}

void SystemLoopController::PrintDefeat()
{
	std::cout << "ПОРАЖЕНИЕ" << std::endl;
}

/// Возвращает 0 - если игра закончена, 1 - если игра продолжается, 2 - если игра поставлена на паузу (переход в меню).
/// @return
int SystemLoopController::ToNextGameIteration()
{
    char command = 0;
	bool isPlayer = true;
	while (command != -1 && _player->IsAlive())
	{
		char input;
		if (isPlayer)
		{
			std::cout << "Ваш ход.\nКоманда: ";
			std::cin >> input;
			command = ToCommand(input);
			switch (command)
			{
				case 4:
					if (_player->GetCurrentModeIndex() == 1)
						_player->SwitchDamageMode(0);
					else
						_player->SwitchDamageMode(1);
					break;
				case 5:
					std::cout << "Введите индекс заклинания: ";
					int i1;
					std::cin >> i1;
					if (ISpell* spell = _currentRoot2->_playerHand.GetSpell(_spells[i1]))
					{
						int x, y;
						std::cout << "Координаты игрока: (" << _player->GetPosition()._x << "; " <<
								_player->GetPosition()._y << ")" << std::endl;
						std::cout << "Введите координаты для активации.\nX: ";
						std::cin >> x;
						std::cout << "Y: ";
						std::cin >> y;
						if (_currentRoot1->_container.GetCell(x, y) == Cell())
						{
							std::cout << "Координаты вне карты." << std::endl;
							break;
						}
						if (ITargetSpell* targetSpell = dynamic_cast<ITargetSpell*>(spell))
						{
							int enemyCount = _enemies.size();
							int towerCount = _towers.size();
							std::cout << "Выберите индекс цели. Количество доступных сущностей: " <<
								1 + enemyCount + towerCount << "." << std::endl;
							std::cout << "P (" << _player->GetPosition()._x << "; " <<
								_player->GetPosition()._y << ") - 0;" << std::endl;
							for (int i2 = 0; i2 < enemyCount; i2++)
							{
								std::cout << "E (" << _enemies[i2]->GetPosition()._x << "; " <<
									_enemies[i2]->GetPosition()._y << ") - " << 1 + i2 << ";" << std::endl;
							}
							for (int i2 = 0; i2 < towerCount; i2++)
							{
								std::cout << "S (" << _towers[i2]->GetPosition()._x << "; " <<
									_towers[i2]->GetPosition()._y << ") - " << 1 + enemyCount + i2 << ";" << std::endl;
							}
							std::cout << "Индекс: " << std::endl;
							int i2;
							std::cin >> i2;
							if (i2 == 0)
								targetSpell->SetTarget(_player);
							else if (i2 <= enemyCount)
								targetSpell->SetTarget(_enemies[i2 - 1]);
							else
								targetSpell->SetTarget(_towers[i2 - enemyCount - 1]);
						}
						spell->Start(Cell(x, y));
					}
					else
						std::cout << "Такого заклинания нет в вашей руке." << std::endl;
					break;
				case 6:
					return 2;
				case -1:
					PrintDefeat();
					return 0;
				default:
					_currentRoot1->_movableController.Move(_player, Direction(command));
					break;
			}
		}
		else
		{
			std::cout << "Ходы противников." << std::endl;
			Cell playerPosition = _player->GetPosition();
			for (Enemy* enemy : _enemies)
			{
				MovableController* controller = &_currentRoot1->_movableController;
				MoveResult moves[4]
				{
					controller->GetMoving(enemy, Direction::Left),
					controller->GetMoving(enemy, Direction::Up),
					controller->GetMoving(enemy, Direction::Right),
					controller->GetMoving(enemy, Direction::Down)
				};
				int minDirection;
				int minDistance = INT_MAX;
				for (int i1 = 0; i1 < 4; i1++)
				{
					if ((moves[i1]._collision != nullptr && moves[i1]._collision != _player)
						|| moves[i1]._isWallCollision)
						continue;
					int distance = _currentRoot1->_container.GetDistance(playerPosition,
						moves[i1]._destination);
					if (distance < minDistance)
					{
						minDirection = i1;
						minDistance = distance;
					}
				}
				controller->Move(enemy, Direction(minDirection));
			}
		}
		_currentRoot2->_updater.Update();
		bool isKilled = false;
		for (int i1 = 0; i1 < _enemies.size(); i1++)
		{
			if (!_enemies[i1]->IsAlive())
			{
				isKilled = true;
				_currentRoot1->_enemyCreator.Destroy(_enemies[i1]);
				_enemies.erase(_enemies.begin() + i1);
				i1--;
			}
		}
		if (isKilled)
		{
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<std::mt19937::result_type> dist3(0, 2);
			int i2 = dist3(rng);
			std::cout << "Игроку выпало заклинание: " << _spells[i2] << std::endl;
			_currentRoot2->_playerHand.AddSpell(_spells[i2],
				_currentRoot2->_spellCreator.Create(_spells[i2]));
		}
		if (_enemies.empty())
		{
			PrintVictory();
			return 0;
		}
		PrintField();
		isPlayer = !isPlayer;
	}
	PrintDefeat();
	return 0;
}

int SystemLoopController::ToCommand(char command)
{
	if (command == 'a')
		return Direction::Left;
	if (command == 'w')
		return Direction::Up;
	if (command == 'd')
		return Direction::Right;
	if (command == 's')
		return Direction::Down;
	if (command == 'c')
		return 4;
	if (command == 'h')
		return 5;
	if (command == 'm')
		return 6;
	return -1;
}