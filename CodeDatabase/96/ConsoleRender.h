#pragma once
#include "GameManager.h"
#include <string>


class ConsoleRenderer{
public:
    void Render(const GameManager& gm, std::string state);
    void Print(const std::string& msg);
};