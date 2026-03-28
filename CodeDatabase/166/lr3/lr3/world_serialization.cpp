#include "world.hpp"

#include <fstream>
#include <limits>
#include <memory>
#include <sstream>
#include <utility>

#include "game_errors.hpp"
#include "spells/aoe_damage_spell.hpp"
#include "spells/direct_damage_spell.hpp"
#include "spells/enhance_spell.hpp"
#include "spells/summon_spell.hpp"
#include "spells/trap_spell.hpp"

namespace {
std::unique_ptr<Spell> makeSpellByName(const std::string& n) {
    if (n == "direct")  return std::unique_ptr<Spell>(new DirectDamageSpell(4,3));
    if (n == "aoe")     return std::unique_ptr<Spell>(new AoEDamageSpell(3,4));
    if (n == "trap")    return std::unique_ptr<Spell>(new TrapSpell(6));
    if (n == "summon")  return std::unique_ptr<Spell>(new SummonSpell(2, 6, 2));
    if (n == "enhance") return std::unique_ptr<Spell>(new EnhanceSpell());
    return nullptr;
}
} // namespace

void save_world_to(const World& w, const std::string& path) {
    std::ofstream out(path, std::ios::trunc);
    if (!out) throw FileOpenError("save_world_to: cannot open file: " + path);

    out << w.hand.capacity() << '\n';
    for (std::size_t i = 0; i < w.hand.capacity(); ++i) {
        if (w.hand.has(i)) out << w.hand.cardName(i) << '\n';
        else               out << "-" << '\n';
    }

    out << w.level << ' ' << (w.baseHeroHp > 0 ? w.baseHeroHp : w.hero.getHp()) << '\n';

    out << w.field.getWidth() << ' ' << w.field.getHeight() << '\n';
    for (int y = 0; y < w.field.getHeight(); ++y) {
        for (int x = 0; x < w.field.getWidth(); ++x) {
            auto t = w.field.at(x, y).getType();
            int v = (t == CellType::Empty ? 0 : (t == CellType::Wall ? 1 : 2));
            out << v << (x + 1 < w.field.getWidth() ? ' ' : '\n');
        }
    }

    Position hp = w.hero.getPosition();
    out << w.hero.getHp() << ' ' << w.hero.getDmg() << ' ' << hp.getX() << ' ' << hp.getY() << '\n';

    out << w.kills << ' ' << w.awardsGiven << '\n';

    out << w.boosts.directRadiusPlus << ' ' << w.boosts.aoeSizePlus << ' ' << w.boosts.trapDamagePlus
        << ' ' << w.boosts.summonCountPlus << ' ' << w.boosts.stacks << '\n';

    out << w.enemies.size() << '\n';
    for (const auto& e : w.enemies) {
        auto p = e.getPosition();
        out << e.getHp() << ' ' << e.getDmg() << ' ' << p.getX() << ' ' << p.getY() << '\n';
    }

    out << w.spawners.size() << '\n';
    for (const auto& s : w.spawners) {
        auto p = s.getPos();
        out << p.getX() << ' ' << p.getY() << ' ' << s.getPeriod() << ' ' << s.getHp() << '\n';
    }

    out << w.traps.size() << '\n';
    for (const auto& t : w.traps) {
        out << t.getPos().getX() << ' ' << t.getPos().getY() << ' ' << t.getDamage() << '\n';
    }

    out << w.towers.size() << '\n';
    for (const auto& t : w.towers) {
        out << t.getPos().getX() << ' ' << t.getPos().getY() << ' ' << t.getDamage() << ' ' << t.getAttackRadius() << ' ' << t.getCooldown() << '\n';
    }

    out << w.allies.size() << '\n';
    for (const auto& a : w.allies) {
        out << a.getPos().getX() << ' ' << a.getPos().getY() << ' ' << a.getHp() << ' ' << a.getDmg() << '\n';
    }
}

