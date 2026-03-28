#ifndef ACTION_H
#define ACTION_H

#include <string>

class Action {
public:
    virtual ~Action() = default;
    virtual bool execute() = 0;
    virtual std::string getName() const = 0;
};

#endif