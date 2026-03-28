#pragma once
#include <string>
#include <memory>
#include <fstream>
#include "GameState.h"
#include "../game_spells/Spell.h"
#include "../game_spells/DirectDamageSpell.h"
#include "../game_spells/AreaDamageSpell.h"
#include "../game_spells/SpawnAllySpell.h"
#include "../game_spells/TrapSpell.h"
#include "../game_spells/UpgradeSpell.h"

class SaveManager {
public:
    SaveManager() = default;
    ~SaveManager() = default;

    // Сохранение состояния игры в файл
    bool save(GameState& state, const std::string& filename);

    // Загрузка состояния игры из файла
    bool load(GameState& state, const std::string& filename);
};