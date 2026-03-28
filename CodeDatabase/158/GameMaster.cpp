#include "GameMaster.h"
#include <algorithm>
#include <iostream>

GameMaster::GameMaster(Player &player_ref)
    :player(player_ref), currentLevel(1), gameRunning(true), game_map(25, 25) 
{
    initializeLevel(currentLevel);
}

void GameMaster::initializeLevel(int level) 
{
    int mapSize;
    switch(level) 
    {
        case 1: mapSize = 25; break;
        case 2: mapSize = 20; break;
        case 3: mapSize = 15; break;
        case 4: mapSize = 10; break;
        default: mapSize = 10; break;
    }

    game_map = Map(mapSize, mapSize);
    player.setXY_coord(1, 1);
    player.healFull();
    
    enemies.clear();
    spawners.clear();
    towers.clear();
    traps.clear();
    doppels.clear();
    
    int enemyHP = 3 + (level - 1);
    int enemyATK = 1 + (level - 1);
    
    if (level == 1) 
    {
        enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, 5, 5, 1));
        enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, 10, 10, 1));
        enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, 15, 15, 1));
        spawners.push_back(EnemySpawner(7, 7, 3));
        spawners.push_back(EnemySpawner(20, 20, 4));
        towers.push_back(EnemyTower(12, 12));
        towers.push_back(EnemyTower(18, 18));
        
        if (enable_animations) 
        {
            animPlayer.play("2525", 1);
        }
    } 
    else if (level == 2) 
    {
        enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, 5, 5, 1));
        enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, 10, 10, 1));
        spawners.push_back(EnemySpawner(7, 7, 3));
        towers.push_back(EnemyTower(12, 12));
    } 
    else if (level == 3) 
    {
        enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, 5, 5, 1));
        spawners.push_back(EnemySpawner(5, 5, 3));
    } 
    else if (level == 4) 
    {
        enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, 5, 5, 1));
    }
    
    updateMap();
}

void GameMaster::cleanupDeadEnemies() 
{
    for (size_t i = 0; i < enemies.size(); i++) 
    {
        if (!enemies[i].checkPulse()) 
        {
            enemies.erase(enemies.begin() + i);
            i--;
        }
    }
}

void GameMaster::reset() 
{
    currentLevel = 1;
    gameRunning = true;
    enemies.clear();
    spawners.clear();
    towers.clear();
    traps.clear();
    doppels.clear();
    initializeLevel(currentLevel);
}

MasterCall GameMaster::processMoveRequest(Coordinates current, Coordinates target) 
{
    int x = target.getX_coord();
    int y = target.getY_coord();
    
    if (!game_map.iswithinlimits(x, y)) return MasterCall(current);
    Cell target_cell = game_map.getCell(x, y);
    if (target_cell.CellGetType() == CellType::wall) return MasterCall(current);
    if (target_cell.CellGetType() == CellType::exit_cell) return MasterCall(target, 0, 100);
    
    for (size_t i = 0; i < traps.size(); i++)
        if (traps[i].getX_coord() == x && traps[i].getY_coord() == y) 
        {
            traps.erase(traps.begin() + i);
            return MasterCall(target, 3);
        }
    
    for (size_t i = 0; i < towers.size(); i++)
        if (towers[i].getX_coord() == x && towers[i].getY_coord() == y && towers[i].isAlive())
            return MasterCall(current);
    
    for (size_t i = 0; i < enemies.size(); i++)
        if (enemies[i].getX_coord() == x && enemies[i].getY_coord() == y && enemies[i].checkPulse())
            return MasterCall(current, enemies[i].getAttackDamage());

    for (size_t i = 0; i < spawners.size(); i++)
        if (spawners[i].getX_coord() == x && spawners[i].getY_coord() == y)
            return MasterCall(current, 1);
    
    if (target_cell.CellGetType() == CellType::swamp) return MasterCall(target, 0, 0, 1);
    return MasterCall(target);
}

