#ifndef IMPROVE_H
#define IMPROVE_H
#include <QPoint>
#include "spellCard.h"
#include "visitor.h"
#include "constants.h"
#include <ostream>
#include <istream>

class Improve : public spellCard
{
public:
    Improve();
    ~Improve() =default;
    bool use_card(Visitor& visitor, int x, int y) override;
    int get_price() const override;
    void upgrade(int level) override;
    int get_level();
    void serialize(std::ostream &os) const override;
    void deserialize(std::istream& is) override;

private:
    int price;
    int level;
};

#endif // IMPROVE_H
