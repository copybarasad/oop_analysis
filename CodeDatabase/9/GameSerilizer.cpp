#include "GameSerializer.h"

#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "LevelManager.h"
#include "Spell.h"
#include "SpellDirect.h"
#include "SpellAoE.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EmpowerSpell.h"
#include "Config.h"
#include "SaveException.h"
#include "LoadException.h"
#include "InvalidSaveDataException.h"

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace {

std::string DifficultyToString(Difficulty diff) {
    switch (diff) {
    case Difficulty::Easy:   return "Easy";
    case Difficulty::Normal: return "Normal";
    case Difficulty::Hard:   return "Hard";
    }
    return "Normal";
}

Difficulty DifficultyFromString(const std::string& value) {
    if (value == "Easy")   return Difficulty::Easy;
    if (value == "Normal") return Difficulty::Normal;
    if (value == "Hard")   return Difficulty::Hard;
    throw InvalidSaveDataException("Unknown difficulty in save: " + value);
}

}

void GameSerializer::Save(Board& board,
                          Player& player,
                          std::vector<Enemy>& enemies,
                          std::vector<Ally>& allies,
                          EnemyBase* base,
                          std::vector<EnemyTower>& towers,
                          LevelManager& levelManager,
                          Difficulty difficulty,
                          int hpBonus,
                          int meleeBonus,
                          int directSpellBonus,
                          const std::string& filename)
{
    try {
        json root;

        root["meta"]["version"] = 1;
        root["meta"]["currentLevel"] = levelManager.CurrentLevel();
        root["meta"]["totalLevels"]  = levelManager.TotalLevels();

        root["meta"]["hpBonus"] = hpBonus;
        root["meta"]["meleeBonus"]  = meleeBonus;
        root["meta"]["directSpellBonus"] = directSpellBonus;

        root["settings"]["boardWidth"]  = board.width();
        root["settings"]["boardHeight"] = board.height();
        root["settings"]["difficulty"]  = DifficultyToString(difficulty);

        auto pp = player.position();
        root["player"]["x"] = pp.first;
        root["player"]["y"]  = pp.second;
        root["player"]["hp"]   = player.hp();
        root["player"]["score"]  = player.score();
        root["player"]["rangedMode"]  = player.isMode();
        root["player"]["skipNextTurn"] = player.shouldSkipTurn();

        SpellModifier pending = player.PeekPendingModifier();
        root["player"]["pendingMod"]["radiusDelta"]  = pending.radiusDelta;
        root["player"]["pendingMod"]["aoeSizeDelta"]  = pending.aoeSizeDelta;
        root["player"]["pendingMod"]["damageDelta"] = pending.damageDelta;
        root["player"]["pendingMod"]["summonCountDelta"] = pending.summonCountDelta;

        root["board"]["width"]  = board.width();
        root["board"]["height"] = board.height();
        root["board"]["cells"]  = json::array();

        for (int y = 0; y < board.height(); ++y) {
            for (int x = 0; x < board.width(); ++x) {
                Cell& c = board.at({x, y});

                json jc;
                jc["x"]  = x;
                jc["y"] = y;
                jc["passable"]  = c.isPassable();
                jc["slow"] = c.isSlow();
                jc["symbol"]    = std::string(1, c.getSymbol());
                jc["hasTrap"]   = c.hasTrap();
                jc["trapDamage"] = c.trapDamage();

                root["board"]["cells"].push_back(jc);
            }
        }

        root["enemies"] = json::array();
        for (auto& e : enemies) {
            auto ep = e.position();
            json je;
            je["x"] = ep.first;
            je["y"] = ep.second;
            je["hp"]  = e.hp();
            je["skipNextTurn"] = e.shouldSkipTurn();
            root["enemies"].push_back(je);
        }

        root["allies"] = json::array();
        for (auto& a : allies) {
            auto ap = a.position();
            json ja;
            ja["x"]  = ap.first;
            ja["y"]  = ap.second;
            ja["hp"]  = a.hp();
            ja["skipNextTurn"] = a.shouldSkipTurn();
            root["allies"].push_back(ja);
        }

        if (base != nullptr) {
            auto bp = base->position();
            root["base"]["exists"]       = true;
            root["base"]["x"]  = bp.first;
            root["base"]["y"]  = bp.second;
            root["base"]["hp"] = base->hp();
            root["base"]["spawnDelay"]  = base->spawnDelay();
            root["base"]["currentTimer"] = base->currentTimer();
        } else {
            root["base"]["exists"] = false;
        }

        root["towers"] = json::array();
        for (const auto& t : towers) {
            if (!t.isAlive()) continue;
            auto tp = t.position();
            json jt;
            jt["x"]  = tp.first;
            jt["y"] = tp.second;
            jt["hp"]= t.hp();
            jt["cooldown"] = t.cooldown();
            root["towers"].push_back(jt);
        }

        root["hand"]["selectedIndex"] = player.hand().GetSelectedIndex();
        root["hand"]["cards"] = json::array();

        std::vector<const char*> card_names;
        player.hand().GetCardNames(card_names);
        for (const char* name : card_names) {
            json jc;
            jc["type"] = std::string(name);
            root["hand"]["cards"].push_back(jc);
        }

        std::ofstream out(filename);
        if (!out.is_open()) {
            throw SaveException("Cannot open save file for writing: " + filename);
        }
        out << root.dump(4);
    }
    catch (const nlohmann::json::exception& e) {
        throw SaveException(std::string("JSON error while saving: ") + e.what());
    }
    catch (const std::exception& e) {
        throw SaveException(std::string("Error while saving: ") + e.what());
    }
}

