#include "LoadSaveGame.h"
#include "GameField.h"
#include "Player.h"
#include "PlayerHand.h"
#include "Spell.h"
#include "StraightSpell.h"
#include "SquareSpell.h"
#include "Trap.h"
#include "Summon.h"
#include "UpgradeSpell.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>

using std::string;

static std::string spellType(const Spell* s) {
    if (!s) return "NONE";
    if (dynamic_cast<const StraightSpell*>(s)) return "STRAIGHT";
    if (dynamic_cast<const SquareSpell*>(s)) return "SQUARE";
    if (dynamic_cast<const TrapSpell*>(s)) return "TRAP";
    if (dynamic_cast<const SummonSpell*>(s)) return "SUMMON";
    if (dynamic_cast<const UpgradeSpell*>(s)) return "UPGRADE";
    return "UNKNOWN";
}

static std::unique_ptr<Spell> createSpellByName(const std::string& name) {
    if (name == "STRAIGHT") return std::make_unique<StraightSpell>();
    if (name == "SQUARE")   return std::make_unique<SquareSpell>();
    if (name == "TRAP")     return std::make_unique<TrapSpell>();
    if (name == "SUMMON")   return std::make_unique<SummonSpell>();
    if (name == "UPGRADE")  return std::make_unique<UpgradeSpell>();
    return nullptr;
}

void LoadSaveGame::save(GameField& field, Player& player, int level, const std::string& filename) {
    SetFileAttributesA(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs.is_open()) throw FileIOException("Cannot open file for saving: " + filename);

    try {
        ofs << "LEVEL " << level << "\n";

        ofs << "FIELD " << field.getWidth() << " " << field.getHeight() << "\n";

        Position ppos = player.getPosition();
        ofs << "PLAYER " << ppos.x << " " << ppos.y << " " << player.getHealth() << " " << player.getMana() << " " << player.getScore() << "\n";

        {
            PlayerHand& hand = player.getHand();
            ofs << "HAND " << hand.getSize() << " " << hand.getMaxSize() << "\n";
            for (int i = 0; i < hand.getSize(); ++i) {
                const Spell* s = hand.getSpell(i);
                string t = spellType(s);
                ofs << "CARD " << t << "\n";
            }
        }

        const auto& enemies = field.getEnemies();
        ofs << "ENEMIES " << enemies.size() << "\n";
        for (auto* e : enemies) {
            Position pos = e->getPosition();
            ofs << "E " << pos.x << " " << pos.y << " " << e->getHealth() << "\n";
        }

        const auto& towers = field.getTowers(); 
        ofs << "TOWERS " << towers.size() << "\n";
        for (const auto& t : towers) {
            Position pos = t.getPosition();
            ofs << "T " << pos.x << " " << pos.y << " " << t.getDamage() << "\n";
        }

        const auto& traps = field.getTraps(); 
        ofs << "TRAPS " << traps.size() << "\n";
        for (const auto& tr : traps) {
            Position pos = tr.getPosition();
            ofs << "TRAP " << pos.x << " " << pos.y << " " << tr.getDamage() << "\n";
        }

        ofs << "END\n";
    }
    catch (const std::exception& ex) {
        throw SaveException(std::string("Failed during save: ") + ex.what());
    }
    ofs.close();
    SetFileAttributesA(filename.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY);
}

void LoadSaveGame::load(GameField*& outField, Player*& outPlayer, int& outLevel, const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) throw FileIOException("Cannot open file for loading: " + filename);

    string line;
    int width = 15, height = 15;
    int level = 1;
    Position playerPos{ 0,0 };
    int playerHealth;
    int playerMana; 
    int playerScore;
    std::vector<std::string> handCards;
    struct EnemyRec { int x, y, h; };
    std::vector<EnemyRec> enemyRecs;
    struct TowerRec { int x, y, d; };
    std::vector<TowerRec> towerRecs;
    struct TrapRec { int x, y, d; };
    std::vector<TrapRec> trapRecs;


    try {
        while (std::getline(ifs, line)) {
            if (line.empty()) continue;
            std::istringstream iss(line);
            string tag;
            iss >> tag;
            if (tag == "LEVEL") {
                if (!(iss >> level)) throw ParseException("LEVEL parse error");
            }
            else if (tag == "FIELD") {
                if (!(iss >> width >> height)) throw ParseException("FIELD parse error");
            }
            else if (tag == "PLAYER") {
                if (!(iss >> playerPos.x >> playerPos.y >> playerHealth >> playerMana >> playerScore))
                    throw ParseException("PLAYER parse error");
            }
            else if (tag == "HAND") {
                int cnt, maxs;
                if (!(iss >> cnt >> maxs)) throw ParseException("HAND parse error");
            }
            else if (tag == "CARD") {
                string cname;
                if (!(iss >> cname)) throw ParseException("CARD parse error");
                handCards.push_back(cname);
            }
            else if (tag == "ENEMIES") {
                int n; if (!(iss >> n)) throw ParseException("ENEMIES parse error");
            }
            else if (tag == "E") {
                EnemyRec er; if (!(iss >> er.x >> er.y >> er.h)) throw ParseException("E parse error");
                enemyRecs.push_back(er);
            }
            else if (tag == "TOWERS") {
                int n; if (!(iss >> n)) throw ParseException("TOWERS parse error");
            }
            else if (tag == "T") {
                TowerRec tr; if (!(iss >> tr.x >> tr.y >> tr.d)) throw ParseException("T parse error");
                towerRecs.push_back(tr);
            }
            else if (tag == "TRAPS") {
                int n; if (!(iss >> n)) throw ParseException("TRAPS parse error");
            }
            else if (tag == "TRAP") {
                TrapRec tr; if (!(iss >> tr.x >> tr.y >> tr.d)) throw ParseException("TRAP parse error");
                trapRecs.push_back(tr);
            }
            else if (tag == "END") {
                break;
            }
            else {
                throw ParseException("Unknown token in save file: " + tag);
            }
        }

        if (width <= 0 || height <= 0) throw ParseException("Invalid field size in save");
        outField = new GameField(width, height);

        outPlayer = new Player(playerPos.x, playerPos.y, playerHealth, playerMana, 10 );
        outPlayer->addScore(playerScore);
        outField->player = outPlayer;
        outField->getCell(playerPos).setHasPlayer(true);

        PlayerHand& hand = outPlayer->getHand();
        for (const auto& cname : handCards) {
            std::unique_ptr<Spell> sp = createSpellByName(cname);
            if (sp) hand.addSpell(std::move(sp));
        }

        for (auto& er : enemyRecs) {
            outField->addEnemy(er.x, er.y);
        }

        for (auto& tr : towerRecs) {
            EnemyTower tower(tr.x, tr.y, tr.d);
            outField->addTower(tower);
        }

        for (auto& tr : trapRecs) {
            TrapOnField tpos(tr.x, tr.y, tr.d);
            outField->addTrap(tpos);
        }

        outLevel = level;
    }
    catch (const ParseException&) {
        throw;
    }
    catch (const std::exception& ex) {
        throw LoadException(std::string("Failed during load: ") + ex.what());
    }
}