World load_world_from(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw FileOpenError("load_world_from: cannot open file: " + path);

    std::size_t handCap;
    if (!(in >> handCap)) throw ParseError("load_world_from: invalid hand capacity in " + path);
    Hand hand(handCap ? handCap : 5);
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (std::size_t i = 0; i < hand.capacity(); ++i) {
        std::string name;
        std::getline(in, name);
        if (!in) throw std::runtime_error("invalid hand entry");
        if (name == "-" || name.empty() || name == "(пусто)") continue;
        if (auto sp = makeSpellByName(name)) { (void)hand.put(std::move(sp)); }
    }

    int level, baseHp;
    if (!(in >> level >> baseHp)) throw std::runtime_error("invalid level/baseHp");
    if (level < 1) level = 1;

    int W, H;
    if (!(in >> W >> H)) throw ParseError("load_world_from: invalid field size in " + path);
    Field fld(W, H);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            int v; if (!(in >> v)) throw std::runtime_error("invalid field cell");
            CellType t = (v == 0 ? CellType::Empty : (v == 1 ? CellType::Wall : CellType::Slow));
            fld.at(x, y).setType(t);
        }
    }

    int hp, dmg, hx, hy;
    if (!(in >> hp >> dmg >> hx >> hy)) throw std::runtime_error("invalid hero");
    Hero hero(hp, dmg, {hx, hy});

    int kills, awards;
    if (!(in >> kills >> awards)) throw std::runtime_error("invalid stats");

    BoostState boosts;
    if (!(in >> boosts.directRadiusPlus >> boosts.aoeSizePlus >> boosts.trapDamagePlus
             >> boosts.summonCountPlus >> boosts.stacks)) throw std::runtime_error("invalid boosts");

    std::size_t nEnemies; if (!(in >> nEnemies)) throw std::runtime_error("invalid enemy count");
    std::vector<Enemy> es; es.reserve(nEnemies);
    for (std::size_t i = 0; i < nEnemies; ++i) {
        int ehp, edmg, ex, ey; if (!(in >> ehp >> edmg >> ex >> ey)) throw std::runtime_error("invalid enemy");
        es.emplace_back(ehp, edmg, Position(ex, ey));
    }

    std::size_t nSp; if (!(in >> nSp)) throw std::runtime_error("invalid spawner count");
    std::vector<Spawner> sp; sp.reserve(nSp);
    for (std::size_t i = 0; i < nSp; ++i) {
        int x, y, period, shp;
        if (!(in >> x >> y >> period >> shp)) throw std::runtime_error("invalid spawner");
        sp.emplace_back(Position(x, y), period, shp);
    }

    std::size_t nTr; if (!(in >> nTr)) throw std::runtime_error("invalid trap count");
    std::vector<Trap> tr; tr.reserve(nTr);
    for (std::size_t i = 0; i < nTr; ++i) {
        int x, y, d; if (!(in >> x >> y >> d)) throw std::runtime_error("invalid trap");
        tr.push_back(Trap{Position(x, y), d});
    }

    std::size_t nTw; if (!(in >> nTw)) throw std::runtime_error("invalid tower count");
    std::vector<Tower> tw; tw.reserve(nTw);
    for (std::size_t i = 0; i < nTw; ++i) {
        int x, y, d, r, c; if (!(in >> x >> y >> d >> r >> c)) throw std::runtime_error("invalid tower");
        tw.emplace_back(Position(x, y), d, r); tw.back().setCooldown(c);
    }

    std::size_t nAl; if (!(in >> nAl)) throw std::runtime_error("invalid ally count");
    std::vector<Ally> al; al.reserve(nAl);
    for (std::size_t i = 0; i < nAl; ++i) {
        int x, y, hp_, dmg_;
        if (!(in >> x >> y >> hp_ >> dmg_)) throw std::runtime_error("invalid ally");
        al.push_back(Ally{Position(x, y), hp_, dmg_});
    }

    World w(std::move(fld), std::move(hero), std::move(es), std::move(sp), hand.capacity());
    w.kills = kills;
    w.awardsGiven = awards;
    w.boosts = boosts;
    w.traps = std::move(tr);
    w.towers = std::move(tw);
    w.allies = std::move(al);
    w.hand = std::move(hand);
    w.level = level;
    w.baseHeroHp = (baseHp > 0 ? baseHp : w.hero.getHp());

    return w;
}

