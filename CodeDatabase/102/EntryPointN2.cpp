#include <cassert>
#include <iostream>
#include <random>
#include "../Headers/Containers/CompositionRootN2.h"
#include "../Headers/Containers/Hand.h"

int ToCommand(char command)
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
	return -1;
}

void PrintField(FieldContainer& field, const PositionEntity* player, const std::vector<Enemy*>& enemies,
	const StraightTower* tower)
{
	int width = field.GetWidth();
	int height = field.GetHeight();
	for (size_t i = 0; i < width; i++)
	{
		for (size_t i1 = 0; i1 < height; i1++)
		{
			Cell cell = field.GetCell(i, i1);
			if (cell != Cell())
			{
				PositionEntity* entity = field.GetEntity(field.GetCell(i, i1));
				if (entity == player)
					std::cout << "P";
				else if (entity == tower)
					std::cout << "S";
				else if (entity == nullptr)
					std::cout << ".";
				else
				{
					for (Enemy* enemy : enemies)
					{
						if (enemy == entity)
						{
							std::cout << "E";
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

void PrintVictory()
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

void PrintDefeat()
{
	std::cout << "ПОРАЖЕНИЕ" << std::endl;
}

int main()
{
	system("chcp 65001");
	std::cout << "Бердичевский Максим, 4381, лабораторная работа №2." << std::endl;
	std::cout << "[Доступные команды] (без скобок).\nХодить по направлению: [a] - налево, [w] - наверх, "
			  "[d] - направо, [s] - вниз." << std::endl;
	std::cout << "Задействовать инвентарь: [c] - переключить режим боя, [h] - достать руку заклинаний."
			  "\nЗакончить игру: [любой символ или строка]." << std::endl;
	std::cout << "Индексы заклинаний: StraightDamage - 0, AreaDamage - 1, Trap - 2." << std::endl;
	std::vector<Cell> walls { Cell(1, 1), Cell(1, 2), Cell(2, 1), Cell(5, 3) };
	CompositionRootN1 root1{ 17, 17, walls };
	CompositionRootN2 root2{ root1 };
	Player* player = root1._playerCreator.Create(Cell(8, 16), 1, 300, 20,
		10);
	Hand hand = Hand(3);
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist3(0,2);
	std::string spells[3]
	{
		"StraightDamage",
		"AreaDamage",
		"Trap"
	};
	int i = dist3(rng);
	std::cout << "Игроку выпало заклинание: " << spells[i] << std::endl;
	hand.AddSpell(spells[i], root2._spellCreator.Create(spells[i]));
	std::vector<Enemy*> enemies
	{
		root1._enemyCreator.Create(Cell(16, 16), 1, 100, 10),
		root1._enemyCreator.Create(Cell(15, 8), 1, 100, 10)
	};
	StraightTower* straightTower = root2._towerCreator.Create(Cell(15, 7), 10);
	straightTower->AddVictim(player);
	char command = 0;
	bool isPlayer = true;
	while (command != -1 && player->IsAlive())
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
					if (player->GetCurrentModeIndex() == 1)
						player->SwitchDamageMode(0);
					else
						player->SwitchDamageMode(1);
					break;
				case 5:
					std::cout << "Введите индекс заклинания: ";
					int i1;
					std::cin >> i1;
					if (ISpell* spell = hand.GetSpell(spells[i1]))
					{
						int x, y;
						std::cout << "Координаты игрока: (" << player->GetPosition()._x << "; " <<
								player->GetPosition()._y << ")" << std::endl;
						std::cout << "Введите координаты для активации.\nX: ";
						std::cin >> x;
						std::cout << "Y: ";
						std::cin >> y;
						if (root1._container.GetCell(x, y) == Cell())
						{
							std::cout << "Координаты вне карты." << std::endl;
							break;
						}
						if (ITargetSpell* targetSpell = dynamic_cast<ITargetSpell*>(spell))
						{
							int count = 2 + enemies.size();
							std::cout << "Выберите индекс цели. Количество доступных сущностей: " << count <<
								"." << std::endl;
							std::cout << "P (" << player->GetPosition()._x << "; " <<
								player->GetPosition()._y << ") - 0;" << std::endl;
							for (int i2 = 0; i2 < enemies.size(); i2++)
							{
								std::cout << "E (" << enemies[i2]->GetPosition()._x << "; " <<
									enemies[i2]->GetPosition()._y << ") - " << 1 + i2 << ";" << std::endl;
							}
							std::cout << "S (" << straightTower->GetPosition()._x << "; " <<
								straightTower->GetPosition()._y << ") - " << count - 1 << "." << std::endl;
							std::cout << "Индекс: " << std::endl;
							int i2;
							std::cin >> i2;
							if (i2 == 0)
								targetSpell->SetTarget(player);
							else if (i2 == count - 1)
								targetSpell->SetTarget(straightTower);
							else
								targetSpell->SetTarget(enemies[i2 - 1]);
						}
						spell->Start(Cell(x, y));
					}
					else
						std::cout << "Такого заклинания нет в вашей руке." << std::endl;
					break;
				case -1:
					PrintDefeat();
					return 0;
				default:
					root1._movableController.Move(player, Direction(command));
					break;
			}
		}
		else
		{
			std::cout << "Ходы противников." << std::endl;
			Cell playerPosition = player->GetPosition();
			for (Enemy* enemy : enemies)
			{
				MovableController* controller = &root1._movableController;
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
					if ((moves[i1]._collision != nullptr && moves[i1]._collision != player) || moves[i1]._isWallCollision)
						continue;
					int distance = root1._container.GetDistance(playerPosition, moves[i1]._destination);
					if (distance < minDistance)
					{
						minDirection = i1;
						minDistance = distance;
					}
				}
				controller->Move(enemy, Direction(minDirection));
			}
		}
		root2._updater.Update();
		bool isKilled = false;
		for (int i1 = 0; i1 < enemies.size(); i1++)
		{
			if (!enemies[i1]->IsAlive())
			{
				isKilled = true;
				root1._enemyCreator.Destroy(enemies[i1]);
				enemies.erase(enemies.begin() + i1);
				i1--;
			}
		}
		if (isKilled)
		{
			int i2 = dist3(rng);
			std::cout << "Игроку выпало заклинание: " << spells[i2] << std::endl;
			hand.AddSpell(spells[i2], root2._spellCreator.Create(spells[i2]));
		}
		if (enemies.empty())
		{
			PrintVictory();
			return 0;
		}
		PrintField(root1._container, player, enemies, straightTower);
		isPlayer = !isPlayer;
	}
	PrintDefeat();
	return 0;
}