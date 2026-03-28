//
// Created by Mac on 25.10.2025.
//

#ifndef GAME_TERM_HAND_H
#define GAME_TERM_HAND_H
#include <vector>

#include "../spells/ISpell.h"

#define DEFAULT_MAXSIZE 5

class Hand: public ISerializable{

    std::vector<std::shared_ptr<ISpell>> spells;
    int maxSize;

public:
    explicit Hand(const int  &maxSpells = DEFAULT_MAXSIZE);

    void addSpell(const std::shared_ptr<ISpell> & spell);

    void addRandomSpell();

    void showHand() const;

    std::shared_ptr<ISpell> getSpell(size_t index);

    size_t getCount() const;

    bool canAddSpell() const;

    void removeHalfRandom();

    void save(Saver &saver) const override;

    void load(Reader &reader) override;
};


#endif //GAME_TERM_HAND_H