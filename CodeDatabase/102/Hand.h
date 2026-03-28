#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "../Entities/ISpell.h"

class Hand
{
    std::unordered_map<std::string, ISpell*> _spells;
    int _maxCount = 0;
public:
    struct HandData
    {
        std::vector<std::string> _spells;

        explicit HandData(const Hand* hand);

        explicit HandData(std::string& s);

        std::string ToString() const;
    };

    explicit Hand(int maxCount);

    void AddSpell(std::string& name, ISpell* spell);

    ISpell* GetSpell(std::string& name);
};