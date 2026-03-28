#pragma once
#include <ncurses.h>
#include <vector>
#include <string>
#include "../saving/Saving.h"
#include "../game/utils/Values.h"
#include "../saving/SaveManager.h"
#include "../game/manager/Manager.h"
#include "../command_manager/Controller.h"
#include "../command_manager/Visualizer.h"
#include "../command_manager/NC_Input.h"
#include "../command_manager/NC_Graphic.h"

class GManager {
private:
    Manager manager;
    
    Controller<NC_Input> controller;
    Visualizer<NC_Graphic> visualizer;
    
public:
    GManager();
    GManager(int height, int width, int hand_size, int level);
    GManager(int height, int width, std::vector<std::string> spells, int level);
    ~GManager();

    void start_work();
    bool work();
    void end_work(bool pass);

    std::vector<std::string> get_reset_spells();
private:
    Saving serialize() const;
    void deserialize(const Saving& data);
};

