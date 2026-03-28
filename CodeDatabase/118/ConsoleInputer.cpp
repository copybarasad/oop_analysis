#include "ConsoleInputer.h"

ConsoleInputer::ConsoleInputer()
{
	commandsTable = {
		{'w', MOVE_FORWARD},
		{'a', MOVE_LEFT},
		{'s', MOVE_BACK},
		{'d', MOVE_RIGHT},
		{'t', ATTACK_TOWER},
		{'b', ATTACK_SPAWNER},
		{'e', ATTACK_ENEMY},
		{'p', BUY_SPELL},
		{'u', USE_SPELL_WITH_NUMBER},
		{'r', SWITCH_TYPE_OF_ATTACK},
		{'f', SAVE},
		{'l', LOAD},
		{'1', LOAD},
		{'q', QUIT},
		{'3', QUIT},
		{'2', NEW_GAME}
	};
}

void ConsoleInputer::GetCommandFromUser(Command& command)
{
	char input;
	std::cin >> input;

	auto it = commandsTable.find(input);
	
	if (it == commandsTable.end())
		throw InputException("Unknown command. Try again.\n");

	command = it->second;
}

void ConsoleInputer::GetStringFromUser(std::string& str)
{
	std::cin >> str;
}

void ConsoleInputer::Get—oordinates(std::pair<int, int>& targetPair, std::pair<int, int>& rangeForFirst, std::pair<int, int>& rangeForSecond)
{
	int first, second;

	std::cin >> first >> second;

	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw InputException("Invalid input. Try again.\n");
	}

	if (first < rangeForFirst.first || first > rangeForFirst.second || second < rangeForSecond.first || second > rangeForSecond.second)
		throw InputException("Incorrect input value (out of ranges). Try again.\n");

	targetPair.first = first;
	targetPair.second = second;
}

void ConsoleInputer::GetTargetNumber(int& number, std::pair<int, int>& range)
{
	int x;

	std::cin >> x;

	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw InputException("Invalid input. Try again.\n");
	}

	if (x < range.first || x > range.second)
		throw InputException("Incorrect input value (out of range). Try again.\n");

	number = x;
}