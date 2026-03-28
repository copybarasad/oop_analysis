#pragma once
#include <string> 

enum class Direction {
	LEFT,
	RIGHT,
	DOWN,
	UP
};

inline std::string getDirectionName(Direction dir) {
	switch (dir) {
	case Direction::LEFT:  return "влево";
	case Direction::RIGHT: return "вправо";
	case Direction::UP:    return "вверх";
	case Direction::DOWN:  return "вниз";
	default:               return "неизвестно";
	}
}