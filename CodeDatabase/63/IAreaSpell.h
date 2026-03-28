#ifndef RPG_IAREASPELL_H
#define RPG_IAREASPELL_H

#include <utility>

class IAreaSpell {
public:
    virtual ~IAreaSpell() = default;

    [[nodiscard]] virtual std::pair<int,int> visualData() const = 0;
};

#endif
