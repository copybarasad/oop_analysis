#pragma once
#include "Saving.h"
#include <fstream>
#include <string>
#include <iostream>

class SaveManager {
private:
    std::string save_filename = "savings/savegame.dat";
    std::string tempsave_filename = "savings/tempsave.dat";
    
public:
    void save_game(const Saving& game_data);
    void load_game(Saving& game_data);
    
    const char* save_game_ans(const Saving& game_data);
    const char* load_game_ans(Saving& game_data);
private:
    void save_saving_data(std::ofstream& file, const Saving& data) ;
    void load_saving_data(std::ifstream& file, Saving& data) ;

    bool save_manager_data(std::ofstream& file, const Saving::ManagerData& data) ;
    bool load_manager_data(std::ifstream& file, Saving::ManagerData& data) ;

    void copy();

    void save_string_vector(std::ofstream& file, const std::vector<std::string>& vec); 
    std::vector<std::string> load_string_vector(std::ifstream& file) ;
};
