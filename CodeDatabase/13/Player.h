#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "../basics/Entity.h"
#include "../basics/ObjTypes.h"
#include "../Weapons/Pan.h"
#include "../Weapons/Mixer.h"
#include "../Weapons/Book.h"
#include "../Spells/SpellManager.h"

class Player final : public Entity {
private:
    bool is_book_open_ = false;
    bool is_alive_ = true;
    SpellManager spell_book_;
    Book *book_ = new Book(this);
    int max_hp_ = 7;

public:
    explicit Player() : Entity("Player", 7, ObjTypes::Friendly, 0,
                               {new Pan(this), new Mixer(this)}) {
    };

    void onDeath() override;

    void activateSpellBook();

    bool isAlive() const;

    void findTarget(int dx, int dy);

    SpellManager *getSpellBook();

    void switchBookState();

    bool isBookOpen() const;

    void healFull();

    void addMaxHp(int hp);

    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif
