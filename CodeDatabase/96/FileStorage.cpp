#include <filesystem>
#include "FileStorage.h"

template <typename Stream, typename T>
void SafeRead(Stream& file, T& value, const std::string& errMsg) {
    if (!(file >> value))
        throw std::runtime_error(errMsg);
}






FileStorage::FileStorage(const std::string& filePath, Mode mode)
    : path(filePath), mode(mode)
{
    namespace fs = std::filesystem;

    // Проверяем каталог
    fs::path p = fs::path(path).parent_path();
    if (!p.empty() && !fs::exists(p)) {
        throw std::runtime_error("Directory does not exist: " + p.string());
    }

    // Если читаем — файл обязан существовать
    if (mode == Mode::Read) {
        if (!fs::exists(path)) {
            throw std::runtime_error("File does not exist: " + path);
        }
        if (!fs::is_regular_file(path)) {
            throw std::runtime_error("Not a regular file: " + path);
        }
    }

    // Режим открытия
    std::ios::openmode openMode = (mode == Mode::Write)
        ? (std::ios::out | std::ios::trunc)
        : std::ios::in;

    file.open(path, openMode);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    // Доп. проверка на запись
    if (mode == Mode::Write) {
        if (!file.good()) {
            throw std::runtime_error("File opened but not writable: " + path);
        }
    }
}




FileStorage::~FileStorage(){
    if (file.is_open()) {
        file.close();
    }
}


void FileStorage::Save(savedata data){
    if (mode != Mode::Write)
        throw std::runtime_error("File opened in read mode");
    file.close();

    // Открываем заново для записи
    file.open(path, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + path);
    }

    // Проверка доступности записи
    if (!file.good()) {
        throw std::runtime_error("File is open but not writable: " + path);
    }

    // Пробуем записать пустую строку — проверка write-access:
    file << "";
    if (!file.good()) {
        throw std::runtime_error("No write permission to file: " + path);
    }
    file << "SAVE\n\n";
    file << data.currentLevel << '\n' << data.improvehand << '\n' << data.improvehealth <<"\n\n";
    file << "field\n";
    file << data.field.width << ' ' << data.field.height << '\n';
    for(auto vec : data.field.grid){
        for (auto cell : vec){
            file << typeCelltoString(cell.GetType()) << '\n';
            file << cell.IsOccupied() << '\n';
            file << cell.GetTrapDamage() << '\n';
        }
    }
    file << '\n';
    file << "PlayerManager\n";
    file << data.player.pos.first << ' ' << data.player.pos.second << '\n';
    file << attackModetoString(data.player.mode) << '\n';
    file << data.player.score << '\n' << data.player.damage << '\n'
         << data.player.health << '\n' << data.player.stunDuration << '\n';
    file << '\n';
    
    file << "EnemyManager\n";
    file << data.enemyManager.numEnemies << '\n';
    for(auto dataenemy: data.enemyManager.enemies){
        file << dataenemy.pos.first << ' ' << dataenemy.pos.second << '\n';
        file << dataenemy.damage << '\n' << dataenemy.health << '\n'
             << dataenemy.stunDuration << '\n';
    }
    file << '\n';

    file << "BuildingManager\n";
    file << data.buildingManager.numBuildings << '\n';
    for (auto databuilding : data.buildingManager.buildings){
        file << databuilding.pos.first << ' ' << databuilding.pos.second << '\n';
        file << databuilding.spawnCooldown << '\n' << databuilding.spawnInterval << '\n'
             << databuilding.damage << '\n' << databuilding.health << '\n'
             << databuilding.stunDuration << '\n';
    }
    file << '\n';

    file << "TowerManager\n";
    file << data.towerManager.numTowers << '\n';
    for(auto datatower : data.towerManager.towers){
        file << datatower.pos.first << ' ' << datatower.pos.second << '\n';
        file << datatower.damageCooldown << '\n' << datatower.damageInterval << '\n'
             << datatower.range << '\n' << datatower.damage << '\n'
             << datatower.health << '\n' << datatower.stunDuration << '\n';
    }
    file << '\n';

    file << "AllyManager\n";
    file << data.allyManager.numAllies << '\n';
    for(auto dataally : data.allyManager.Allies){
        file << dataally.pos.first << ' ' << dataally.pos.second << '\n';
        file << dataally.damage << '\n' << dataally.health << '\n'
             << dataally.stunDuration << '\n';
    }
    file << '\n';

    file << "Hand\n";
    file << data.hand.spells.size() << '\n';
    file << data.hand.maxSpell << '\n' << data.hand.currentSpell << '\n';
    for(std::shared_ptr<dataSpell> spell : data.hand.spells){
        file << spellTypetoString(spell->type) << '\n';
        if (auto s = std::dynamic_pointer_cast<empowerSpell>(spell)) {
            file << s->bonus << '\n';
        } else if (auto s = std::dynamic_pointer_cast<areaDamageSpell>(spell)) {
            file << s->damage << '\n';
            file << s->radius << '\n';
        } else if (auto s = std::dynamic_pointer_cast<directDamageSpell>(spell)) {
            file << s->damage << '\n';
            file << s->range << '\n';
        } else if (auto s = std::dynamic_pointer_cast<summAllySpell>(spell)) {
            file << s->numAllies << '\n';
        } else if (auto s = std::dynamic_pointer_cast<trapSpell>(spell)) {
            file << s->damage << '\n';
            file << s->range << '\n';
        }
    }
    if (!file) {
        throw std::runtime_error("Failed to write data to file: " + path);
    }
    
}



