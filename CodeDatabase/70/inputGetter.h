#ifndef INPUTGETTER_H
#define INPUTGETTER_H

#include <iostream>
#include <format>

class InputGetter
{
public:
	InputGetter();
	static InputGetter& getInstance();
	void getNumberInRange(int* number, int down_border, int up_border);
	std::pair<int, int> getCell(int left_down_border_x, int left_down_border_y,
                                int right_up_border_x, int right_up_border_y);
};

#endif