#pragma once
#include <string>
class GameManager;


class IRenderer {
public:
    virtual void Render(const GameManager& gm) = 0;
    virtual void Print(const std::string& msg) = 0;

    virtual ~IRenderer() = default;
};