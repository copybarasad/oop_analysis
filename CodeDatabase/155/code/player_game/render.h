#include <string>
#include <string>
#include <iostream>
#include "game_master.h"
#ifndef RENDERH
#define RENDERH

class game_master;

class IRender
{
    public:
        virtual void render(map*) = 0;
};

class console_renderer: public IRender
{
    public:
        console_renderer();

        void render(map*);

        ~console_renderer();
};

template<typename render_type> 
class game_renderer
{
    render_type rend;
    public:
        game_renderer(){}

        void renderer(map* map)
        {
            rend.render(map);
        }
        
        ~game_renderer(){};
};

#endif