void GameMaster::updateBuildings() 
{
    for (size_t i = 0; i < spawners.size(); i++)
        if (spawners[i].updateAndSpawn()) 
        {
            int spawnX = spawners[i].getX_coord() + (rand() % 3 - 1);
            int spawnY = spawners[i].getY_coord() + (rand() % 3 - 1);
            if (game_map.iswithinlimits(spawnX, spawnY) && game_map.getCell(spawnX, spawnY).CellIsntWall()) 
            {
                int enemyHP = 3 + (currentLevel - 1);
                int enemyATK = 1 + (currentLevel - 1);
                enemies.push_back(Enemy(false, true, ATKtype::melee, "Ѫ ", enemyHP, enemyATK, spawnX, spawnY, 1));
            }
        }
    
    for (size_t i = 0; i < towers.size(); i++)
        if (towers[i].updateAndAttack() && towers[i].isAlive()) 
        {
            std::vector<Coordinates> attackCoords = towers[i].getAttackCoordinates();
            for (auto& coord : attackCoords)
                if (game_map.iswithinlimits(coord.getX_coord(), coord.getY_coord()))
                    game_map.getCell(coord.getX_coord(), coord.getY_coord()).CellSetTile(". ");
            
            std::cout << "\033c";
            drawMap();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            for (auto& coord : attackCoords) 
            {
                if (game_map.iswithinlimits(coord.getX_coord(), coord.getY_coord())) 
                {
                    game_map.getCell(coord.getX_coord(), coord.getY_coord()).CellResetTile();
                    if (player.getX_coord() == coord.getX_coord() && player.getY_coord() == coord.getY_coord())
                        player.take_damage(1);
                    for (size_t j = 0; j < enemies.size(); j++) 
                    {
                        if (enemies[j].getX_coord() == coord.getX_coord() && 
                            enemies[j].getY_coord() == coord.getY_coord() &&
                            enemies[j].checkPulse()) 
                        {
                            enemies[j].take_damage(1);
                            if (!enemies[j].checkPulse()) 
                            {
                                player.addKDA(1);
                                player.addSoul(2);
                            }
                        }
                    }
                }
            }
        }
}

void GameMaster::updateTraps() 
{
    for (size_t i = 0; i < traps.size(); i++) 
    {
        if (player.getX_coord() == traps[i].getX_coord() && player.getY_coord() == traps[i].getY_coord()) 
        {
            player.take_damage(3);
            traps.erase(traps.begin() + i);
            i--;
            continue;
        }
        
        for (size_t j = 0; j < enemies.size(); j++) 
        {
            if (enemies[j].getX_coord() == traps[i].getX_coord() && 
                enemies[j].getY_coord() == traps[i].getY_coord() &&
                enemies[j].checkPulse()) 
            {
                enemies[j].take_damage(3);
                if (!enemies[j].checkPulse()) 
                {
                    player.addKDA(1);
                    player.addSoul(2);
                }
                traps.erase(traps.begin() + i);
                i--;
                break;
            }
        }
    }
}

