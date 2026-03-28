#ifndef SPELLCASTER_H
#define SPELLCASTER_H

class SpellCaster {
public:
    virtual ~SpellCaster() = default;
    virtual int getPositionX() const = 0;
    virtual int getPositionY() const = 0;
};

#endif