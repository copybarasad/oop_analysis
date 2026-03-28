#ifndef POISON_H
#define POISON_H
#include "spellcard.h"
#include "visitor.h"
#include "constants.h"
#include <ostream>
#include <istream>

class Poison : public spellCard
{
public:
    Poison();
    ~Poison() = default;
     bool use_card(Visitor& visitor, int x, int y) override;
    int get_damage() const;
    int get_price() const override;
    void upgrade(int level) override;
    void serialize(std::ostream &os) const override;
    void deserialize(std::istream& is) override;

private:
    int damage;
    int price;
    int radius;

};

#endif // POISON_H
