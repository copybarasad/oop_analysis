#ifndef COMMAND_MANAGER
#define COMMAND_MANAGER

#include "Commands.h"
#include "GameExceptions.h"
#include "InfoForCommand.h"

template <typename TInputer, typename TRenderer>

class CommandManager
{
	std::unordered_map<Command, std::function<std::shared_ptr<ICommand>()>> playerCommands;
	std::unordered_map<Command, std::function<std::shared_ptr<ICommand>(PlayerConfig&, TInputer&, TRenderer&)>> fileCommands;
public:
	CommandManager()
	{
		playerCommands = std::unordered_map<Command, std::function<std::shared_ptr<ICommand>()>>
		{
			{ MOVE_FORWARD, []() { return std::make_shared<MoveCommand>(std::pair(0, 1)); } },
			{ MOVE_BACK, []() { return std::make_shared<MoveCommand>(std::pair(0, -1)); } },
			{ MOVE_LEFT, []() { return std::make_shared<MoveCommand>(std::pair(-1, 0)); } },
			{ MOVE_RIGHT, []() { return std::make_shared<MoveCommand>(std::pair(1, 0)); } },
			{ ATTACK_SPAWNER, []() { return std::make_shared<AttackSpawnerCommand>(SPAWNER_KILL_SCORE); } },
			{ ATTACK_TOWER, []() { return std::make_shared<AttackTowerCommand>(TOWER_KILL_SCORE); } },
			{ SWITCH_TYPE_OF_ATTACK, []() { return std::make_shared<SwitchTypeOfAttackCommand>(); } }
		};

		fileCommands = std::unordered_map<Command, std::function<std::shared_ptr<ICommand>(PlayerConfig&, TInputer&, TRenderer&)>>
		{
			{ SAVE, [](PlayerConfig& playerConfig, TInputer& inputer, TRenderer& renderer)
				{
					std::string fileName = "";
					renderer.DrawMessage("Enter the name of file to save: ");
					inputer.GetStringFromUser(fileName);
					return std::make_shared<SaveCommand>(playerConfig, fileName);
				}
			},
			{ LOAD, [](PlayerConfig& playerConfig, TInputer& inputer, TRenderer& renderer)
				{
					std::string fileName = "";
					renderer.DrawMessage("Enter the name of file to load: ");
					inputer.GetStringFromUser(fileName);
					return std::make_shared<LoadCommand>(playerConfig, fileName);
				}
			}
		};
	}

	std::shared_ptr<ICommand> GetCommand(TInputer& inputer, TRenderer& renderer, PlayerConfig& config, std::unordered_map<Command, std::vector<InfoForCommand>>& paramRanges, bool& quit)
	{
		Command cmd = UNKNOWN_COMMAND;

		while (cmd == UNKNOWN_COMMAND)
		{
			try
			{
				renderer.DrawMessage("Enter the command: ");
				inputer.GetCommandFromUser(cmd);
				break;
			}
			catch (const GameException& ex)
			{
				renderer.DrawMessage(ex.what());
				continue;
			}
		}

		if (cmd == QUIT)
		{
			quit = true;
			return nullptr;
		}

		if (playerCommands.count(cmd))
			return playerCommands[cmd]();

		if (fileCommands.count(cmd))
			return fileCommands[cmd](config, inputer, renderer);

		if (!paramRanges.count(cmd))
			return nullptr;

		int number = -1;
		std::pair<int, int> targetPos(-1, -1);

		std::pair<int, int> firstRange = paramRanges[cmd][0].rangeOfCommandParam;
		std::pair<int, int> secondRange;

		if (firstRange.first == firstRange.second)
			throw CommandException("This command cannot be performed. Choose other command.\n");

		if (cmd == BUY_SPELL)
			renderer.DrawSpellShopMenu();

		while (number < firstRange.first || number >= firstRange.second)
		{
			try
			{
				renderer.DrawMessage(paramRanges[cmd][0].inputMessage);
				inputer.GetTargetNumber(number, firstRange);
			}

			catch (const GameException& ex)
			{
				renderer.DrawMessage(ex.what());
				continue;
			}
		}

		switch (cmd)
		{
		case ATTACK_ENEMY: return std::make_shared<AttackEnemyCommand>(ENEMY_KILL_SCORE, number);
		case BUY_SPELL: return std::make_shared<BuySpellCommand>(number);
		case USE_SPELL_WITH_NUMBER:
			secondRange = paramRanges[cmd][1].rangeOfCommandParam;
			while (targetPos.first < 0 || targetPos.second < 0 || targetPos.first >= secondRange.first || targetPos.second >= secondRange.second)
			{
				try
				{
					auto xRange = std::pair<int, int>(0, secondRange.first);
					auto yRange = std::pair<int, int>(0, secondRange.second);

					renderer.DrawMessage(paramRanges[cmd][1].inputMessage);
					inputer.GetCoordinates(targetPos, xRange, yRange);
				}
				catch (const GameException& ex)
				{
					renderer.DrawMessage(ex.what());
					continue;
				}
			}
			return std::make_shared<UseSpellCommand>(number, targetPos);

		default:
			break;
		}

		return nullptr;
	}
};

#endif