void GameSerializer::Load(
    Board& board,
    Player& player,
    std::vector<Enemy>& enemies,
    std::vector<Ally>& allies,
    EnemyBase*& base,
    std::vector<EnemyTower>& towers,
    LevelManager& level_manager,
    Difficulty& difficulty,
    int& hp_bonus,
    int& melee_bonus,
    int& direct_spell_bonus,
    const std::string& filename)
{
    json root;

    try {
        std::ifstream in(filename);
        if (!in.is_open()) {
            throw LoadException("Cannot open save file: " + filename);
        }

        try {
            in >> root;
        } catch (const json::parse_error& e) {
            throw InvalidSaveDataException(std::string("JSON parse error: ") + e.what());
        }

        if (!root.is_object()) {
            throw InvalidSaveDataException("Save root is not an object.");
        }

        if (!root.contains("board") || !root.contains("player")) {
            throw InvalidSaveDataException("Save file is missing board/player sections.");
        }

        if (root.contains("meta")) {
            const auto& jm = root.at("meta");

            if (jm.contains("totalLevels")) {
                level_manager.SetTotalLevels(jm.at("totalLevels").get<int>());
            }
            if (jm.contains("currentLevel")) {
                level_manager.SetCurrentLevel(jm.at("currentLevel").get<int>());
            }

            hp_bonus = 0;
            melee_bonus = 0;
            direct_spell_bonus = 0;

            if (jm.contains("hpBonus")) {
                hp_bonus = jm.at("hpBonus").get<int>();
            }
            if (jm.contains("meleeBonus")) {
                melee_bonus = jm.at("meleeBonus").get<int>();
            }
            if (jm.contains("directSpellBonus")) {
                direct_spell_bonus = jm.at("directSpellBonus").get<int>();
            }
        }

        const auto& jBoard = root.at("board");
        int w = jBoard.at("width").get<int>();
        int h = jBoard.at("height").get<int>();

        if (w < 10 || h < 10 || w > 25 || h > 25) {
            throw InvalidSaveDataException("Board size from save is out of allowed range.");
        }

        enemies.clear();
        allies.clear();
        towers.clear();
        if (base != nullptr) {
            delete base;
            base = nullptr;
        }

        board = Board(w, h);

        for (const auto& jc : jBoard.at("cells")) {
            int x = jc.at("x").get<int>();
            int y = jc.at("y").get<int>();

            if (x < 0 || x >= board.width() || y < 0 || y >= board.height()) {
                throw InvalidSaveDataException("Cell coordinates out of range in save.");
            }

            Cell& c = board.at({x, y});
            c.setPassable(jc.at("passable").get<bool>());
            c.setSlow(jc.at("slow").get<bool>());

            char sym = jc.at("symbol").get<std::string>()[0];
            c.setSymbol(sym);

            bool has_trap = jc.at("hasTrap").get<bool>();
            int trap_damage = jc.at("trapDamage").get<int>();
            if (has_trap) {
                c.setTrap(trap_damage);
            } else {
                c.clearTrap();
            }
        }
        if (root.contains("settings") && root.at("settings").contains("difficulty")) {
            const auto& js = root.at("settings");
            std::string diffName = js.at("difficulty").get<std::string>();

            if (diffName == "Easy") {
                difficulty = Difficulty::Easy;
            } else if (diffName == "Normal") {
                difficulty = Difficulty::Normal;
            } else if (diffName == "Hard") {
                difficulty = Difficulty::Hard;
            } else {
                throw InvalidSaveDataException("Unknown difficulty in save: " + diffName);
            }

            level_manager.SetDifficulty(difficulty);
        }

        const auto& jp = root.at("player");
        int px = jp.at("x").get<int>();
        int py = jp.at("y").get<int>();
        int php = jp.at("hp").get<int>();
        int pscore = jp.at("score").get<int>();
        bool ranged_mode = jp.at("rangedMode").get<bool>();
        bool skip_turn = jp.at("skipNextTurn").get<bool>();

        player = Player(PLAYER_HP, PLAYER_MELEE_DAMAGE, PLAYER_RANGED_DAMAGE, {px, py});
        player.setPosition({px, py});
        player.setHp(php);
        if (pscore > 0) {
            player.addScore(pscore);
        }
        if (ranged_mode != player.isMode()) {
            player.AttackMobile();
        }
        player.setSkipNextTurn(skip_turn);

        SpellModifier pending{};
        if (jp.contains("pendingMod")) {
            const auto& jpm = jp.at("pendingMod");
            pending.radiusDelta = jpm.at("radiusDelta").get<int>();
            pending.aoeSizeDelta = jpm.at("aoeSizeDelta").get<int>();
            pending.damageDelta = jpm.at("damageDelta").get<int>();
            pending.summonCountDelta = jpm.at("summonCountDelta").get<int>();
        }
        if (pending.radiusDelta || pending.aoeSizeDelta ||
            pending.damageDelta || pending.summonCountDelta) {
            player.AddPendingModifier(pending);
        }

        enemies.clear();
        if (root.contains("enemies")) {
            const auto& jEnemies = root.at("enemies");
            if (!jEnemies.is_array()) {
                throw InvalidSaveDataException("enemies section is not an array.");
            }

            for (const auto& je : jEnemies) {
                int ex = je.at("x").get<int>();
                int ey = je.at("y").get<int>();
                int ehp = je.at("hp").get<int>();
                bool eskip = je.at("skipNextTurn").get<bool>();

                enemies.push_back(Enemy(level_manager.EnemyHp(), level_manager.EnemyDamage(), {ex, ey}));
                Enemy& e = enemies.back();
                e.setPosition({ex, ey});
                e.setHp(ehp);
                e.setSkipNextTurn(eskip);
            }
        }

        allies.clear();
        if (root.contains("allies")) {
            const auto& jAllies = root.at("allies");
            if (!jAllies.is_array()) {
                throw InvalidSaveDataException("allies section is not an array.");
            }

            for (const auto& ja : jAllies) {
                int ax = ja.at("x").get<int>();
                int ay = ja.at("y").get<int>();
                int ahp = ja.at("hp").get<int>();
                bool askip = ja.at("skipNextTurn").get<bool>();

                allies.push_back(Ally(ALLY_HP, ALLY_DAMAGE, {ax, ay}));
                Ally& a = allies.back();
                a.setPosition({ax, ay});
                a.setHp(ahp);
                a.setSkipNextTurn(askip);
            }
        }

        if (root.contains("base") &&
            root.at("base").contains("exists") &&
            root.at("base").at("exists").get<bool>()) {

            const auto& jb = root.at("base");
            int bx = jb.at("x").get<int>();
            int by = jb.at("y").get<int>();
            int bhp = jb.at("hp").get<int>();
            int bdelay = jb.at("spawnDelay").get<int>();
            int btimer = jb.at("currentTimer").get<int>();

            base = new EnemyBase({bx, by}, bdelay);
            base->setHp(bhp);
            base->setCurrentTimer(btimer);
        } else {
            base = nullptr;
        }

        towers.clear();
        if (root.contains("towers")) {
            const auto& jTowers = root.at("towers");
            if (!jTowers.is_array()) {
                throw InvalidSaveDataException("towers section is not an array.");
            }

            for (const auto& jt : jTowers) {
                int tx = jt.at("x").get<int>();
                int ty = jt.at("y").get<int>();
                int thp = jt.at("hp").get<int>();
                int tcool = jt.at("cooldown").get<int>();

                towers.push_back(
                    EnemyTower({tx, ty}, TOWER_RANGE,
                               level_manager.TowerDamage(),
                               TOWER_COOLDOWN,
                               level_manager.TowerHp())
                );
                EnemyTower& t = towers.back();
                t.setHp(thp);
                t.setCooldown(tcool);
            }
        }
        if (root.contains("hand")) {
            const auto& jh = root.at("hand");
            Hand& hand = player.hand();

            if (jh.contains("cards")) {
                const auto& jCards = jh.at("cards");
                if (!jCards.is_array()) {
                    throw InvalidSaveDataException("hand.cards is not an array.");
                }

                for (const auto& jc : jCards) {
                    std::string type = jc.at("type").get<std::string>();

                    if (type == "Direct damage" || type == "DirectDamage") {
                        hand.AddCard(new DirectDamageSpell(DIRECT_RANGE, DIRECT_DAMAGE + direct_spell_bonus));
                    } else if (type == "AoE" || type == "Area damage") {
                        hand.AddCard(new AoESpell(AOE_RANGE, AOE_DAMAGE, AOE_SIZE));
                    } else if (type == "Trap") {
                        hand.AddCard(new TrapSpell(TRAP_RANGE, TRAP_DAMAGE));
                    } else if (type == "Summon") {
                        hand.AddCard(new SummonSpell(1));
                    } else if (type == "Empower") {
                        hand.AddCard(new EmpowerSpell(
                            EMPOWER_RADIUS_DELTA,
                            EMPOWER_AOE_DELTA,
                            EMPOWER_DAMAGE_DELTA,
                            EMPOWER_SUMMON_DELTA
                        ));
                    }
                }
            }

            if (jh.contains("selectedIndex")) {
                int sel = jh.at("selectedIndex").get<int>();
                if (sel >= 0 && sel < hand.GetCount()) {
                    hand.SelectByIndex(sel);
                }
            }
        }
    }
    catch (const SaveLoadException&) {
        throw;
    }
    catch (const nlohmann::json::exception& e) {
        throw InvalidSaveDataException(std::string("JSON error while loading: ") + e.what());
    }
    catch (const std::exception& e) {
        throw LoadException(std::string("Error while loading: ") + e.what());
    }
}
