#ifndef SPELLH
#define SPELLH

#define DAM_SPELL "Strait damage"
#define AOE_SPELL "AOE damage"
#define TRAP_SPELL "Trap" 
#define SUMMON_SPELL "Summon ally"
#define UPGRADE_SPELL "Upgrade"
#define DEF_DAM 5
#define DEF_AOE 3
#define DEF_TRAP 4
#define DEF_SUMMON 1
#define DEF_UPGR 1


class map;

class hand;

class Ispell
{
    public:
        virtual bool use_spell(map*, hand*) = 0;

        virtual void upgrade(int) = 0;

        virtual int get_level() = 0;

        virtual std::string get_name() = 0;
};

class damage_spell: public Ispell
{
    int damage, radius;
    public:
        damage_spell(int dmg = DEF_DAM, int rad = 5);

        bool use_spell(map*, hand*);

        bool use_spell(map*);

        void upgrade(int);
        
        int get_level();

        std::string get_name();

        ~damage_spell();
};

class aoe_spell: public Ispell
{
    int damage, radius, square;
    public:
        aoe_spell(int dmg = DEF_AOE, int rad = 4, int sqr = 3);

        bool use_spell(map*, hand*);

        void upgrade(int);

        int get_level();

        std::string get_name();

        ~aoe_spell();
};

class trap_spell: public Ispell
{
    int damage;
    public:
        trap_spell(int dmg = DEF_TRAP);

        bool use_spell(map*, hand*);

        void upgrade(int);

        int get_level();

        std::string get_name();

        ~trap_spell();
};

class summon_spell: public Ispell
{
    int allies_number;
    public:
        summon_spell(int allies = DEF_SUMMON);

        bool use_spell(map*, hand*);

        void upgrade(int);

        int get_level();

        std::string get_name();

        ~summon_spell();
};

class upgrade_spell: public Ispell
{
    int upgrade_status;
    public:
        upgrade_spell(int upgrade_status = DEF_UPGR);

        bool use_spell(map*, hand*);

        void upgrade(int);

        int get_level();

        std::string get_name();

        ~upgrade_spell();
};

#endif