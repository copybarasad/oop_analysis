#include "gameserializer.hpp"
#include "gamemodel.hpp"  
#include "spellfactory.hpp"
#include "cell.hpp"      
#include "trap.hpp"     
#include "ally.hpp"    
#include "enemytower.hpp"
#include "enemy_building.hpp"
#include "field.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "spell.hpp"
#include "errors.hpp"

#include "gameserializer.hpp"
#include "input_json.hpp"
#include "output_json.hpp"
#include "errors.hpp"

using json = nlohmann::json;

void GameSerializer::SaveGame(Game& game, const std::string& path)
{
    json j;
    j["meta"] = {
        {"width",  game.getField().getWidth()},
        {"height", game.getField().getHeight()},
        {"level",  game.getLevel()}
    };

    j["player"] = SerializePlayer(game);
    j["field"]  = SerializeField(game);
    j["entity"] = SerializeEntity(game);

   JsonWriter out(path);
    out.Write(j);
}

void GameSerializer::LoadGame(Game& game, const std::string& path)
{
    JsonReader in(path);
    json j = in.Read();

    int width  = j.at("meta").at("width");
    int height = j.at("meta").at("height");
    int level  = j.at("meta").at("level");

    game.CreateEmptyWorld(width, height);
    game.SetLevel(level);

    DeserializeField(game, j.at("field"));
    DeserializePlayer(game, j.at("player"));
    DeserializeEntity(game, j.at("entity"));
}

json GameSerializer::SerializePlayer(Game& game)
{
    const auto& p = game.getPlayer();
    const auto& hand = p.getHand();

    json spells = json::array();
    for (const auto& sp : hand.getSpells())
        spells.push_back(sp->getName());

    return {
        {"x", p.getX()},
        {"y", p.getY()},
        {"hp", p.getHealth()},
        {"max_hp", p.getMaxHealth()},
        {"damage", p.getDamage()},
        {"score", p.getScore()},
        {"kills", p.getKills()},
        {"slowed", p.isSlowed()},
        {"ranged", p.isRanged()},
        {"hand_spells", spells},
        {"hand_stacks", hand.getEnhancementStacks()}
    };
}

json GameSerializer::SerializeField(Game& game)
{
    const auto& f = game.getField();
    json jf = json::array();

    for (int y = 0; y < f.getHeight(); ++y) {
        json row = json::array();
        for (int x = 0; x < f.getWidth(); ++x) {
            row.push_back({
                {"type", (int)f.At(x,y).getType()},
                {"id",   f.At(x,y).getEntityId()}
            });
        }
        jf.push_back(row);
    }

    return jf;
}

json GameSerializer::SerializeEntity(Game& game)
{
    json j;

    {
        json arr = json::array();
        for (const auto& e : game.getEnemies()) {
            arr.push_back({
                {"id", e.id},
                {"x",  e.x},
                {"y",  e.y},
                {"hp", e.enemy.getHealth()},
                {"dmg", e.enemy.getDamage()}
            });
        }
        j["enemies"] = arr;
    }

    {
        json arr = json::array();
        for (const auto& b : game.getBuildings()) {
            arr.push_back({
                {"x", b.getX()},
                {"y", b.getY()},
                {"hp", b.getHealth()},
                {"delay", b.getDelay()},
                {"counter", b.getCounter()}
            });
        }
        j["buildings"] = arr;
    }

    {
        json arr = json::array();
        for (const auto& t : game.getTowers()) {
            arr.push_back({
                {"x", t.getX()},
                {"y", t.getY()},
                {"hp", t.getHealth()},
                {"dmg", t.getDamage()},
                {"radius", t.getRadius()},
                {"delay", t.getDelay()},
                {"counter", t.getCounter()}
            });
        }
        j["towers"] = arr;
    }

    {
        json arr = json::array();
        for (const auto& t : game.getTraps()) {
            arr.push_back({
                {"x", t->getX()},
                {"y", t->getY()},
                {"dmg", t->getDamage()},
                {"active", t->IsActive()}
            });
        }
        j["traps"] = arr;
    }

    {
        json arr = json::array();
        for (const auto& a : game.getAllies()) {
            arr.push_back({
                {"x", a->getX()},
                {"y", a->getY()},
                {"hp", a->getHealth()},
                {"dmg", a->getDamage()}
            });
        }
        j["allies"] = arr;
    }

    return j;
}

