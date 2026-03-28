#ifndef TEAMMATE_H
#define TEAMMATE_H
#include "spellcard.h"
#include "visitor.h"
#include <ostream>
#include <istream>

class Teammate : public spellCard
{
public:
    Teammate();
    ~Teammate() = default;
    bool use_card(Visitor& visitor, int x, int y) override;
    int get_price() const override;
    void upgrade(int level) override;
    void serialize(std::ostream &os) const override;
    void deserialize(std::istream& is) override;

private:
    int price;
    int x;
    int y;
    int count;
};

#endif // TEAMMATE_H
