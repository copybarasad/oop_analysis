#pragma once
#include "GameConfig.h"


struct LevelConfig : public GameConfig {
    std::string name = "НА ТИХОМ БЕРЕГУ";
    int next_lvl_ind = 2;

    LevelConfig(const std::string new_name, int lvl_id, int n_lvl_id) : GameConfig(), name(new_name)
                                                        , next_lvl_ind(n_lvl_id) {
        this->lvl_index = lvl_id;
    }

    LevelConfig(){}
};