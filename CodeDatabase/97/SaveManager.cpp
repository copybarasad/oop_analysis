#include "SaveManager.h"
#include "SaveExceptions.h"
#include "GameState.h"
#include "Player.h"
#include "Field.h"
#include "Cell.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Spawner.h"
#include "Trap.h"
#include "Ally.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <optional>

namespace SaveManager {

    void save(const GameState &state, const std::string &filename) {
        std::ofstream out(filename, std::ios::out | std::ios::trunc);
        if (!out) throw FileError("Cannot open file for saving: " + filename);

        out << "LABGAME v1\n";
        out << "GAME " << state.level() << " " << state.grantsGiven() << " " << state.nextSpellThreshold() << "\n";

        auto player = state.player();
        if (!player) throw SaveError("No player to save");
        out << "PLAYER " << player->hp() << " " << player->hpMax() << " "
            << player->getDamageForCurrentMode()  << " "
            << player->score() << " " << (player->isNear() ? 1 : 0) << " " << (player->hasSkipNextMove() ? 1 : 0) << " "
            << player->hand().capacity() << "\n";

        auto spells = player->hand().spells();
        out << "HAND " << spells.size() << "\n";
        for (const auto &s : spells) out << s << "\n";

        // Save field and entities
        const Field &field = state.model().field();
        out << "FIELD " << field.rows() << " " << field.cols() << "\n";
        for (int r = 0; r < field.rows(); ++r) {
            for (int c = 0; c < field.cols(); ++c) {
                const Cell &cell = field.cellAt({r,c});
                int t = 0; if (!cell.isPassable()) t = 1; else if (cell.isSlow()) t = 2;
                auto e = cell.getEntity();
                if (!e) out << "CELL " << r << " " << c << " " << t << " NONE\n";
                else {
                    if (dynamic_cast<Player*>(e.get())) out << "CELL " << r << " " << c << " " << t << " PLAYER\n";
                    else if (dynamic_cast<EnemyBuilding*>(e.get())) {
                        out << "CELL " << r << " " << c << " " << t << " BUILDING " << e->hp() << "\n";
                    }
                    else if (dynamic_cast<Spawner*>(e.get())) {
                        out << "CELL " << r << " " << c << " " << t << " SPAWNER " << e->hp() << "\n";
                    }
                    else if (dynamic_cast<Trap*>(e.get())) {
                        out << "CELL " << r << " " << c << " " << t << " TRAP " << e->hp() << " " << e->damage() << "\n";
                    }
                    else if (dynamic_cast<Ally*>(e.get())) {
                        out << "CELL " << r << " " << c << " " << t << " ALLY " << e->hp() << " " << e->damage() << "\n";
                    }
                    else if (dynamic_cast<Enemy*>(e.get())) {
                        out << "CELL " << r << " " << c << " " << t << " ENEMY " << e->hp() << " " << e->damage() << "\n";
                    }
                    else {
                        out << "CELL " << r << " " << c << " " << t << " UNKNOWN\n";
                    }
                }
            }
        }

        if (!out) throw SaveError("Failed writing save file: " + filename);
    }

    void load(GameState &state, const std::string &filename) {
        std::ifstream in(filename);
        if (!in) throw FileError("Cannot open file for loading: " + filename);
        std::string header; std::getline(in, header);
        if (header != "LABGAME v1") throw ParseError("Unsupported header/version: " + header);
        std::string line;

        if (!std::getline(in, line)) throw ParseError("Missing GAME line");
        { std::istringstream ss(line); std::string tag; ss>>tag; if (tag!="GAME") throw ParseError("Expected GAME line"); int level, grants, thresh; ss>>level>>grants>>thresh; state.setLevel(level); state.setGrantsGiven(grants); state.setNextSpellThreshold(thresh); }

        if (!std::getline(in, line)) throw ParseError("Missing PLAYER line");
        { std::istringstream ss(line); std::string tag; ss>>tag; if (tag!="PLAYER") throw ParseError("Expected PLAYER line");
            int hp,hpMax,curModeOrDmg,score,isNear,skipNext,handCap;
            ss>>hp>>hpMax>>curModeOrDmg>>score>>isNear>>skipNext>>handCap;
            auto player = std::make_shared<Player>(hpMax, /*nearDamage*/curModeOrDmg, /*farDamage*/curModeOrDmg);
            if (hp < player->hp()) {
                player->takeDamage(player->hp() - hp);
            }
            if (skipNext) player->setSkipNextMove(true);
            state.setPlayer(player, std::nullopt);
        }

        if (!std::getline(in, line)) throw ParseError("Missing HAND line");
        { std::istringstream ss(line); std::string tag; ss>>tag; if (tag!="HAND") throw ParseError("Expected HAND line"); int count; ss>>count;
            for (int i=0;i<count;++i){ if (!std::getline(in,line)) throw ParseError("Unexpected end while reading hand"); state.player()->hand().addSpell(line); }
        }

        if (!std::getline(in, line)) throw ParseError("Missing FIELD line");
        int rows, cols; { std::istringstream ss(line); std::string tag; ss>>tag; if (tag!="FIELD") throw ParseError("Expected FIELD line"); ss>>rows>>cols; state.model() = GameModel(rows, cols); }

        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::istringstream ss(line);
            std::string tag; ss >> tag;
            if (tag != "CELL") throw ParseError("Expected CELL line");
            int r,c,t; std::string type;
            ss >> r >> c >> t >> type;
            if (!state.model().field().inBounds({r,c})) continue;
            if (t == 1) state.model().field().setCellType({r,c}, CellType::Obstacle);
            else if (t == 2) state.model().field().setCellType({r,c}, CellType::Slow);
            else state.model().field().setCellType({r,c}, CellType::Empty);

            if (type == "NONE") continue;
            if (type == "PLAYER") {
                auto pl = state.player();
                state.model().field().placeEntity(pl, {r,c});
                state.setPlayerCoord(std::make_optional<Coord>(Coord{r,c}));
                continue;
            }
            if (type == "ENEMY") {
                int hp,dmg; ss >> hp >> dmg;
                state.model().field().placeEntity(std::make_shared<Enemy>(hp,dmg), {r,c});
                continue;
            }
            if (type == "ALLY") {
                int hp,dmg; ss >> hp >> dmg;
                state.model().field().placeEntity(std::make_shared<Ally>(hp,dmg), {r,c});
                continue;
            }
            if (type == "TRAP") {
                int hp,dmg; ss >> hp >> dmg;
                state.model().field().placeEntity(std::make_shared<Trap>(dmg), {r,c});
                continue;
            }
            if (type == "SPAWNER") {
                int hp; ss >> hp;
                state.model().field().placeEntity(std::make_shared<Spawner>(hp,3), {r,c});
                continue;
            }
            if (type == "BUILDING") {
                int hp; ss >> hp;
                state.model().field().placeEntity(std::make_shared<EnemyBuilding>(hp,5), {r,c});
                continue;
            }
        }
    }

}

