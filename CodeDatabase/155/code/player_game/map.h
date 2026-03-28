#include <vector>
#include "player.h"
#include "../dummy/usables.h"
#include "../dummy/enemy.h"
#include "../dummy/enemy_barrack.h"
#include "../dummy/enemy_tower.h"
#include "../dummy/ally.h"
#ifndef MAPH
#define MAPH


#define LEFT_UP_CORNER "┌"
#define RIGHT_UP_CORNER "┐"
#define LEFT_DOWN_CORNER "└"
#define RIGHT_DOWN_CORNER "┘"
#define UP_DOWN_WALL "─"
#define RIGHT_LEFT_WALL "│"

#define EMPTY "∘"
#define ENEMY "▲"
#define PLAYER "♥"
#define WALL "█"
#define SLOW "░"
#define ALLY "♠"
#define ENEMY_BARRACS "☗"
#define ENEMY_TOWER "♝"
#define CHEST "◈"
#define TRAP "⋇"

struct tile
{
    std::string status;
    dummy* adress;
};

const tile border = {WALL, nullptr};

class map
{
    int _width, _height;
    tile** _tiles;
    player* player_ptr;
    std::vector<ally*> allies;
    std::vector<enemy*> enemies;
    std::vector<enemy_barrack*> barracks;
    std::vector<enemy_tower*> towers;
    std::vector<trap*> traps;

    public:

        map(int width = 25, int height = 20, player* pl = nullptr);
        
        map(player* pl, int width, int height, tile** , std::vector<ally*>, std::vector<enemy*>, std::vector<enemy_barrack*>, std::vector<enemy_tower*>, std::vector<trap*>);

        void generate(std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>, player*);

        void gen_one(int, std::string);
        
        void gen_one(int, std::string, int, std::vector<int>);

        void render();


        int get_height();

        int get_width();

        std::vector<ally*> get_allies();

        std::vector<enemy*> get_enemies();
        
        std::vector<enemy_barrack*> get_enemy_barracks();

        std::vector<enemy_tower*> get_enemy_towers();

        std::vector<trap*> get_traps();

        player* get_pl_ptr();
        
        tile get_tile(int, int);


        void replace_tile(std::string, int, int, int, int);

        void replace_tile(std::string, int, int, int, int, dummy*);


        void add_ally(ally*, int, int);

        void add_enemy(enemy*, int, int);

        void add_enemy_barrack(enemy_barrack*, int, int);

        void add_enemy_tower(enemy_tower*, int, int);

        void add_trap(trap*, int, int);

        void del_ally(ally*);

        void del_enemy(enemy*);

        void del_enemy_barrack(enemy_barrack*);

        void del_enemy_tower(enemy_tower*);

        void del_trap(trap*);

        
        std::string save_parameters();

        std::string save_par_helper(int, char);

        ~map();
};

#endif