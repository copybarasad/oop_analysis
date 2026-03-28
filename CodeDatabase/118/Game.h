#ifndef GAME_H
#define GAME_H

#include "LevelManager.h"

#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "Spells.h"

#include "GameLogic.h"
#include "ConsoleRenderer.h"

#include "CommandManager.h"
#include "ConsoleInputer.h"

template<typename TInputer, typename TRenderer>

class Game
{
	std::shared_ptr<TRenderer> renderer;
	std::shared_ptr<TInputer> inputer;

	std::shared_ptr<CommandManager<TInputer, TRenderer>> commandManager;
	std::shared_ptr<LevelManager> levelManager;

	std::shared_ptr<PlayerConfig> playerConfig;

	std::shared_ptr<GameLogic> gameLogic;

	std::unordered_map<Command, std::vector<InfoForCommand>> inputRanges;
	bool quit = false;

	void UpdateInputRanges()
	{
		inputRanges[ATTACK_ENEMY][0].rangeOfCommandParam = std::pair(0, (int)gameLogic->GetEnemyPositions().size());
		inputRanges[BUY_SPELL][0].rangeOfCommandParam = std::pair(0, (int)gameLogic->GetTableOfSpells().size());
		inputRanges[USE_SPELL_WITH_NUMBER][0].rangeOfCommandParam = std::pair(0, gameLogic->GetPlayer()->GetHand().GetCurrSize());
		inputRanges[USE_SPELL_WITH_NUMBER][1].rangeOfCommandParam = std::pair(gameLogic->GetGameField()->GetWidth(), gameLogic->GetGameField()->GetHeight());
	}

	bool HasLevelComplete()
	{
		return gameLogic->GetTower()->isDied && gameLogic->GetEnemySpawner()->isDied && gameLogic->GetEnemyPositions().size() == 0;
	}

	void RunLevel(int lvlNumber)
	{
		int currStep = 0;
		bool currCommandIsExecuted = false;

		while (!(quit || gameLogic->GetPlayer()->isDied || HasLevelComplete()))
		{
			renderer->DrawMainWindow(lvlNumber, gameLogic);

			UpdateInputRanges();

			std::shared_ptr<ICommand> cmd;

			while (!currCommandIsExecuted)
			{
				try { cmd = commandManager->GetCommand(*inputer, *renderer, *playerConfig, inputRanges, quit); }
				catch (const GameException& ex) { renderer->DrawMessage(ex.what()); continue; }

				if (quit)
					return;

				try { cmd->Execute(*gameLogic); currCommandIsExecuted = true; }
				catch (const GameException& ex) { renderer->DrawMessage(ex.what()); continue; }
			}

			gameLogic->EnemiesMove();

			if (currStep > 0)
				gameLogic->SpawnEnemy(currStep);

			if (!gameLogic->GetTower()->isDied)
				gameLogic->GetTower()->Attack(gameLogic->GetPlayer());

			currStep++;
			currCommandIsExecuted = false;
		}
	}

public:
	Game(const std::vector<LevelTemplate>& templates)
		: renderer(std::make_shared<TRenderer>()),
		inputer(std::make_shared<TInputer>()),
		commandManager(std::make_shared<CommandManager<TInputer, TRenderer>>()),
		levelManager(std::make_shared<LevelManager>(templates)),
		playerConfig(std::make_shared<PlayerConfig>()),
		gameLogic(std::make_shared<GameLogic>())
	{
		inputRanges = std::unordered_map<Command, std::vector<InfoForCommand>>
		{
			{ ATTACK_ENEMY, { InfoForCommand("Enter the number of enemy: ") } },
			{ BUY_SPELL, { InfoForCommand("Enter the number of spell: ") } },
			{ USE_SPELL_WITH_NUMBER, { InfoForCommand("Enter the number of spell for using: "), InfoForCommand("Enter the coordinates of target cell: ") } }
		};
	}

	void Run()
	{
		renderer->DrawStartMenu();
		std::pair range(1, 3);
		std::shared_ptr<ICommand> currOption;

		while (!quit)
		{
			currOption = commandManager->GetCommand(*inputer, *renderer, *playerConfig, inputRanges, quit);

			if (quit)
				return;

			if (currOption != nullptr)
			{
				try
				{
					currOption->Execute(*gameLogic);
				}
				catch (const GameException& ex)
				{
					renderer->DrawMessage(ex.what());
					continue;
				}
			}

			else
				levelManager->LoadNextLevel(*playerConfig, *gameLogic);

			while (!quit && playerConfig->currLvl < levelManager->GetCountLevels())
			{
				RunLevel(playerConfig->currLvl + 1);

				if (gameLogic->GetPlayer()->isDied)
				{
					renderer->DrawLoseLevelMenu("You lose :(\n", "1. Load saved game\n2. Retry complete this level\n3. Quit\n");
					break;
				}

				if (HasLevelComplete())
				{
					try
					{
						levelManager->PrepareStateForNextLevel(*playerConfig, *gameLogic);
						levelManager->LoadNextLevel(*playerConfig, *gameLogic);
					}
					catch (const GameException&)
					{
						renderer->DrawEndGame();
						break;
					}
				}
			}
		}
	}
};

#endif