void GameMaster::updateDoppels() 
{
    for (size_t i = 0; i < doppels.size(); i++) 
    {
        if (!enemies.empty()) 
        {
            int targetIdx = rand() % enemies.size();
            Coordinates enemyPos(enemies[targetIdx].getX_coord(), enemies[targetIdx].getY_coord());
            Direction moveDir = enemyAI(doppels[i].first, enemyPos);
            Coordinates newPos = doppels[i].first;
            
            switch(moveDir) 
            {
                case Direction::UP: newPos.setXY_coord(newPos.getX_coord(), newPos.getY_coord() - 1); break;
                case Direction::DOWN: newPos.setXY_coord(newPos.getX_coord(), newPos.getY_coord() + 1); break;
                case Direction::LEFT: newPos.setXY_coord(newPos.getX_coord() - 1, newPos.getY_coord()); break;
                case Direction::RIGHT: newPos.setXY_coord(newPos.getX_coord() + 1, newPos.getY_coord()); break;
                default: break;
            }
            
            if (game_map.iswithinlimits(newPos.getX_coord(), newPos.getY_coord()) &&
                game_map.getCell(newPos.getX_coord(), newPos.getY_coord()).CellIsntWall())
                doppels[i].first = newPos;
            
            for (size_t j = 0; j < enemies.size(); j++) 
            {
                if (enemies[j].getX_coord() == doppels[i].first.getX_coord() &&
                    enemies[j].getY_coord() == doppels[i].first.getY_coord() &&
                    enemies[j].checkPulse()) 
                {
                    enemies[j].take_damage(1);
                    if (!enemies[j].checkPulse()) 
                    {
                        player.addKDA(1);
                        player.addSoul(2);
                    }
                }
            }
        }
        
        doppels[i].second--;
        if (doppels[i].second <= 0) 
        {
            doppels.erase(doppels.begin() + i);
            i--;
        }
    }
}

