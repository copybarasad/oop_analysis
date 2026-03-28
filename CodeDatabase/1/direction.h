#pragma once
enum class Direction {
    UP,
    DOWN, 
    LEFT,
    RIGHT
};

int directionToDx(Direction direction);
int directionToDy(Direction direction);
int normalizeComponent(int value);