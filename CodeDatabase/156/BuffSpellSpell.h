//
// Created by Artem on 24.10.2025.
//

#ifndef LABAOOP2_BUFFSPELLSPELL_H
#define LABAOOP2_BUFFSPELLSPELL_H

#include "Spell.h"

class BuffSpellSpell : public Spell{ //если используется заклинание улучшения, то фактически улучщается рука а не другое заклинание!
public:
    BuffSpellSpell(): Spell(SpellType::BuffNextUsedSpell){};
    std::unique_ptr<Spell> clone() const override;
};


#endif //LABAOOP2_BUFFSPELLSPELL_H