void GameSerializer::DeserializePlayer(Game& game, const json& j)
{
    auto& p = game.getPlayer();

    p.setPosition(j.at("x"), j.at("y"));
    p.setHealth(j.at("hp"));
    p.setMaxHealth(j.at("max_hp"));
    p.setDamage(j.at("damage"));
    p.setScore(j.at("score"));
    p.setKills(j.at("kills"));
    p.setSlowed(j.at("slowed"));
    p.setRanged(j.at("ranged"));

    game.getField().At(p.getX(), p.getY()).set(CellType::kPlayer);

    Hand& hand = p.getHand();
    hand.Clear();

    SpellFactory sf;

    for (auto& spName : j.at("hand_spells")) {
        SpellType type = SpellFactory::FromString(spName.get<std::string>());
        hand.AddSpell(sf.CreateSpell(type));
    }

    hand.SetEnhancementStacks(j.at("hand_stacks"));
}

void GameSerializer::DeserializeField(Game& game, const json& j)
{
    auto& f = game.getField();

    for (int y = 0; y < f.getHeight(); ++y)
        for (int x = 0; x < f.getWidth(); ++x)
            f.At(x,y).set(
                (CellType)j[y][x]["type"].get<int>(),
                j[y][x]["id"].get<int>()
            );
}

void GameSerializer::DeserializeEntity(Game& game, const json& j)
{
    auto& f = game.getField();

    game.getEnemies().clear();
    int maxId = 0;

    for (auto& e : j.at("enemies")) {
        int id = e.at("id");
        int x  = e.at("x");
        int y  = e.at("y");
        int hp = e.at("hp");
        int dmg = e.at("dmg");

        game.getEnemies().push_back({id, Enemy(hp, dmg), x, y});
        f.At(x,y).set(CellType::kEnemy, id);

        if (id > maxId) maxId = id;
    }
    game.getNextEnemyId() = maxId + 1;

    game.getBuildings().clear();
    for (auto& b : j.at("buildings")) {
        int x=b["x"], y=b["y"], hp=b["hp"], delay=b["delay"], c=b["counter"];
        game.getBuildings().emplace_back(x,y,delay);
        game.getBuildings().back().setHealth(hp);
        game.getBuildings().back().setCounter(c);
        f.At(x,y).set(CellType::kBuilding);
    }

    game.getTowers().clear();
    for (auto& t : j.at("towers")) {
        int x=t["x"], y=t["y"], hp=t["hp"], dmg=t["dmg"];
        int rad=t["radius"], delay=t["delay"], c=t["counter"];

        game.getTowers().emplace_back(x,y,hp,dmg,rad,delay);
        game.getTowers().back().setCounter(c);
        f.At(x,y).set(CellType::kTower);
    }

    game.getTraps().clear();
    for (auto& t : j.at("traps")) {
        int x=t["x"], y=t["y"], dmg=t["dmg"];
        bool active=t["active"];

        auto trap = std::make_unique<Trap>(x,y,dmg);
        if (!active) trap->Deactivate();

        f.At(x,y).set(CellType::kTrap);
        game.getTraps().push_back(std::move(trap));
    }

    game.getAllies().clear();
    for (auto& a : j.at("allies")) {
        int x=a["x"], y=a["y"], hp=a["hp"], dmg=a["dmg"];

        game.getAllies().push_back(std::make_unique<Ally>(x,y,hp,dmg));
        f.At(x,y).set(CellType::kAlly);
    }
}
