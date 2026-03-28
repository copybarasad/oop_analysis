#ifndef DIRECTSPELL_H
#define DIRECTSPELL_H
#include "CommonSpell.h"
#include "string"


class DirectSpell : public CommonSpell {
public:
    DirectSpell(std::string name, int dam);
};



#endif