bool GameMaster::processSpellCast(SpellType spell, Coordinates target) 
{
    if (spell != SpellType::DEATH_NOTE && spell != SpellType::LEVEL_UP) 
    {
        player.applyUpgradeBuffer(spell);
    }
    
    bool result = false;
    std::string animation_name = "";
    
    switch(spell) 
    {
        case SpellType::DEATH_NOTE:
            animation_name = "DeathNote";
            for (size_t i = 0; i < enemies.size(); i++) 
            {
                if (enemies[i].getX_coord() == target.getX_coord() && 
                    enemies[i].getY_coord() == target.getY_coord() &&
                    enemies[i].checkPulse()) 
                {
                    enemies[i].take_damage(999);
                    if (!enemies[i].checkPulse()) 
                    {
                        player.addKDA(1);
                        player.addSoul(2);
                        enemies.erase(enemies.begin() + i);
                    }
                    result = true;
                    break;
                }
            }
            for (size_t i = 0; i < towers.size(); i++) 
            {
                if (towers[i].getX_coord() == target.getX_coord() && 
                    towers[i].getY_coord() == target.getY_coord() &&
                    towers[i].isAlive()) 
                {
                    towers[i].take_damage(999);
                    if (!towers[i].isAlive()) 
                    {
                        player.addSoul(5);
                    }
                    result = true;
                    break;
                }
            }
            break;
            
        case SpellType::PSI_STORM: 
        {
            animation_name = "Explosion";
            int spellLevel = 1;
            for (const auto& spellPair : player.listSpells()) 
            {
                if (spellPair.first == static_cast<int>(SpellType::PSI_STORM)) 
                {
                    spellLevel = spellPair.second;
                    break;
                }
            }
            
            int aoeSize = 1 + spellLevel;
            int damage = 1 + spellLevel;
            bool hit = false;
            
            for (int dx = -aoeSize; dx <= aoeSize; dx++) 
            {
                for (int dy = -aoeSize; dy <= aoeSize; dy++) 
                {
                    int x = target.getX_coord() + dx;
                    int y = target.getY_coord() + dy;
                    if (game_map.iswithinlimits(x, y)) 
                    {
                        for (size_t i = 0; i < enemies.size(); i++) 
                        {
                            if (enemies[i].getX_coord() == x && enemies[i].getY_coord() == y &&
                                enemies[i].checkPulse()) 
                            {
                                enemies[i].take_damage(damage);
                                if (!enemies[i].checkPulse()) 
                                {
                                    player.addKDA(1);
                                    player.addSoul(2);
                                    enemies.erase(enemies.begin() + i);
                                    i--;
                                }
                                hit = true;
                            }
                        }
                        for (size_t i = 0; i < towers.size(); i++) 
                        {
                            if (towers[i].getX_coord() == x && towers[i].getY_coord() == y &&
                                towers[i].isAlive()) 
                            {
                                towers[i].take_damage(damage);
                                if (!towers[i].isAlive()) 
                                {
                                    player.addSoul(5);
                                }
                                hit = true;
                            }
                        }
                    }
                }
            }
            result = true;
            break;
        }
            
        case SpellType::TRAP: 
        {
            animation_name = "Trap";
            int trapLevel = 1;
            for (const auto& spellPair : player.listSpells()) 
            {
                if (spellPair.first == static_cast<int>(SpellType::TRAP)) 
                {
                    trapLevel = spellPair.second;
                    break;
                }
            }
            
            int trapsCreated = 0;
            for (int i = 0; i < trapLevel; i++) 
            {
                int offsetX = (rand() % 3) - 1;
                int offsetY = (rand() % 3) - 1;
                Coordinates trapPos(target.getX_coord() + offsetX, 
                                   target.getY_coord() + offsetY);
                
                if (game_map.iswithinlimits(trapPos.getX_coord(), trapPos.getY_coord()) &&
                    game_map.getCell(trapPos.getX_coord(), trapPos.getY_coord()).CellGetType() != CellType::wall) 
                {
                    bool alreadyHasTrap = false;
                    for (const auto& existingTrap : traps) 
                    {
                        if (existingTrap == trapPos) 
                        {
                            alreadyHasTrap = true;
                            break;
                        }
                    }
                    if (!alreadyHasTrap) 
                    {
                        traps.push_back(trapPos);
                        trapsCreated++;
                    }
                }
            }
            
            result = (trapsCreated > 0);
            break;
        }
            
        case SpellType::DOPPELGANGER: 
        {
            animation_name = "Doppelganger";
            int doppelLevel = 1;
            for (const auto& spellPair : player.listSpells()) 
            {
                if (spellPair.first == static_cast<int>(SpellType::DOPPELGANGER)) 
                {
                    doppelLevel = spellPair.second;
                    break;
                }
            }
            
            int doppelsCreated = 0;
            for (int i = 0; i < doppelLevel; i++) 
            {
                int offsetX = (rand() % 5) - 2;
                int offsetY = (rand() % 5) - 2;
                Coordinates doppelPos(player.getX_coord() + offsetX, 
                                     player.getY_coord() + offsetY);
                
                if (game_map.iswithinlimits(doppelPos.getX_coord(), doppelPos.getY_coord()) &&
                    game_map.getCell(doppelPos.getX_coord(), doppelPos.getY_coord()).CellGetType() != CellType::wall) 
                {
                    bool alreadyHasDoppel = false;
                    for (const auto& existingDoppel : doppels) 
                    {
                        if (existingDoppel.first == doppelPos) 
                        {
                            alreadyHasDoppel = true;
                            break;
                        }
                    }
                    if (!alreadyHasDoppel) 
                    {
                        doppels.push_back(std::make_pair(doppelPos, 10));
                        doppelsCreated++;
                    }
                }
            }
            
            result = (doppelsCreated > 0);
            break;
        }
            
        case SpellType::LEVEL_UP:
            player.addUpgradeBuffer();
            animation_name = "Upgrade";
            result = true;
            break;
    }
    
    if (result && !animation_name.empty()) 
    {
        animPlayer.play(animation_name, 1);
    }
    
    if (result) 
    {
        GameLogger::logEvent("Spell cast: " + std::to_string(static_cast<int>(spell)) + 
                            " at (" + std::to_string(target.getX_coord()) + 
                            "," + std::to_string(target.getY_coord()) + ")");
    }
    
    return result;
}

