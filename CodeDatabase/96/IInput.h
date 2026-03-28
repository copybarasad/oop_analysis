#pragma once
#include <string>


class IInput {
public:
    virtual std::string Read() = 0;
    virtual ~IInput() = default;
};