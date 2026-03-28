#ifndef TRAP_H
#define TRAP_H
#include "spellcard.h"
#include "visitor.h"
#include <ostream>
#include <istream>

class Trap : public spellCard
{
public:
    Trap();
    ~Trap() = default;
    bool use_card(Visitor& visitor, int x, int y) override;
    int get_price() const override;
    void upgrade(int level) override;
    int get_damage() const;
    void serialize(std::ostream &os) const override;
    void deserialize(std::istream& is) override;

private:
    int damage;
    int price;
};

#endif // TRAP_H
