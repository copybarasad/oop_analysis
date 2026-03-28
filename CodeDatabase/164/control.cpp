#include "control.h"

Control::Control(){
    std::ifstream file(CONFIG_FILE);
    if (!file.is_open()) {
        reset();
    }else{
        try {
            json config = json::parse(file);

            exit = config["exit"].get<char>();
            save = config["save"].get<char>();
            if(exit == save){
                reset();
            }
            load = config["load"].get<char>();
            if(load == exit && load == save){
                reset();
            }
            yes = config["yes"].get<char>();
            if(yes == load && yes == exit && yes == save){
                reset();
            }
            no = config["no"].get<char>();
            if(no == exit && no == save && no == load && no == yes){
                reset();
            }
            move = config["move"].get<char>();
            if(move == exit && move == save && move == load && move == yes && move == no){
                reset();
            }
            change_attack = config["change_attack"].get<char>();
            if(change_attack == exit && change_attack == save && change_attack == load && 
                    change_attack == yes && change_attack == no && change_attack == move){
                reset();
            }
            use_spell = config["use_spell"].get<char>();
            if(use_spell == exit && use_spell == save && use_spell == load && use_spell == yes && 
                    use_spell == no && use_spell == move && use_spell == change_attack){
                reset();
            }
            start = config["start"].get<char>();
            if(start == exit && start == save && start == load && start == yes && 
                    start == no && start == move && start == use_spell && start == change_attack){
                reset();
            }
        } catch (const std::exception& e) {
            reset();
        }
            
        file.close();
    }
};

void Control::reset(){
    json config;
        
    exit = 'e';
    config["exit"] = 'e';
    save = 's';
    config["save"] = 's';
    load = 'l';
    config["load"] = 'l';
    yes = 'y';
    config["yes"] = 'y';
    no = 'n';
    config["no"] = 'n';
    move = 'm';
    config["move"] = 'm';
    change_attack = 'c';
    config["change_attack"] = 'c';
    use_spell = 'u';
    config["use_spell"] = 'u';
    start = 'n';
    config["start"] = 'n';
    help = 'h';
    config["help"] = 'h';
        
    std::ofstream file(CONFIG_FILE);
    if (file.is_open()) {
        file << config.dump(4);
        file.close();
    }
};