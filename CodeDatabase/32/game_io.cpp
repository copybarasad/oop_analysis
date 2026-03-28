#include "game_io.hpp"
#include "file_raii.hpp"
#include "validator.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "spell_factory.hpp"

#include <sstream>
#include <algorithm>
#include <limits>

static bool IsNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

static int readIntChecked(std::fstream& in, const std::string& fieldName) {
    std::string token;
    if (!(in >> token))
        throw FileReadError("Ошибка чтения поля: " + fieldName);

    if (!IsNumber(token))
        throw FileReadError("Неверный формат поля '" + fieldName +
                            "': получено '" + token + "'");

    return std::stoi(token);
}


void GameSave(Game& game, const std::string& filename)
{
    FileRAII file(filename, std::ios::out | std::ios::trunc);
    std::fstream& out = file.get();

    out << "MAD_SAVE_V1\n";

    out << game.GetLevel() << "\n";

    Player& p = game.GetPlayer();
    out << p.GetHealth()       << "\n";
    out << p.GetDamage()       << "\n";
    out << p.GetX()            << "\n";
    out << p.GetY()            << "\n";
    out << (p.IsRanged() ? 1 : 0)       << "\n";
    out << (p.IsSlowed() ? 1 : 0)       << "\n";
    out << (p.HasEnhancement() ? 1 : 0) << "\n";

    Field temp = game.GetField();

    for (int y = 0; y < temp.height(); ++y)
        for (int x = 0; x < temp.width(); ++x)
            if (temp.At(x,y).type() == CellType::kPlayer ||
                temp.At(x,y).type() == CellType::kEnemy  ||
                temp.At(x,y).type() == CellType::kAlly)
                temp.At(x,y).SetType(CellType::kEmpty);

    for (auto& e : game.GetEnemies())
        if (e.IsAlive())
            temp.At(e.GetX(), e.GetY()).SetType(CellType::kEnemy);

    for (auto& a : game.GetAllies())
        if (a.IsAlive())
            temp.At(a.GetX(), a.GetY()).SetType(CellType::kAlly);

    for (auto& tr : game.GetTraps())
        temp.At(tr.GetX(), tr.GetY()).SetType(CellType::kTrap);

    for (auto& b : game.GetBuildings())
        if (!b.IsDestroyed())
            temp.At(b.GetX(), b.GetY()).SetType(CellType::kEnemyBuilding);

    for (auto& t : game.GetTowers())
        temp.At(t.GetX(), t.GetY()).SetType(CellType::kEnemyTower);

    temp.At(p.GetX(), p.GetY()).SetType(CellType::kPlayer);

    out << temp.width()  << "\n";
    out << temp.height() << "\n";

    for (int y = 0; y < temp.height(); y++) {
        for (int x = 0; x < temp.width(); x++)
            out << (int)temp.At(x, y).type() << " ";
        out << "\n";
    }

    auto& enemies = game.GetEnemies();
    out << enemies.size() << "\n";
    for (auto& e : enemies) {
        out << e.GetName()        << "\n";
        out << e.GetHealth()      << "\n";
        out << e.GetDamage()      << "\n";
        out << e.GetX()           << "\n";
        out << e.GetY()           << "\n";
        out << (e.HasAttacked() ? 1 : 0) << "\n";
    }

    auto& buildings = game.GetBuildings();
    out << buildings.size() << "\n";
    for (auto& b : buildings) {
        out << b.GetX()                  << "\n";
        out << b.GetY()                  << "\n";
        out << (b.IsDestroyed() ? 1 : 0) << "\n";
    }

    auto& towers = game.GetTowers();
    out << towers.size() << "\n";
    for (auto& t : towers) {
        out << t.GetX() << "\n";
        out << t.GetY() << "\n";
    }

    auto& traps = game.GetTraps();
    out << traps.size() << "\n";
    for (auto& tr : traps) {
        out << tr.GetX()      << "\n";
        out << tr.GetY()      << "\n";
        out << tr.GetDamage() << "\n";
    }

    auto& allies = game.GetAllies();
    out << allies.size() << "\n";
    for (auto& a : allies) {
        out << a.GetName()   << "\n";
        out << a.GetHealth() << "\n";
        out << a.GetDamage() << "\n";
        out << a.GetX()      << "\n";
        out << a.GetY()      << "\n";
    }

    Hand& hand = p.GetHand();
    out << hand.Size() << "\n";

    for (size_t i = 0; i < hand.Size(); ++i) {
        const Spell* s = hand.GetSpell(i);
        if (!s) {
            out << 0 << "\n";
            out << 0 << "\n";
            continue;
        }
        int typeId = SpellFactory::DetectTypeId(*s);
        int lvl    = hand.GetUpgradeLevel(i);
        out << typeId << "\n";
        out << lvl    << "\n";
    }
}


