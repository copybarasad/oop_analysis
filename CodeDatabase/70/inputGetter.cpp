#include "inputGetter.h"

InputGetter::InputGetter(){}
InputGetter& InputGetter::getInstance()
{
    static InputGetter instance;
    return instance;
}

void InputGetter::getNumberInRange(int* number, int down_border, int up_border)
{
	while (!(std::cin >> *number) or *number < down_border or *number >= up_border) {
        std::cout << std::format("Invalid input. Please enter an integer in range[{};{}): ", down_border, up_border);
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::pair<int, int> InputGetter::getCell(int left_down_border_x,
                                         int left_down_border_y,
                                         int right_up_border_x,
                                         int right_up_border_y)
{
    std::pair<int, int> point;
    point.first = -1;
    point.second = -1;
    std::cout << "Choose point. Axis x: ";
    getNumberInRange(&point.first, left_down_border_x, right_up_border_x);
    std::cout << "Axis y: ";
    getNumberInRange(&point.second, left_down_border_y, right_up_border_y);
    return point;
}