void GameMaster::processPlayerAttack() 
{
    GameLogger::logEvent("Player attacked with " + 
                       std::string(player.getAttackType() == ATKtype::melee ? "MELEE" : "RANGED") +
                       " attack");
    
    std::vector<Coordinates> attackCoords = player.getAttackCoordinates();
    
    for (const auto& coord : attackCoords) 
    {
        if (game_map.iswithinlimits(coord.getX_coord(), coord.getY_coord())) 
        {
            game_map.getCell(coord.getX_coord(), coord.getY_coord()).CellSetTile(". ");
        }
    }
    
    std::cout << "\033c";
    drawMap();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    
    for (const auto& coord : attackCoords) 
    {
        if (game_map.iswithinlimits(coord.getX_coord(), coord.getY_coord())) 
        {
            game_map.getCell(coord.getX_coord(), coord.getY_coord()).CellResetTile();
            
            for (size_t i = 0; i < enemies.size(); i++) 
            {
                if (enemies[i].getX_coord() == coord.getX_coord() && 
                    enemies[i].getY_coord() == coord.getY_coord() &&
                    enemies[i].checkPulse()) 
                {
                    
                    enemies[i].take_damage(player.getAttackDamage());
                    if (!enemies[i].checkPulse()) 
                    {
                        player.addKDA(1);
                        player.addSoul(1);
                        enemies.erase(enemies.begin() + i);
                        i--;
                    }
                }
            }
            
            for (size_t i = 0; i < towers.size(); i++) 
            {
                if (towers[i].getX_coord() == coord.getX_coord() && 
                    towers[i].getY_coord() == coord.getY_coord() &&
                    towers[i].isAlive()) 
                {
                    
                    towers[i].take_damage(player.getAttackDamage());
                    if (!towers[i].isAlive()) 
                    {
                        player.addSoul(3);
                    }
                }
            }
        }
    }
}

void GameMaster::updateMap() 
{
    for (int i = 0; i < game_map.getYSize(); i++)
        for (int j = 0; j < game_map.getXSize(); j++)
            game_map.getCell(j, i).CellResetTile();
    
    game_map.getCell(player.getX_coord(), player.getY_coord()).CellSetTile("ꙮ ");
    for (size_t i = 0; i < traps.size(); i++)
        game_map.getCell(traps[i].getX_coord(), traps[i].getY_coord()).CellSetTile("փ ");
    for (size_t i = 0; i < doppels.size(); i++)
        game_map.getCell(doppels[i].first.getX_coord(), doppels[i].first.getY_coord()).CellSetTile("ʘ ");
    for (size_t i = 0; i < enemies.size(); i++)
        if (enemies[i].checkPulse())
            game_map.getCell(enemies[i].getX_coord(), enemies[i].getY_coord()).CellSetTile("Ѫ ");
    for (size_t i = 0; i < spawners.size(); i++)
        spawners[i].setTile(game_map);
    for (size_t i = 0; i < towers.size(); i++)
        if (towers[i].isAlive())
            towers[i].setTile(game_map);
}

void GameMaster::drawMap() { game_map.draw(); }
std::vector<Enemy> &GameMaster::getEnemies() { return enemies; }
bool GameMaster::isGameRunning() const { return gameRunning; }
void GameMaster::setGameRunning(bool running) { gameRunning = running; }
int GameMaster::getCurrentLevel() const { return currentLevel; }

bool GameMaster::checkLevelCompletion() 
{
    int exitX = game_map.getXSize()-2;
    int exitY = game_map.getYSize()-2;
    
    if (player.getX_coord() == exitX && player.getY_coord() == exitY) 
    {
        currentLevel++;
        if (currentLevel > 4) 
        {
            if (enable_animations) 
            {
                animPlayer.play("Heaven", 1);
            }
            std::cout << "Congratulations! You completed all levels!\n";
            gameRunning = false;
            return true;
        } 
        else 
        {
            if (enable_animations) 
            {
                std::string levelAnim;
                switch(currentLevel) 
                {
                    case 2: levelAnim = "2020"; break;
                    case 3: levelAnim = "1515"; break;
                    case 4: levelAnim = "1010"; break;
                    default: levelAnim = "2525"; break;
                }
                animPlayer.play(levelAnim, 1);
            }
            
            int half = player.listSpells().size() / 2;
            player.removeRandomSpells(half);
            
            player.addSoul(5);
            player.healFull();
            
            std::cout << "Level " << currentLevel << " started! You gained 5 souls and recovered HP!\n";
            
            if (enable_animations) 
            {
                animPlayer.play("Upgrade", 1);
            }
            showUpgradeMenu();
            
            initializeLevel(currentLevel);
            return true;
        }
    }
    return false;
}

