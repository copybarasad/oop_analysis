#ifndef COMMAND_H
#define COMMAND_H

#include <utility>

enum class COMMAND_TYPE {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SHOWENEMIES,
	ATTACK,
	SWAPRANGE,
	PASS,
	BUYSPELL,
	SHOWHAND,
	USESPELL,
	PAUSE
};

class Command
{
private:
	COMMAND_TYPE commandType;
	int option;
	std::pair<int, int> point;
	std::pair<std::pair<int, int>, std::pair<int, int>> area;
public:
	Command();
	void setCommandType(COMMAND_TYPE newCommandType);
	void setOption(int newOption);
	void setPoint(std::pair<int, int> newPoint);
	void setArea(std::pair<std::pair<int, int>, std::pair<int, int>> newArea);

	COMMAND_TYPE getCommandType();
	int getOption();
	std::pair<int, int> getPoint();
	std::pair<std::pair<int, int>, std::pair<int, int>> getArea();
};

#endif