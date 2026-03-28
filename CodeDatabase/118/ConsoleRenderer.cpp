#include "ConsoleRenderer.h"



void ConsoleRenderer::DrawStartMenu()
{
	std::cout << "		Killing Enemies		\n";
	std::cout << "The game ends either when the player dies or when all the enemies dies.\n";
	std::cout << "Choose option:\n1. Load the saved game (enter 1)\n2. Start new game (enter 2)\n3. Quit (enter 3)\n";
}

void ConsoleRenderer::DrawMainWindow(const int lvlNumber, const std::shared_ptr<GameLogic>& logic)
{
	Update();
	std::cout << std::format("		LEVEL {}	\n", lvlNumber);
	int h = logic->GetGameField()->GetHeight();
	int w = logic->GetGameField()->GetWidth();
	std::vector<std::pair<int, int>> enemiesPos{};

	for (int y = -1; y <= h; ++y)
	{
		for (int x = -1; x <= w; ++x)
		{
			auto currPos = std::pair<int, int>(x, y);

			if (!logic->GetGameField()->IsCorrectPos(currPos))
				std::cout << "-";

			else if (logic->GetPlayer()->GetPosition() == currPos)
				std::cout << "P";

			else if (logic->GetEnemySpawner()->GetPosition() == currPos && !logic->GetEnemySpawner()->isDied)
				std::cout << "S";

			else if (logic->GetTower()->GetPosition() == currPos && !logic->GetTower()->isDied)
				std::cout << "T";

			else if (logic->GetGameField()->GetCellByPosForReading(currPos).GetCellContentForReading() != nullptr \
				&& !logic->GetGameField()->GetCellByPosForReading(currPos).GetCellContentForReading()->isDied)
			{
				std::cout << "E";
				enemiesPos.push_back(currPos);
			}

			else if (logic->GetGameField()->GetCellByPosForReading(currPos).GetTypeOfCell() == IMPASSABLE)
				std::cout << "|";

			else if (logic->GetGameField()->GetCellByPosForReading(currPos).GetTypeOfCell() == SLOWING)
				std::cout << "*";

			else if (logic->GetGameField()->GetCellByPosForReading(currPos).GetTypeOfCell() == TRAP)
				std::cout << "#";

			else
				std::cout << ".";
		}
		std::cout << "\n";
	}

	std::string typeAttack = logic->GetPlayer()->GetTypeOfAttack() == MELEE ? "MELEE" : "RANGED";

	std::cout << std::format("\nHealth: {}\nPosition: ({}, {})\nType of attack: {}\nScore: {}\n", logic->GetPlayer()->GetHealth(), logic->GetPlayer()->GetPosition().first, \
		logic->GetPlayer()->GetPosition().second, typeAttack, logic->GetPlayer()->GetScore());
	
	for (auto i = 0; i < logic->GetPlayer()->GetHand().GetCurrSize(); ++i)
	{
		std::shared_ptr<ISpell> spell= logic->GetPlayer()->GetHand().GetSpell(i);

		std::cout << i << "." << spell->GetTitle();
	}
	
	if (!logic->GetEnemySpawner()->isDied)
		std::cout << std::format("EnemySpawner health: {}\nEnemySpawner Position: ({}, {})\n", \
			std::max(logic->GetEnemySpawner()->GetHealth(), 0), \
			logic->GetEnemySpawner()->GetPosition().first, logic->GetEnemySpawner()->GetPosition().second);


	if (!logic->GetTower()->isDied)
		std::cout << std::format("Tower health: {}\nTower Position: ({}, {})\n", \
			std::max(logic->GetTower()->GetHealth(), 0), \
			logic->GetTower()->GetPosition().first, logic->GetTower()->GetPosition().second);

	for (int i = 0; i < enemiesPos.size(); i++)
	{
		auto enemy = logic->GetGameField()->GetCellByPosForReading(enemiesPos[i]).GetCellContentForReading();
		std::cout << std::format("Enemy {}:\n\tHealth: {}\n\tPosition: ({}, {})\n", \
			i, enemy->GetHealth(), enemy->GetPosition().first, enemy->GetPosition().second);
	}
		
	std::cout << "Commands:\n";
	std::cout << "w/a/s/d - Move to forward/left/back/right\n";
	std::cout << "t/b/e - Attack Tower/Spawner/Enemy\n";
	std::cout << "p - Buy new spell.\n";
	std::cout << "u - Use spell\n";
	std::cout << "r - Switch type of attack\n";
	std::cout << "f - Save game\n";
	std::cout << "l - Load game\n";
	std::cout << "q - Quit\n";
}

void ConsoleRenderer::DrawSpellShopMenu()
{
	Update();
	std::cout << "	Spells shop	\n";
	std::cout << "0. Direct damage - " << DIRECT_COST << "\n";
	std::cout << "1. Area damage - " << AREA_COST << "\n";
	std::cout << "2. Spawn trap - " << TRAP_COST << "\n";
}

void ConsoleRenderer::DrawLoseLevelMenu(const std::string& message, const std::string& avaivaibleOptions)
{
	Update();
	std::cout << message;
	std::cout << "Now you can choose one of these option:\n";
	std::cout << avaivaibleOptions;
}


void ConsoleRenderer::DrawEndGame() {
	Update();
	std::cout << "You complete this game! See you later :)\n";
}

void ConsoleRenderer::DrawMessage(const std::string message)
{
	std::cout << message;
}

void ConsoleRenderer::Update()
{
	system("cls");
}