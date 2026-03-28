#ifndef LIGHTNING_H
#define LIGHTNING_H
#include "spellcard.h"
#include "visitor.h"
#include "constants.h"
#include <ostream>
#include <istream>

class Lightning : public spellCard
{
public:
    Lightning();
    ~Lightning() = default;
    bool use_card(Visitor& visitor, int x, int y) override;
    int get_price() const override;
    void upgrade(int level) override;
    void serialize(std::ostream &os) const override;
    void deserialize(std::istream& is) override;

private:
    int damage;
    int price;
    int width_damage;
};

#endif // LIGHTNING_H
