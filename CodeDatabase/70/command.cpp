#include "command.h"

Command::Command() : commandType(COMMAND_TYPE::PASS), option(0), point({-1,-1}), area({{-1,-1},{-1,-1}}){}

void Command::setCommandType(COMMAND_TYPE newCommandType) {commandType = newCommandType;}
void Command::setOption(int newOption) {option = newOption;}
void Command::setPoint(std::pair<int, int> newPoint) {point = newPoint;}
void Command::setArea(std::pair<std::pair<int, int>, std::pair<int, int>> newArea) {area = newArea;}

COMMAND_TYPE Command::getCommandType(){return commandType;};
int Command::getOption(){return option;};
std::pair<int, int> Command::getPoint(){return point;};
std::pair<std::pair<int, int>, std::pair<int, int>> Command::getArea(){return area;};