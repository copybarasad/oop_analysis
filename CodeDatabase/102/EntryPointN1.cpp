#include <cassert>
#include <iostream>
#include <string>
#include "../Headers/Containers/CompositionRootN1.h"

/*void TestFieldMoving()
{
	std::cout << "=== ���� �� ����������� ���� ===" << std::endl;
	// ���� 1: ����������� �����������
	{
		FieldContainer source(10, 10);
		Player player = Player(Cell(5, 5), 300, 100, 10);
		source.AddEntity(&player);
		size_t originalWidth = source.GetWidth();
		size_t originalHeight = source.GetHeight();
		FieldContainer destination(std::move(source));
		// ���������, ��� ������ �������������
		assert(destination.GetWidth() == originalWidth);
		assert(destination.GetHeight() == originalHeight);
		// ���������, ��� �������� ������ ���������
		assert(source.GetWidth() == 0);
		assert(source.GetHeight() == 0);
		std::cout << "���� �� ����������� ����������� �������." << std::endl;
	}
	// ���� 2: �������� �����������
	{
		FieldContainer source(15, 15);
		FieldContainer destination(5, 5);
		Player player = Player(300, 100, 10, MovableController(Cell(5, 5), source));
		source.AddEntity(&player);
		destination = std::move(source);
		// ��������� �����������
		assert(destination.GetWidth() == 15);
		assert(destination.GetHeight() == 15);
		// ��������� ��������� ���������
		assert(source.GetWidth() == 0);
		assert(source.GetHeight() == 0);
		std::cout << "���� �� �������� ����������� �������." << std::endl;
	}
}

void TestFieldDeepCopy()
{
	std::cout << "=== ���� �� �������� ����������� ���� ===" << std::endl;
	// ���� 1: ����������� �����������
	{
		FieldContainer original(8, 8);
		std::vector<Cell> walls = { Cell(1, 1), Cell(2, 2) };
		FieldContainer originalWithWalls(8, 8, walls);
		// ��������� movable
		Player player = Player(300, 100, 10, MovableController(Cell(3, 3), originalWithWalls));
		originalWithWalls.AddEntity(&player);
		FieldContainer copy = originalWithWalls; // ����������� �����������
		// ���������, ��� ������� ���������
		assert(copy.GetWidth() == originalWithWalls.GetWidth());
		assert(copy.GetHeight() == originalWithWalls.GetHeight());
		// ���������, ��� ��� ������ ������� (�������� �����)
		copy.RemoveCell(1, 2); // ��������� ����� � �����
		// �������� �� ������ ����������
		Cell originalCell = originalWithWalls.GetCell(1, 2);
		// �����������, ��� �������� ������ ������������� � � "������" ���������
		assert(originalCell != NULL); // ������ � ��������� ������ ��������
		std::cout << "���� �� ����������� ����������� �������." << std::endl;
	}
	// ���� 2: �������� ������������ ������������
	{
		FieldContainer original(10, 10);
		FieldContainer copy(5, 5);
		std::vector<Cell> walls = { Cell(5, 5) };
		FieldContainer originalWithWalls(10, 10, walls);
		copy = originalWithWalls; // �������� ������������
		assert(copy.GetWidth() == 10);
		assert(copy.GetHeight() == 10);
		// �������� ����� � ���������, ��� �������� �� ��������
		copy.RemoveCell(5, 6);
		Cell originalCell = originalWithWalls.GetCell(5, 6);
		// �����������, ��� �������� ������ ������������� � � "������" ���������
		assert(originalCell != NULL); // ������ � ��������� ������ ��������
		std::cout << "���� �� �������� ����������� �������." << std::endl;
	}
}*/

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
	return -1;
}

void PrintField(FieldContainer& field, const PositionEntity* player, const std::vector<Enemy*>& enemies)
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
				else if (!entity)
					std::cout << ".";
				else
				{
					for (PositionEntity* enemy : enemies)
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
	std::cout << "Бердичевский Максим, 4381, лабораторная работа №1." << std::endl;
	std::cout << "[Доступные команды] (без скобок).\nХодить по направлению: [a] - налево, [w] - наверх, "
			  "[d] - направо, [s] - вниз." << std::endl;
	std::cout << "Задействовать инвентарь: [c] - переключить режим боя.\nЗакончить игру: [любой символ или строка]." <<
		std::endl;
	std::vector<Cell> walls { Cell(1, 1), Cell(1, 2), Cell(2, 1), Cell(5, 3) };
	CompositionRootN1 root1{ 17, 17, walls };
	Player* player = root1._playerCreator.Create(Cell(8, 16), 1, 300, 20,
		10);
	std::vector<Enemy*> enemies
	{
		root1._enemyCreator.Create(Cell(16, 16), 1, 100, 10),
		root1._enemyCreator.Create(Cell(15, 8), 1, 100, 10)
	};
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
				case -1:
					PrintDefeat();
					return 0;
				default:
					root1._movableController.Move(player, Direction(command));
					for (int i = 0; i < enemies.size(); i++)
					{
						if (!enemies[i]->IsAlive())
						{
							root1._enemyCreator.Destroy(enemies[i]);
							enemies.erase(enemies.begin() + i);
							i--;
						}
					}
					if (enemies.empty())
					{
						PrintVictory();
						return 0;
					}
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
				for (int i = 0; i < 4; i++)
				{
					if ((moves[i]._collision && moves[i]._collision != player) || moves[i]._isWallCollision)
						continue;
					int distance = root1._container.GetDistance(playerPosition, moves[i]._destination);
					if (distance < minDistance)
					{
						minDirection = i;
						minDistance = distance;
					}
				}
				controller->Move(enemy, Direction(minDirection));
			}
		}
		PrintField(root1._container, player, enemies);
		isPlayer = !isPlayer;
	}
	PrintDefeat();
	return 0;
}