void GameMaster::saveGame(const std::string& filename) 
{
    std::stringstream data;
    
    data << currentLevel << "\n";
    
    data << "MAP\n";
    data << game_map.serialize();
    
    data << "PLAYER\n";
    data << player.serialize() << "\n";
    
    data << "ENEMIES " << enemies.size() << "\n";
    for (const auto& enemy : enemies) 
    {
        data << enemy.serialize() << "\n";
    }
    
    data << "SPAWNERS " << spawners.size() << "\n";
    for (const auto& spawner : spawners) 
    {
        data << spawner.serialize() << "\n";
    }
    
    data << "TOWERS " << towers.size() << "\n";
    for (const auto& tower : towers) 
    {
        data << tower.serialize() << "\n";
    }
    
    data << "TRAPS " << traps.size() << "\n";
    for (const auto& trap : traps) 
    {
        data << trap.getX_coord() << " " << trap.getY_coord() << "\n";
    }
    
    data << "DOPPELS " << doppels.size() << "\n";
    for (const auto& doppel : doppels) 
    {
        data << doppel.first.getX_coord() << " " << doppel.first.getY_coord() 
             << " " << doppel.second << "\n";
    }
    
    if (SaveFileProtector::protectFile(filename, data.str())) 
    {
        std::cout << "Game saved to " << filename << " (protected with hash)\n";
    } 
    else 
    {
        std::cout << "Error saving game!\n";
    }
}

bool GameMaster::loadGame(const std::string& filename) 
{
    auto [success, data] = SaveFileProtector::readProtectedFile(filename);
    if (!success) 
    {
        std::cout << "Cannot load game or save file corrupted!\n";
        return false;
    }
    
    try 
    {
        std::stringstream ss(data);
        std::string line;
        
        if (!std::getline(ss, line)) return false;
        currentLevel = std::stoi(line);
        
        if (!std::getline(ss, line) || line != "MAP") return false;
        
        std::string mapData;
        while (std::getline(ss, line)) 
        {
            if (line == "PLAYER") break;
            mapData += line + "\n";
        }
        
        game_map.deserialize(mapData);
        
        std::string playerData;
        while (std::getline(ss, line)) 
        {
            if (line.find("ENEMIES") == 0 || 
                line.find("SPAWNERS") == 0 || 
                line.find("TOWERS") == 0 || 
                line.find("TRAPS") == 0 || 
                line.find("DOPPELS") == 0) 
            {
                break;
            }
            playerData += line + "\n";
        }
        player.deserialize(playerData);
        
        enemies.clear();
        spawners.clear();
        towers.clear();
        traps.clear();
        doppels.clear();
        
        if (line.find("ENEMIES") == 0) 
        {
            int enemyCount = std::stoi(line.substr(8));
            for (int i = 0; i < enemyCount; i++) 
            {
                if (!std::getline(ss, line)) break;
                Enemy enemy;
                enemy.deserialize(line);
                enemies.push_back(enemy);
            }
            if (!std::getline(ss, line)) line = "";
        }
        
        if (line.find("SPAWNERS") == 0) 
        {
            int spawnerCount = std::stoi(line.substr(9));
            for (int i = 0; i < spawnerCount; i++) 
            {
                if (!std::getline(ss, line)) break;
                EnemySpawner spawner;
                spawner.deserialize(line);
                spawners.push_back(spawner);
            }
            if (!std::getline(ss, line)) line = "";
        }
        
        if (line.find("TOWERS") == 0) 
        {
            int towerCount = std::stoi(line.substr(7));
            for (int i = 0; i < towerCount; i++) 
            {
                if (!std::getline(ss, line)) break;
                EnemyTower tower;
                tower.deserialize(line);
                towers.push_back(tower);
            }
            if (!std::getline(ss, line)) line = "";
        }
        
        if (line.find("TRAPS") == 0) 
        {
            int trapCount = std::stoi(line.substr(6));
            for (int i = 0; i < trapCount; i++) 
            {
                int x, y;
                if (!(ss >> x >> y)) break;
                traps.push_back(Coordinates(x, y));
            }
            ss.clear();
            if (!std::getline(ss, line)) line = "";
            if (!std::getline(ss, line)) line = "";
        }
        
        if (line.find("DOPPELS") == 0) 
        {
            int doppelCount = std::stoi(line.substr(8));
            for (int i = 0; i < doppelCount; i++) 
            {
                int x, y, timer;
                if (!(ss >> x >> y >> timer)) break;
                doppels.push_back(std::make_pair(Coordinates(x, y), timer));
            }
        }
        
        updateMap();
        
        if (enable_animations) 
        {
            animPlayer.play("Load", 1);
        }
        
        std::cout << "Game loaded from " << filename << "\n";
        return true;
    }
    catch(const std::exception& e) 
    { 
        std::cout << "Error loading game file: " << e.what() << "\n"; 
        return false; 
    }
}

