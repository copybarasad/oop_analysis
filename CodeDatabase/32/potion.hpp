#ifndef POTION_H
#define POTION_H

#include <string>

class Potion {
private:
    std::string name_;
    int heal_amount_;

public:
    Potion(const std::string& name, int heal_amount);

    std::string GetName() const;
    int GetHealAmount() const;
};

#endif
