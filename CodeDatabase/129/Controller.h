#include "Control.h"
#include "../game/utils/Functions.h"

template <typename InputType>
class Controller{
private:
    InputType input_handler;
    SaveManager save_manager;
public:
    bool process(Manager& manager){
        char c = input_handler.get_input();

        switch(c){
            case CONTROL::SHIFT_RIGHT: case CONTROL::SHIFT_UP:
            case CONTROL::SHIFT_DOWN: case CONTROL::SHIFT_LEFT:
            case CONTROL::HIT_DOWN: case CONTROL::HIT_UP:
            case CONTROL::HIT_RIGHT: case CONTROL::HIT_LEFT:
            case CONTROL::COMBAT: {
                return manager.process(c, manager.get_level());
            }

            case CONTROL::CHOOSE_SPELL:{
                HelpDisplay hd;
                hd.display_choose_spell();
                char spell_c = input_handler.get_input(); 
                hd.display_spell_was_choosed(spell_c, manager);
                char direction_c = input_handler.get_input(); 
                bool was = manager.use_spell(spell_c, direction_c);       
                hd.display_spell_was_used(was); 
                return true;
            }

            case CONTROL::SAVE_GAME:{
                HelpDisplay hd;
                Saving data = manager.serialize();
                hd.display_save_game(save_manager.save_game_ans(data));
                return true;
            } 

            case CONTROL::LOAD_GAME:{
                HelpDisplay hd;
                Saving data;
                const char* ans = save_manager.load_game_ans(data);
                hd.display_load_game(ans);
                if(std::string(ans) == "Loaded"){
                    manager.deserialize(data);
                }
                return true;
            } 

            case CONTROL::QUIT:{
                return false;
            }

            default:
                return true;
        }
    }
};