void GameMaster::showUpgradeMenu() 
{
    std::cout << "Upgrade Menu:\n";
    std::cout << "1. Increase Max HP (+2 HP, Cost: 5 Souls)\n";
    std::cout << "2. Increase Max Spells (+1 slot, Cost: 8 Souls)\n";
    std::cout << "3. Upgrade Attack (Switch to ranged/melee, Cost: 10 Souls)\n";
    std::cout << "4. Exit\n";
    
    int choice;
    std::cin >> choice;
    switch(choice) 
    {
        case 1:
            if (player.getSouls() >= 5) 
            {
                player.spendSoul(5);
                player.setMaxHP(player.getMaxHP() + 2);
                player.healFull();
                std::cout << "Max HP increased to " << player.getMaxHP() << "!\n";
            } 
            else std::cout << "Not enough Souls!\n";
            break;
        case 2:
            if (player.getSouls() >= 8) 
            {
                player.spendSoul(8);
                player.setMaxSpells(player.getMaxSpells() + 1);
                std::cout << "Max spells increased to " << player.getMaxSpells() << "!\n";
            } 
            else std::cout << "Not enough Souls!\n";
            break;
        case 3:
            if (player.getSouls() >= 10) 
            {
                player.spendSoul(10);
                player.switchAttackType();
                std::cout << "Attack upgraded! Now using " 
                          << (player.getAttackType() == ATKtype::melee ? "MELEE" : "RANGED") 
                          << " attack.\n";
            } 
            else std::cout << "Not enough Souls!\n";
            break;
        case 4: break;
        default: std::cout << "Invalid choice!\n";
    }
}

void GameMaster::updateGameWorld() 
{
    updateDoppels();
    
    for (size_t i = 0; i < enemies.size(); i++) 
    {
        if (enemies[i].checkPulse()) 
        {
            Direction enemyDir = enemyAI(
                Coordinates(enemies[i].getX_coord(), enemies[i].getY_coord()),
                Coordinates(player.getX_coord(), player.getY_coord())
            );
            Coordinates enemyTarget = enemies[i].moveto(enemyDir);
            MasterCall enemyResult = processMoveRequest(
                Coordinates(enemies[i].getX_coord(), enemies[i].getY_coord()),
                enemyTarget
            );
            enemies[i].move(enemyResult.getDestination(), 
                          enemyResult.getDamage(), 
                          enemies[i].getSpeed() - enemyResult.getSlowdown());
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    updateBuildings();
    updateTraps();
    updateMap();
}