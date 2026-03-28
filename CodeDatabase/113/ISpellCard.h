#ifndef ISPELLCARD_H
#define ISPELLCARD_H

#include <string>

class ISpellCard {
public:
    virtual ~ISpellCard() = default;
    virtual const std::string& getName() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
    virtual std::string getDescription() const = 0;
};

#endif // ISPELLCARD_H