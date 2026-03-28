#pragma once

class IUpdating
{
public:
    virtual void Update() = 0;

    virtual ~IUpdating() = default;
};