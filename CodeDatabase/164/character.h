#ifndef CHARACTER_H
#define CHARACTER_H

enum class character_type{
    not_defined, // ne opredeleno   
    hero,        // geroy           &
    ally,        // pomoschtschnik  @
    enemy,       // vrag            $
    base,        // baza            *
    tower        // baschnya        T
};

class Character{
    protected:
    character_type type;
    unsigned int hp;
    bool move;

    public:
    Character(unsigned int game_lvl = 1, unsigned int hard_lvl = 1);

    Character(const Character& character);

    Character& operator= (const Character& character);

    bool get_damage(unsigned int damage);

    bool can_move();

    character_type get_type();

    virtual void change_move();

    virtual unsigned int give_points();

    int get_hp();
};

#endif