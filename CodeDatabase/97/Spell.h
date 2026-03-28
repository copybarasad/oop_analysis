#ifndef UNTITLED_SPELL_H
#define UNTITLED_SPELL_H

#include <memory>
#include <string>

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool apply() = 0;
    virtual std::shared_ptr<Spell> clone() const = 0;
    virtual std::string name() const = 0;
    virtual std::string description() const = 0;
};

#endif // UNTITLED_SPELL_H