savedata FileStorage::Load() {
    if (mode != Mode::Read)
        throw std::runtime_error("File opened in write mode");

    if (!file.is_open())
        throw std::runtime_error("File is not open: " + path);

    savedata data;
    std::string token;

    // --- 1. Заголовок SAVE ---
    SafeRead(file, token, "Invalid save format: expected SAVE");
    if (token != "SAVE")
        throw std::runtime_error("Invalid save format: missing SAVE");

    // --- 2. Текущий уровень ---
    SafeRead(file, data.currentLevel, "Failed to read currentLevel");
    SafeRead(file, data.improvehand, "Failed to read improvehand");
    SafeRead(file, data.improvehealth, "Failed to read improvehealth");
    
    // ---------------- FIELD -----------------
    SafeRead(file, token, "Failed to read token 'field'");
    if (token != "field")
        throw std::runtime_error("Invalid save: expected 'field'");

    SafeRead(file, data.field.width,  "Field width missing");
    SafeRead(file, data.field.height, "Field height missing");
    if (data.field.width < 10 || data.field.height < 10 || data.field.height > 25 || data.field.width > 25)
        throw std::runtime_error("Invalid field size");
    
    data.field.grid.resize(data.field.height, std::vector<Cell>(data.field.width));

    for (int y = 0; y < data.field.height; y++){
        for (int x = 0; x < data.field.width; x++){
            
            std::string typeStr;
            bool occupied;
            int trapDamage;

            SafeRead(file, typeStr,      "Invalid cell type");
            SafeRead(file, occupied,     "Invalid occupied flag");
            SafeRead(file, trapDamage,   "Invalid trap damage");

            Cell cell;
            cell.SetType(stringToTypeCell(typeStr));
            cell.SetOccupied(occupied);
            cell.SetTrap(trapDamage);

            data.field.grid[y][x] = cell;
        }
    }
    
    // ---------------- PLAYER -----------------
    SafeRead(file, token, "Failed to read token PlayerManager");
    if (token != "PlayerManager")
        throw std::runtime_error("Invalid save: expected PlayerManager");

    SafeRead(file, data.player.pos.first,  "Invalid player pos.x");
    SafeRead(file, data.player.pos.second, "Invalid player pos.y");
    

    std::string modeStr;
    SafeRead(file, modeStr, "Invalid attack mode");
    data.player.mode = stringToAttackMode(modeStr);

    SafeRead(file, data.player.score,        "Invalid player score");
    SafeRead(file, data.player.damage,       "Invalid player damage");
    SafeRead(file, data.player.health,       "Invalid player health");
    SafeRead(file, data.player.stunDuration, "Invalid player stun");

    // ---------------- ENEMIES -----------------
    SafeRead(file, token, "Expected EnemyManager");
    if (token != "EnemyManager")
        throw std::runtime_error("Invalid save: expected EnemyManager");

    SafeRead(file, data.enemyManager.numEnemies, "Invalid enemy count");
    if (data.enemyManager.numEnemies < 0)
        throw std::runtime_error("Enemy count out of range");

    data.enemyManager.enemies.resize(data.enemyManager.numEnemies);

    for (int i = 0; i < data.enemyManager.numEnemies; i++){
        auto &e = data.enemyManager.enemies[i];
        SafeRead(file, e.pos.first,  "Invalid enemy pos.x");
        SafeRead(file, e.pos.second, "Invalid enemy pos.y");
        SafeRead(file, e.damage,     "Invalid enemy damage");
        SafeRead(file, e.health,     "Invalid enemy health");
        SafeRead(file, e.stunDuration,"Invalid enemy stun");
    }

    // ---------------- BUILDINGS -----------------
    SafeRead(file, token, "Expected BuildingManager");
    if (token != "BuildingManager")
        throw std::runtime_error("Invalid save: expected BuildingManager");

    SafeRead(file, data.buildingManager.numBuildings, "Invalid building count");
    if (data.buildingManager.numBuildings < 0)
        throw std::runtime_error("Enemy building count out of range");
    data.buildingManager.buildings.resize(data.buildingManager.numBuildings);

    for (int i = 0; i < data.buildingManager.numBuildings; i++){
        auto &b = data.buildingManager.buildings[i];

        SafeRead(file, b.pos.first,  "Invalid building pos.x");
        SafeRead(file, b.pos.second, "Invalid building pos.y");
        SafeRead(file, b.spawnCooldown,"Invalid cooldown");
        SafeRead(file, b.spawnInterval,"Invalid interval");
        SafeRead(file, b.damage,      "Invalid building damage");
        SafeRead(file, b.health,      "Invalid building health");
        SafeRead(file, b.stunDuration,"Invalid building stun");
    }

    // ---------------- TOWERS -----------------
    SafeRead(file, token, "Expected TowerManager");
    if (token != "TowerManager")
        throw std::runtime_error("Invalid save: expected TowerManager");

    SafeRead(file, data.towerManager.numTowers, "Invalid tower count");

    if (data.towerManager.numTowers < 0)
        throw std::runtime_error("Towers count out of range");
    data.towerManager.towers.resize(data.towerManager.numTowers);

    for (int i = 0; i < data.towerManager.numTowers; i++){
        auto &t = data.towerManager.towers[i];
        SafeRead(file, t.pos.first,     "Invalid tower pos.x");
        SafeRead(file, t.pos.second,    "Invalid tower pos.y");
        SafeRead(file, t.damageCooldown,"Invalid tower cooldown");
        SafeRead(file, t.damageInterval,"Invalid tower interval");
        SafeRead(file, t.range,         "Invalid tower range");
        SafeRead(file, t.damage,        "Invalid tower damage");
        SafeRead(file, t.health,        "Invalid tower health");
        SafeRead(file, t.stunDuration,  "Invalid tower stun");
    }

    // ---------------- ALLIES -----------------
    SafeRead(file, token, "Expected AllyManager");
    if (token != "AllyManager")
        throw std::runtime_error("Invalid save: expected AllyManager");

    SafeRead(file, data.allyManager.numAllies, "Invalid ally count");
    if (data.allyManager.numAllies < 0)
        throw std::runtime_error("Ally count out of range");
    data.allyManager.Allies.resize(data.allyManager.numAllies);

    for (int i = 0; i < data.allyManager.numAllies; i++){
        auto &a = data.allyManager.Allies[i];
        SafeRead(file, a.pos.first,     "Invalid ally pos.x");
        SafeRead(file, a.pos.second,    "Invalid ally pos.y");
        SafeRead(file, a.damage,        "Invalid ally damage");
        SafeRead(file, a.health,        "Invalid ally health");
        SafeRead(file, a.stunDuration,  "Invalid ally stun");
    }
    
    // ---------------- HAND -----------------
    SafeRead(file, token, "Expected Hand");
    if (token != "Hand")
        throw std::runtime_error("Invalid save: expected Hand");
    int numSpells;
    SafeRead(file, numSpells, "Invalid spell count");
    SafeRead(file, data.hand.maxSpell,     "Invalid maxSpell");
    SafeRead(file, data.hand.currentSpell, "Invalid currentSpell");
    if (numSpells < 0 || numSpells > data.hand.maxSpell)
        throw std::runtime_error("Spell count out of range");

    data.hand.spells.clear();
    for (int i = 0; i < numSpells; i++){
        std::string spellTypeStr;
        SafeRead(file, spellTypeStr, "Invalid spell type");
        SpellType type = stringToSpellType(spellTypeStr);
        std::shared_ptr<dataSpell> spell;

        switch(type) {
            case SpellType::empower: {
                auto s = std::make_shared<empowerSpell>();
                SafeRead(file, s->bonus, "Invalid empower bonus");
                spell = s;
                break;
            }
            case SpellType::areaDamage: {
                auto s = std::make_shared<areaDamageSpell>();
                SafeRead(file, s->damage, "Invalid area damage");
                SafeRead(file, s->radius, "Invalid area radius");
                spell = s;
                break;
            }
            case SpellType::directDamage: {
                auto s = std::make_shared<directDamageSpell>();
                SafeRead(file, s->damage, "Invalid direct damage");
                SafeRead(file, s->range,  "Invalid direct range");
                spell = s;
                break;
            }
            case SpellType::summAlly: {
                auto s = std::make_shared<summAllySpell>();
                SafeRead(file, s->numAllies, "Invalid ally count");
                spell = s;
                break;
            }
            case SpellType::trap: {
                auto s = std::make_shared<trapSpell>();
                SafeRead(file, s->damage, "Invalid trap damage");
                SafeRead(file, s->range,  "Invalid trap range");
                spell = s;
                break;
            }
            default:
                throw std::runtime_error("Unknown spell type");
        }

        data.hand.spells.push_back(spell);
    }
    if (!file)
        throw std::runtime_error("Failed to read data from file: " + path);

    return data;
}
