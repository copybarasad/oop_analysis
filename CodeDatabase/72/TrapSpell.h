//
// Created by Mac on 26.10.2025.
//

#ifndef GAME_TERM_TRAPSPELL_H
#define GAME_TERM_TRAPSPELL_H
#include <string>

#include "ISpell.h"
#include "ITrapSpell.h"

#define DEFAULT_TRAP_DAMAGE 100

class TrapSpell: public ITrapSpell{

    int damage;

public:

    explicit TrapSpell(const int &  damage = DEFAULT_TRAP_DAMAGE);

    Type getType() const override;

    std::string getName() const override;

    void cast(Field &field) override;

    bool canPost(Field &field, const Position &target) const override;

    void save(Saver &saver) const override;

    void load(Reader &reader) override;
};


#endif //GAME_TERM_TRAPSPELL_H