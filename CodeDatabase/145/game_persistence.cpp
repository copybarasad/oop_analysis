#include "game_persistence.h"
#include "game.h"
#include "exceptions.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include <iostream>
#include <sstream>

void GamePersistence::saveToStream(const Game& game, std::ostream& out) {
    out << "OOP_GAME_V1\n";
    out << game.field.getWidth() << ' ' << game.field.getHeight() << '\n';
    auto ppos = game.player.getPosition();
    out << game.player.getHealth() << ' ' << game.player.getMaxHealth() << ' ' << game.player.getDamage() << ' ' << game.player.getScore()
        << ' ' << ppos.first << ' ' << ppos.second << ' ' << (int)game.player.getCombatMode() << ' '
        << game.playerSkippingTurn << ' ' << game.playerTurnCounter << ' '
        << game.player.getMeleeDamage() << ' ' << game.player.getRangedDamage() << '\n';
    out << game.currentLevel << '\n';

    for (size_t y = 0; y < game.field.getHeight(); ++y) {
        for (size_t x = 0; x < game.field.getWidth(); ++x) {
            if (!game.field.isPassable((int)x, (int)y)) out << (int)CellType::IMPASSABLE;
            else if (game.field.isSlowing((int)x, (int)y)) out << (int)CellType::SLOWING;
            else out << (int)CellType::EMPTY;
            if (x + 1 < game.field.getWidth()) out << ' ';
        }
        out << '\n';
    }

    const auto& traps = game.field.getTraps();
    out << traps.size() << '\n';
    for (const auto& t : traps) {
        out << t.x << ' ' << t.y << ' ' << t.damage << '\n';
    }

    out << game.enemies.size() << '\n';
    for (const auto& e : game.enemies) {
        auto pos = e.getPosition();
        out << e.getHealth() << ' ' << e.getDamage() << ' ' << pos.first << ' ' << pos.second << '\n';
    }

    out << game.buildings.size() << '\n';
    for (const auto& b : game.buildings) {
        auto pos = b.getPosition();
        out << b.getHealth() << ' ' << pos.first << ' ' << pos.second << ' '
            << b.getSpawnInterval() << ' ' << b.getSpawnCounter() << '\n';
    }

    out << game.towers.size() << '\n';
    for (const auto& t : game.towers) {
        auto pos = t.getPosition();
        out << t.getHealth() << ' ' << pos.first << ' ' << pos.second << ' '
            << t.getAttackRadius() << ' ' << t.getSpellDamage() << ' ' << t.getCooldownCounter() << '\n';
    }

    out << game.allies.size() << '\n';
    for (const auto& a : game.allies) {
        auto pos = a.getPosition();
        out << a.getHealth() << ' ' << a.getDamage() << ' ' << pos.first << ' ' << pos.second << '\n';
    }

    size_t handMax = game.spellManager ? game.spellManager->getHand().getMaxSize() : 3;
    out << handMax << '\n';
    int enh = game.enhancementLevel;
    out << enh << '\n';
    const auto& spells = game.spellManager->getHand().getSpells();
    out << spells.size() << '\n';
    for (const auto& sp : spells) {
        if (!sp) { out << "None\n"; continue; }
        const char* name = sp->getName();
        out << sp->getSerializedData() << '\n';
    }
}