void GameLoad(Game& game, const std::string& filename)
{
    FileRAII file(filename, std::ios::in);
    std::fstream& in = file.get();

    std::string header;
    std::getline(in, header);
    if (header != "MAD_SAVE_V1")
        throw InvalidSaveFormat("Неверный заголовок сохранения");

    int level = readIntChecked(in, "LEVEL");
    Validator::CheckRange("LEVEL", level, 1, 999);
    game.SetLevel(level);

    int hp  = readIntChecked(in, "PlayerHP");
    int dmg = readIntChecked(in, "PlayerDMG");
    int px  = readIntChecked(in, "PlayerX");
    int py  = readIntChecked(in, "PlayerY");
    int ranged   = readIntChecked(in, "RangedFlag");
    int slowed   = readIntChecked(in, "SlowedFlag");
    int enhanced = readIntChecked(in, "EnhFlag");

    Validator::CheckRange("PlayerHP", hp, 0, 1000);
    Validator::CheckRange("PlayerDMG", dmg, 0, 500);
    Validator::CheckFlag("RangedFlag", ranged);
    Validator::CheckFlag("SlowedFlag", slowed);
    Validator::CheckFlag("EnhFlag", enhanced);

    int w = readIntChecked(in, "FIELD_W");
    int h = readIntChecked(in, "FIELD_H");

    Validator::CheckRange("FieldW", w, 5, 50);
    Validator::CheckRange("FieldH", h, 5, 50);

    game.ForceResizeField(w, h);
    Field& f = game.GetField();

    game.GetEnemies().clear();
    game.GetBuildings().clear();
    game.GetTowers().clear();
    game.GetTraps().clear();
    game.GetAllies().clear();

    bool player_found = false;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int t = readIntChecked(in, "CellType");
            Validator::CheckCellType(t);
            CellType ct = (CellType)t;
            f.At(x, y).SetType(ct);

            if (ct == CellType::kPlayer) {
                game.GetPlayer().SetPosition(x, y);
                player_found = true;
            }
        }

    if (!player_found) {
        Validator::CheckCoord("PlayerX", px, w);
        Validator::CheckCoord("PlayerY", py, h);
        game.GetPlayer().SetPosition(px, py);
        f.At(px, py).SetType(CellType::kPlayer);
    }

    int nEnemies = readIntChecked(in, "EnemiesCount");
    Validator::CheckRange("EnemiesCount", nEnemies, 0, 500);

    for (int i = 0; i < nEnemies; i++) {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string name;
        std::getline(in, name);

        int ehp   = readIntChecked(in, "EnemyHP");
        int edmg  = readIntChecked(in, "EnemyDMG");
        int ex    = readIntChecked(in, "EnemyX");
        int ey    = readIntChecked(in, "EnemyY");
        int attacked = readIntChecked(in, "EnemyFlag");

        Validator::CheckRange("EnemyHP", ehp, 0, 1000);
        Validator::CheckRange("EnemyDMG", edmg, 0, 500);
        Validator::CheckCoord("EnemyX", ex, w);
        Validator::CheckCoord("EnemyY", ey, h);
        Validator::CheckFlag("EnemyFlag", attacked);

        game.GetEnemies().emplace_back(name, ehp, edmg);
        auto& e = game.GetEnemies().back();
        e.SetPosition(ex, ey);
        e.SetAttacked(attacked != 0);

        f.At(ex, ey).SetType(CellType::kEnemy);
    }

    int nBuildings = readIntChecked(in, "BuildingsCount");
    Validator::CheckRange("BuildingsCount", nBuildings, 0, 200);

    for (int i = 0; i < nBuildings; i++) {
        int bx = readIntChecked(in, "Bx");
        int by = readIntChecked(in, "By");
        int destroyed = readIntChecked(in, "Destroyed");

        Validator::CheckCoord("Bx", bx, w);
        Validator::CheckCoord("By", by, h);
        Validator::CheckFlag("Destroyed", destroyed);

        game.GetBuildings().emplace_back(bx, by, BUILDING_SPAWN_DELAY);
        game.GetBuildings().back().SetDestroyed(destroyed != 0);

        if (!destroyed)
            f.At(bx, by).SetType(CellType::kEnemyBuilding);
    }

    int nT = readIntChecked(in, "TowersCount");
    Validator::CheckRange("TowersCount", nT, 0, 200);

    for (int i = 0; i < nT; i++) {
        int tx = readIntChecked(in, "Tx");
        int ty = readIntChecked(in, "Ty");

        Validator::CheckCoord("Tx", tx, w);
        Validator::CheckCoord("Ty", ty, h);

        game.GetTowers().emplace_back(tx, ty, &game);
        f.At(tx, ty).SetType(CellType::kEnemyTower);
    }

    int nTr = readIntChecked(in, "TrapsCount");
    Validator::CheckRange("TrapsCount", nTr, 0, 200);

    for (int i = 0; i < nTr; i++) {
        int x = readIntChecked(in, "TrX");
        int y = readIntChecked(in, "TrY");
        int d = readIntChecked(in, "TrDMG");

        Validator::CheckCoord("TrX", x, w);
        Validator::CheckCoord("TrY", y, h);
        Validator::CheckRange("TrDMG", d, 1, 500);

        game.GetTraps().emplace_back(x, y, d);
        f.At(x, y).SetType(CellType::kTrap);
    }

    int nAl = readIntChecked(in, "AlliesCount");
    Validator::CheckRange("AlliesCount", nAl, 0, 200);

    for (int i = 0; i < nAl; i++) {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string nm;
        std::getline(in, nm);

        int ah = readIntChecked(in, "AlHP");
        int ad = readIntChecked(in, "AlDMG");
        int ax = readIntChecked(in, "AlX");
        int ay = readIntChecked(in, "AlY");

        Validator::CheckRange("AlHP", ah, 0, 1000);
        Validator::CheckRange("AlDMG", ad, 0, 500);
        Validator::CheckCoord("AlX", ax, w);
        Validator::CheckCoord("AlY", ay, h);

        game.GetAllies().emplace_back(nm, ah, ad, ax, ay);
        f.At(ax, ay).SetType(CellType::kAlly);
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            CellType ct = f.At(x, y).type();

            switch (ct) {
                case CellType::kEnemy: {
                    bool exists = false;
                    for (const auto& e : game.GetEnemies()) {
                        if (e.GetX() == x && e.GetY() == y) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        game.GetEnemies().emplace_back("Гоблин", ENEMY_GOBLIN_HP, ENEMY_GOBLIN_DMG);
                        game.GetEnemies().back().SetPosition(x, y);
                    }
                } break;

                case CellType::kAlly: {
                    bool exists = false;
                    for (const auto& a : game.GetAllies()) {
                        if (a.GetX() == x && a.GetY() == y) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        game.GetAllies().emplace_back("Союзник", 30, 10, x, y);
                    }
                } break;

                case CellType::kTrap: {
                    bool exists = false;
                    for (const auto& tr : game.GetTraps()) {
                        if (tr.GetX() == x && tr.GetY() == y) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        game.GetTraps().emplace_back(x, y, 20);
                    }
                } break;

                case CellType::kEnemyBuilding: {
                    bool exists = false;
                    for (const auto& b : game.GetBuildings()) {
                        if (b.GetX() == x && b.GetY() == y) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        game.GetBuildings().emplace_back(x, y, BUILDING_SPAWN_DELAY);
                    }
                } break;

                case CellType::kEnemyTower: {
                    bool exists = false;
                    for (const auto& t : game.GetTowers()) {
                        if (t.GetX() == x && t.GetY() == y) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        game.GetTowers().emplace_back(x, y, &game);
                    }
                } break;

                default:
                    break;
            }
        }
    }

    Player& p = game.GetPlayer();
    p.SetHealth(hp);
    p.SetDamage(dmg);
    p.SetRangedMode(ranged != 0);
    p.SetSlowed(slowed != 0);
    if (enhanced) p.ActivateEnhancement();

    int nSpells = readIntChecked(in, "SpellsCount");
    Validator::CheckRange("SpellsCount", nSpells, 0, 50);

    p.GetHand().Clear();

    for (int i = 0; i < nSpells; ++i) {
        int type = readIntChecked(in, "SpellType");
        Validator::CheckRange("SpellType", type, 0, 4);

        int lvl = readIntChecked(in, "SpellLevel");
        Validator::CheckRange("SpellLevel", lvl, 0, 10);

        auto sp = SpellFactory::CreateByType(type);
        SpellFactory::ApplyUpgradeLevel(*sp, lvl);
        p.GetHand().AddSpellWithLevel(std::move(sp), lvl);
    }

    for (auto& e : game.GetEnemies()) {
        if (e.IsAlive()) {
            f.At(e.GetX(), e.GetY()).SetType(CellType::kEnemy);
        }
    }

    for (auto& a : game.GetAllies()) {
        if (a.IsAlive()) {
            f.At(a.GetX(), a.GetY()).SetType(CellType::kAlly);
        }
    }

    for (auto& tr : game.GetTraps()) {
        f.At(tr.GetX(), tr.GetY()).SetType(CellType::kTrap);
    }

    for (auto& b : game.GetBuildings()) {
        if (!b.IsDestroyed()) {
            f.At(b.GetX(), b.GetY()).SetType(CellType::kEnemyBuilding);
        }
    }

    for (auto& t : game.GetTowers()) {
        f.At(t.GetX(), t.GetY()).SetType(CellType::kEnemyTower);
    }
}