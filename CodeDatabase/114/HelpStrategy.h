#ifndef HELP_STRATEGY_H
#define HELP_STRATEGY_H

#include "IStrategy.h"

class HelpStrategy : public IInputStrategy {
public:
    bool handle() override;
};

#endif