void GamePersistence::loadFromStream(Game& game, std::istream& in) {
    std::string header;
    std::getline(in, header);
    if (header != "OOP_GAME_V1") {
        throw ParseException(std::string("Unsupported save format or corrupt header: found '") + header + "'");
    }

    size_t fw, fh;
    in >> fw >> fh;
    if (!in) throw ParseException("Unexpected end of file while reading field size");
    game.field = GameField(fw, fh);

    int pHealth, pMax, pDamage, pScore, px, py, pMode, pSkip, pTurn;
    int pMeleeDmg = 15, pRangedDmg = 10;
    in >> pHealth >> pMax >> pDamage >> pScore >> px >> py >> pMode >> pSkip >> pTurn;
    if (!in) throw ParseException("Unexpected end of file while reading player data");

    in >> pMeleeDmg >> pRangedDmg;
    if (!in) {
        in.clear();
        pMeleeDmg = 15;
        pRangedDmg = 10;
    }

    int lvl;
    in >> lvl;
    if (!in) throw ParseException("Unexpected end of file while reading current level");
    game.currentLevel = lvl;
    game.player.setPosition(px, py);
    game.player.setHealth(pHealth);
    game.player.setMaxHealth(pMax);
    if ((int)game.player.getCombatMode() != pMode) game.player.switchCombatMode();
    game.playerTurnCounter = pTurn;
    game.playerSkippingTurn = pSkip;
    game.player.increaseMeleeDamage(pMeleeDmg - 15);
    game.player.increaseRangedDamage(pRangedDmg - 10);

    for (size_t y = 0; y < fh; ++y) {
        for (size_t x = 0; x < fw; ++x) {
            int t;
            in >> t;
            if (!in) throw ParseException("Unexpected end of file while reading grid cell data");
            game.field.setCell((int)x, (int)y, static_cast<CellType>(t));
        }
    }

    size_t trapCount;
    in >> trapCount;
    if (!in) throw ParseException("Unexpected end of file while reading traps count");
    for (size_t i = 0; i < trapCount; ++i) {
        int tx, ty, td;
        in >> tx >> ty >> td;
        if (!in) throw ParseException("Unexpected end of file while reading trap data");
        game.field.placeTrap(tx, ty, td);
    }

    size_t ecount;
    in >> ecount;
    if (!in) throw ParseException("Unexpected end of file while reading enemies count");
    game.enemies.clear();
    for (size_t i = 0; i < ecount; ++i) {
        int h, d, ex, ey;
        in >> h >> d >> ex >> ey;
        if (!in) throw ParseException("Unexpected end of file while reading enemy data");
        game.enemies.emplace_back(h, d, ex, ey);
        game.enemies.back().setHealth(h);
    }

    size_t bcount;
    in >> bcount;
    if (!in) throw ParseException("Unexpected end of file while reading buildings count");
    game.buildings.clear();
    for (size_t i = 0; i < bcount; ++i) {
        int h, bx, by, interval, counter;
        in >> h >> bx >> by >> interval >> counter;
        if (!in) throw ParseException("Unexpected end of file while reading building data");
        game.buildings.emplace_back(bx, by, interval);
        game.buildings.back().setHealth(h);
        game.buildings.back().setSpawnCounter(counter);
    }

    size_t tcount;
    in >> tcount;
    if (!in) throw ParseException("Unexpected end of file while reading towers count");
    game.towers.clear();
    for (size_t i = 0; i < tcount; ++i) {
        int h, tx, ty, ar, sd, cd;
        in >> h >> tx >> ty >> ar >> sd >> cd;
        if (!in) throw ParseException("Unexpected end of file while reading tower data");
        game.towers.emplace_back(tx, ty, ar, sd, 2);
        game.towers.back().setHealth(h);
        game.towers.back().setCooldownCounter(cd);
    }

    size_t acount;
    in >> acount;
    if (!in) throw ParseException("Unexpected end of file while reading allies count");
    game.allies.clear();
    for (size_t i = 0; i < acount; ++i) {
        int h, d, ax, ay;
        in >> h >> d >> ax >> ay;
        if (!in) throw ParseException("Unexpected end of file while reading ally data");
        game.allies.emplace_back(h, d, ax, ay);
        game.allies.back().setHealth(h);
    }

    size_t handMax;
    in >> handMax;
    if (!in) throw ParseException("Unexpected end of file while reading hand max size");
    int enhLevel;
    in >> enhLevel;
    if (!in) throw ParseException("Unexpected end of file while reading enhancement level");
    size_t spellCount;
    in >> spellCount;
    if (!in) throw ParseException("Unexpected end of file while reading spell count");

    game.spellManager = std::make_unique<SpellManager>(handMax);
    std::string rest;
    std::getline(in, rest);
    for (size_t i = 0; i < spellCount; ++i) {
        std::string line;
        std::getline(in, line);
        if (line.empty()) { --i; continue; }
        std::istringstream ss(line);
        std::string sname;
        ss >> sname;
        if (sname == "Direct" || sname == "Direct Damage") {
            int dmg, rad;
            if (ss >> dmg >> rad) {
                game.spellManager->addSpellToHand(std::make_unique<DirectDamageSpell>(dmg, rad));
            } else {
                game.spellManager->addSpellToHand(std::make_unique<DirectDamageSpell>());
            }
        } else if (sname == "Area" || sname == "Area Damage") {
            int dmg, rad, area;
            if (ss >> dmg >> rad >> area) {
                game.spellManager->addSpellToHand(std::make_unique<AreaDamageSpell>(dmg, rad, area));
            } else {
                game.spellManager->addSpellToHand(std::make_unique<AreaDamageSpell>());
            }
        } else if (sname == "Trap") {
            int dmg;
            if (ss >> dmg) game.spellManager->addSpellToHand(std::make_unique<TrapSpell>(dmg));
            else game.spellManager->addSpellToHand(std::make_unique<TrapSpell>());
        } else if (sname == "Summon") {
            int cnt;
            if (ss >> cnt) game.spellManager->addSpellToHand(std::make_unique<SummonSpell>(cnt));
            else game.spellManager->addSpellToHand(std::make_unique<SummonSpell>());
        } else if (sname == "Enhancement") {
            game.spellManager->addSpellToHand(std::make_unique<EnhancementSpell>());
        } else if (sname == "None") {
        } else {
            if (sname.find("Direct") != std::string::npos) game.spellManager->addSpellToHand(std::make_unique<DirectDamageSpell>());
            else if (sname.find("Area") != std::string::npos) game.spellManager->addSpellToHand(std::make_unique<AreaDamageSpell>());
            else if (sname.find("Trap") != std::string::npos) game.spellManager->addSpellToHand(std::make_unique<TrapSpell>());
            else if (sname.find("Summon") != std::string::npos) game.spellManager->addSpellToHand(std::make_unique<SummonSpell>());
            else game.spellManager->addSpellToHand(std::make_unique<DirectDamageSpell>());
        }
    }
    game.enhancementLevel = enhLevel;
}