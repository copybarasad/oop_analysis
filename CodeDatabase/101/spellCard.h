#ifndef SPELLCARD_H
#define SPELLCARD_H
#include <ostream>
#include <istream>

class Visitor;

class spellCard
{
public:
    virtual bool use_card(Visitor& visitor, int x, int y) = 0;
    virtual int get_price() const = 0;
    virtual void upgrade(int level) = 0;
    virtual ~spellCard() = default;
    virtual void serialize(std::ostream& os) const = 0;
    virtual void deserialize(std::istream& is) = 0;

};

#endif // SPELLCARD_H
