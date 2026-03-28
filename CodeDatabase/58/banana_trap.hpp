#pragma once

#include "i_trap.hpp"

class BananaTrap : public ITrap{
private:
    std::string name = "BananaTrap";

public:
    virtual ~BananaTrap() = default;

    virtual std::string get_name() override;
    virtual bool cast(World* world) override;

    virtual bool can_place(World* world, int x, int y) override;
};