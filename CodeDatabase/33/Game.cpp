#include "Game.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <memory>
#include <random>
#include <chrono>
#include "Exceptions.h"
#include "SaveManager.h"
#include "Spell.h"
#include "GameEvent.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BoostSpell.h"
#define LIMIT 5000
#define SPELL_REWARD_INTERVAL 3
#define RANGE 3
#define INIT_DIST 1000000

Game::Balance Game::MakeBalance(Difficulty d) {
    switch (d) {
        case Difficulty::Easy: return { 100, 6, 4 };
        case Difficulty::Normal: return { 80, 5, 3 };
        case Difficulty::Hard: return { 50, 5, 3 };
        case Difficulty::Nightmare: return { 30, 6, 4 };
    }
    return {24,5,3};
}

Field Game::CreateFieldChecked(int w, int h, std::mt19937& rng) {
    if (w < Field::kMin) w = Field::kMin;
    if (h < Field::kMin) h = Field::kMin;
    if (w > Field::kMax) w = Field::kMax;
    if (h > Field::kMax) h = Field::kMax;
    return Field(w, h, rng);
}

Game::Game(const LevelConfig& cfg, Difficulty diff, std::size_t hand_capacity)
    : diff_(diff),
      hand_(hand_capacity) {
    ReconfigureFrom(cfg);
}

bool Game::IsEnemyAt(Coord p) const {
    for (const auto& e : enemies_) {
        if (e.IsAlive() && e.Pos() == p) return true;
    }
    return false;
}

int Game::EnemyIndexAt(Coord p) const {
    for (int i=0; i<(int)enemies_.size(); ++i) {
        if (enemies_[i].IsAlive() && enemies_[i].Pos()==p) return i;
    }
    return -1;
}

bool Game::IsAllyAt(Coord p) const {
    for (const auto& a : allies_) {
        if (a.IsAlive() && a.Pos() == p) return true;
    }
    return false;
}

int Game::AllyIndexAt(Coord p) const {
    for (int i=0; i<(int)allies_.size(); ++i) {
        if (allies_[i].IsAlive() && allies_[i].Pos()==p) return i;
    }
    return -1;
}

bool Game::IsTowerAt(Coord p) const {
    for (const auto& t : towers_) {
        if (t.Pos() == p) return true;
    }
    return false;
}

int Game::TowerIndexAt(Coord p) const {
    for (int i=0; i<(int)towers_.size(); ++i) {
        if (towers_[i].Pos() == p) return i;
    }
    return -1;
}

bool Game::IsTurretAt(Coord p) const {
    for (const auto& r : turrets_) {
        if (r.IsAlive() && r.Pos() == p) return true;
    }
    return false;
}

int Game::TurretIndexAt(Coord p) const {
    for (int i=0; i<(int)turrets_.size(); ++i) {
        if (turrets_[i].IsAlive() && turrets_[i].Pos()==p) return i;
    }
    return -1;
}

bool Game::IsTrapAt(Coord p) const {
    for (const auto& tr : traps_) {
        if (tr.Pos() == p) return true;
    }
    return false;
}

int Game::TrapIndexAt(Coord p) const {
    for (int i=0; i<(int)traps_.size(); ++i) {
        if (traps_[i].Pos() == p) return i;
    }
    return -1;
}

void Game::RemoveTrapByIndex(int trap_idx) {
    if (trap_idx < 0 || trap_idx >= (int)traps_.size()) return;
    traps_.erase(traps_.begin() + trap_idx);
}

bool Game::IsBlocked(const Coord& p) const {
    return p == player_.Pos() || IsEnemyAt(p) || IsTowerAt(p) || IsTurretAt(p) || IsAllyAt(p);
}

void Game::PlacePlayerAndEnemies() {
    enemies_.clear();
    enemies_.reserve(init_enemies_);

    for (int i=0; i<init_enemies_; ++i) {
        Coord p = field_.RandomPassable(rng_);
        int guard = 0;
        while (IsBlocked(p) && guard++ < LIMIT) {
            p = field_.RandomPassable(rng_);
        }

        enemies_.emplace_back(enemy_hp_, enemy_damage_, p);
    }
}

void Game::PlaceTowers() {
    towers_.clear();
    towers_.reserve(init_towers_);

    for (int i=0; i<init_towers_; ++i) {
        Coord p = field_.RandomPassable(rng_);
        int guard = 0;
        while (IsBlocked(p) && guard++ < LIMIT) {
            p = field_.RandomPassable(rng_);
        }

        towers_.emplace_back(p, tower_interval_);
    }
}

void Game::PlaceTurrets() {
    turrets_.clear();
    turrets_.reserve(init_turrets_);

    for (int i=0; i<init_turrets_; ++i) {
        Coord p = field_.RandomPassable(rng_);
        int guard = 0;
        while (IsBlocked(p) && guard++ < LIMIT) {
            p = field_.RandomPassable(rng_);
        }

        turrets_.emplace_back(
            p,
            turret_hp_,
            turret_damage_,
            turret_range_,
            turret_cooldown_
        );
    }
}

void Game::NewGame(const std::string& player_name, std::size_t hand_capacity) {
    std::random_device rd;
    auto now = static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    std::seed_seq seq{ rd(), rd(), rd(), rd(), now };
    rng_.seed(seq);
    std::string name = player_name.empty() ? std::string("Player") : player_name;
    const auto b = MakeBalance(diff_);
    player_ = Player{name, b.player_hp, b.player_melee, b.player_ranged, Coord{0,0}};
    enemies_.clear(); allies_.clear(); turrets_.clear(); towers_.clear(); traps_.clear();
    hand_ = PlayerHand(hand_capacity);
    hand_.AddRandomSpell(rng_);
    current_level_index_ = 1;
    last_victory_ = false;
    quit_requested_ = false;
}

void Game::ReconfigureFrom(const LevelConfig& cfg) {
    w_              = cfg.width;
    h_              = cfg.height;

    init_enemies_   = cfg.enemies;
    enemy_hp_       = cfg.enemy_hp;
    enemy_damage_   = cfg.enemy_damage;

    init_towers_    = cfg.towers;
    tower_interval_ = cfg.tower_interval;

    init_turrets_    = cfg.turrets;
    turret_hp_       = cfg.turret_hp;
    turret_damage_   = cfg.turret_damage;
    turret_range_    = cfg.turret_range;
    turret_cooldown_ = cfg.turret_cooldown;
}

void Game::StartLevel() {
    last_victory_   = false;
    quit_requested_ = false;

    field_ = CreateFieldChecked(w_, h_, rng_);
    move_locked_next_turn_ = false;

    allies_.clear();
    traps_.clear();

    player_.SetPos(field_.RandomPassable(rng_));

    PlacePlayerAndEnemies();
    PlaceTowers();
    PlaceTurrets();
}

void Game::SetEventBus(GameEventBus* bus) {
    event_bus_ = bus;
}

const Field& Game::GetField() const { return field_; }
const Player& Game::GetPlayer() const { return player_; }
const std::vector<Enemy>& Game::Enemies() const { return enemies_; }
const std::vector<Ally>& Game::Allies() const { return allies_; }
const std::vector<Tower>& Game::Towers() const { return towers_; }
const std::vector<Turret>& Game::Turrets() const { return turrets_; }
const std::vector<Trap>& Game::Traps() const { return traps_; }
const PlayerHand& Game::GetHand() const { return hand_; }
bool Game::MoveLockedNextTurn() const { return move_locked_next_turn_; }
bool Game::QuitRequested() const { return quit_requested_; }

void Game::RequestQuit() { quit_requested_ = true; }
void Game::ResetLevelFlags() {
    quit_requested_ = false;
    last_victory_ = false;
}
void Game::MarkVictory() { last_victory_ = true; }

void Game::ToggleMode() {
    player_.ToggleMode();
    Notify({
        GameEventType::ModeChanged,
        player_.Name(),
        "",
        player_.Pos(),
        0,
        player_.CurrentMode()==Player::Mode::Melee ? "MELEE" : "RANGED"
    });
    std::cout << "Switched to "
              << (player_.CurrentMode()==Player::Mode::Melee ? "MELEE" : "RANGED")
              << ".\n";
}

bool Game::CastSpellByIndex(int idx) {
    bool used = hand_.CastSpellByIndex(idx, *this);
    if (used) {
        Notify({GameEventType::SpellCast, player_.Name(), "", player_.Pos(), 0, "index " + std::to_string(idx)});
        return true;
    }
    std::cout << "Spell not applied. Choose action again.\n";
    return false;
}

void Game::Notify(const GameEvent& event) {
    if (event_bus_) {
        event_bus_->Publish(event);
    }
}


bool Game::LastLevelWasVictory() const { return last_victory_; }
int  Game::CurrentLevelIndex() const { return current_level_index_; }
void Game::SetCurrentLevelIndex(int idx) { current_level_index_ = idx; }

void Game::HealPlayerFull() {
   player_.HealFull();
}

void Game::HalveHandRandom() {
    hand_.RemoveHalfRandom(rng_);
}

void Game::ApplyUpgradeChoice(int id) {
    int kUpd = current_level_index_*2;
    switch (id) {
        case 1: {
            constexpr int kHpUp = 5;
            player_.SetBaseHP(player_.BaseHP() + kHpUp*kUpd);
            break;
        }
        case 2: {
            int kMelUp = 1;
            player_.SetMelee(player_.MeleeDamage() + kMelUp*kUpd);
            break;
        }
        case 3: {
            int kRanUp = 1;
            player_.SetRanged(player_.RangedDamage() + kRanUp*kUpd);
            break;
        }
        case 4: {
            hand_.IncreaseCapacity(1);
            break;
        }
        default: break;
    }
}

void Game::SaveToStream(std::ostream& os) const {
    os << "LEVEL " << current_level_index_ << "\n";
    os << "DIFF "  << static_cast<int>(diff_) << "\n";

    os << "PLAYER "
       << player_.Name()    << ' '
       << player_.HP()      << ' '
       << player_.BaseHP()  << ' '
       << player_.Score()   << ' '
       << player_.Pos().X() << ' '
       << player_.Pos().Y() << ' '
       << (player_.CurrentMode()==Player::Mode::Melee ? 0 : 1) << ' '
       << player_.MeleeDamage()  << ' '
       << player_.RangedDamage() << '\n';

    os << "FIELD " << field_.W() << " " << field_.H() << "\n";
    os << "CELLS\n";
    for (int y = 0; y < field_.H(); ++y) {
        for (int x = 0; x < field_.W(); ++x) {
            os << static_cast<int>(field_.At({x,y}).Type()) << (x+1==field_.W()?'\n':' ');
        }
    }

    os << "ENEMYBASE " << enemy_hp_ << ' ' << enemy_damage_ << "\n";

    os << "ENEMIES " << enemies_.size() << "\n";
    for (auto& e : enemies_) os << e.HP() << " " << e.Damage() << " " << e.Pos().X() << " " << e.Pos().Y() << "\n";

    os << "ALLIES " << allies_.size() << "\n";
    for (auto& a : allies_)  os << a.HP() << " " << a.Damage() << " " << a.Pos().X() << " " << a.Pos().Y() << "\n";

    os << "TURRETS " << turrets_.size() << "\n";
    for (const auto& t : turrets_) {
        os << t.Pos().X() << ' ' << t.Pos().Y() << ' '
        << t.HP()      << ' ' << t.Damage()   << ' '
        << t.Range()   << ' ' << t.Cooldown() << ' '
        << t.CooldownTimer()
        << "\n";
    }

    os << "TOWERS " << towers_.size() << "\n";
    for (const auto& t : towers_) {
        os << t.Pos().X() << ' ' << t.Pos().Y() << ' '
        << t.Interval() << ' ' << t.TicksLeft() << '\n';
    }

    os << "TRAPS " << traps_.size() << "\n";
    for (auto& tr : traps_) os << tr.Pos().X() << " " << tr.Pos().Y() << " " << tr.Damage() << "\n";

    os << "HAND " << hand_.Capacity() << " " << hand_.Size() << "\n";
    for (const auto& sp : hand_.Spells()) os << sp->Name() << "\n";

    os << "END\n";
}

void Game::LoadFromStream(std::istream& is) {
    enemies_.clear(); allies_.clear(); turrets_.clear(); towers_.clear(); traps_.clear();

    std::string tag;
    if (!(is >> tag) || tag != "LEVEL") throw ParseError("expected LEVEL");
    is >> current_level_index_; if (!is) throw ParseError("bad LEVEL");

    if (!(is >> tag) || tag != "DIFF") throw ParseError("expected DIFF");
    int di = 0; is >> di; if (!is) throw ParseError("bad DIFF");
    diff_ = static_cast<Difficulty>(di);

    if (!(is >> tag) || tag != "PLAYER") throw ParseError("expected PLAYER");
    std::string name;
    int hp=0, basehp=0, score=0, px=0, py=0, mode=0, mele=0, rang=0;
    is >> name >> hp >> basehp >> score >> px >> py >> mode >> mele >> rang;
    if (!is) throw ParseError("bad PLAYER");
    player_.SetName(name);
    player_.SetPos({px,py});
    if (mode == 0 && player_.CurrentMode()!=Player::Mode::Melee) player_.ToggleMode();
    if (mode == 1 && player_.CurrentMode()==Player::Mode::Melee) player_.ToggleMode();
    player_.SetMelee(mele);
    player_.SetRanged(rang);
    player_.SetBaseHP(basehp);
    player_.SetHP(hp);
    player_.SetScore(score);

    if (!(is >> tag) || tag != "FIELD") throw ParseError("expected FIELD");
    int w=0,h=0; is >> w >> h; if (!is) throw ParseError("bad FIELD");
    w_ = w;
    h_ = h;
    field_ = CreateFieldChecked(w, h, rng_);

    if (!(is >> tag) || tag != "CELLS") throw ParseError("expected CELLS");
    for (int y=0; y<h; ++y) {
        for (int x=0; x<w; ++x) {
            int t; is >> t; if (!is) throw ParseError("bad CELLS");
            field_.At({x,y}).SetType(static_cast<Cell::CellType>(t));
        }
    }

    auto read_list = [&](const char* key, auto inserter){
        if (!(is >> tag) || tag != key) throw ParseError(std::string("expected ")+key);
        int n=0; is >> n; if (!is) throw ParseError(std::string("bad ")+key);
        for (int i=0;i<n;++i) inserter();
    };

    if (!(is >> tag) || tag != "ENEMYBASE") throw ParseError("expected ENEMYBASE");
    is >> enemy_hp_ >> enemy_damage_;

    read_list("ENEMIES", [&]{
        int eh,ed,x,y; is >> eh >> ed >> x >> y; if (!is) throw ParseError("enemy row");
        enemies_.emplace_back(eh, ed, Coord{x,y});
    });
    read_list("ALLIES", [&]{
        int ah,ad,x,y; is >> ah >> ad >> x >> y; if (!is) throw ParseError("ally row");
        allies_.emplace_back(ah, ad, Coord{x,y});
    });
    read_list("TURRETS", [&]{
    int x, y, th, td, range, cd, timer;
    if (!(is >> x >> y >> th >> td >> range >> cd >> timer)) {
        throw ParseError("turret row (expected: x y hp dmg range cd timer)");
    }
    turrets_.emplace_back(Coord{x,y}, th, td, range, cd);
    turrets_.back().SetCooldownTimer(timer);
    });
    read_list("TOWERS", [&]{
    int x, y, itv, left;
    is >> x >> y >> itv >> left;
    if (!is) throw ParseError("tower row");
    towers_.emplace_back(Coord{x,y}, itv);
    towers_.back().SetTicksLeft(left);
    });
    read_list("TRAPS", [&]{
        int x,y,d; is >> x >> y >> d; if (!is) throw ParseError("trap row");
        traps_.emplace_back(Coord{x,y}, d);
    });

    if (!(is >> tag) || tag != "HAND") throw ParseError("expected HAND");
    std::size_t cap=0, sz=0; is >> cap >> sz; if (!is) throw ParseError("bad HAND");
    hand_ = PlayerHand(cap);
    std::string line; std::getline(is, line);
    for (std::size_t i=0;i<sz;++i) {
        std::string name_line;
        std::getline(is, name_line); if (!is) throw ParseError("hand row");
        if (name_line == "Direct Damage") hand_.AddSpell(std::unique_ptr<Spell>(new DirectDamageSpell(5,3)));
        else if (name_line == "Area Damage") hand_.AddSpell(std::unique_ptr<Spell>(new AreaDamageSpell(4,2)));
        else if (name_line == "Trap") hand_.AddSpell(std::unique_ptr<Spell>(new TrapSpell(4)));
        else if (name_line == "Summon Ally") hand_.AddSpell(std::unique_ptr<Spell>(new SummonSpell(1, 10, 3)));
        else if (name_line == "Boost") hand_.AddSpell(std::unique_ptr<Spell>(new BoostSpell(1)));
        else { /* неизвестная*/ }
    }

    if (!(is >> tag) || tag != "END") throw ParseError("expected END");

    quit_requested_ = false;
    last_victory_  = false;
}

bool Game::TrySaveCommand(const std::string& path) {
    try {
        SaveManager sm(path.empty()? "save.txt" : path);
        sm.Save(*this);
        std::cout << "Saved to " << (path.empty()? "save.txt" : path) << "\n";
        Notify({GameEventType::Saved, player_.Name(), "", Coord{}, 0, path});
        return true;
    } catch (const GameError& e) {
        std::cout << e.what() << "\n"; return false;
    }
}
bool Game::TryLoadCommand(const std::string& path) {
    try {
        SaveManager sm(path.empty()? "save.txt" : path);
        sm.Load(*this);
        std::cout << "Loaded from " << (path.empty()? "save.txt" : path) << "\n";
        Notify({GameEventType::Loaded, player_.Name(), "", Coord{}, 0, path});
        return true;
    } catch (const GameError& e) {
        std::cout << e.what() << "\n"; return false;
    }
}

void Game::MaybeRewardSpell() {
    if (enemies_killed_ - last_reward_kill_count_ >= SPELL_REWARD_INTERVAL) {
        if (hand_.Size() < hand_.Capacity()) {
            hand_.AddRandomSpell(rng_);
            if (!hand_.Spells().empty()) {
                Notify({GameEventType::SpellAcquired, player_.Name(), "", player_.Pos(), 0, hand_.Spells().back()->Name()});
            }
            std::cout << "You obtained a new spell card!\n";
        }
        last_reward_kill_count_ = enemies_killed_;
    }
}

void Game::TryMovePlayer(int dx, int dy) {
    if (move_locked_next_turn_) {
        std::cout << "You are slowed: movement skipped this turn.\n";
        move_locked_next_turn_ = false;
        return;
    }

    Coord cur = player_.Pos();
    Coord next_pos(cur.X()+dx, cur.Y()+dy);

    if (!field_.IsPassable(next_pos)) { std::cout << "Blocked.\n"; return; }
    if (IsEnemyAt(next_pos)) { std::cout << "Enemy blocks the way.\n"; return; }
    if (IsAllyAt(next_pos)) { std::cout << "Ally blocks the way.\n"; return; }
    if (IsTowerAt(next_pos)) { std::cout << "Tower blocks the way.\n"; return; }
    if (IsTurretAt(next_pos)) { std::cout << "Turret blocks the way.\n"; return; }

    player_.SetPos(next_pos);
    Notify({GameEventType::PlayerMoved, player_.Name(), "", next_pos, 0, ""});

    if (field_.At(next_pos).IsSlow()) {
        move_locked_next_turn_ = true;
    }
}

void Game::PlayerAttack(int dx, int dy) {
    if (player_.CurrentMode() == Player::Mode::Melee) {
        bool hit = RayDamageFromPlayer(dx, dy, 1, player_.CurrentDamage(), false);
        if (!hit) {
            std::cout << "Nothing to hit.\n";
        }
    } else {
        bool hit = RayDamageFromPlayer(dx, dy, RANGE, player_.CurrentDamage(), false);
        if (!hit) {
            std::cout << "Miss.\n";
        }
    }
}

void Game::AlliesTurn() {
    auto sign = [](int x) {
        return (x > 0) - (x < 0);
    };

    for (auto& ally : allies_) {
        if (!ally.IsAlive()) continue;
        if (enemies_.empty() && turrets_.empty()) continue;

        int best_idx = -1;
        bool best_is_turret = false;
        int best_dist = INIT_DIST;

        for (int i=0; i<(int)enemies_.size(); ++i) {
            if (!enemies_[i].IsAlive()) continue;
            int dx = enemies_[i].Pos().X() - ally.Pos().X();
            if (dx < 0) dx = -dx;
            int dy = enemies_[i].Pos().Y() - ally.Pos().Y();
            if (dy < 0) dy = -dy;
            int dist = dx + dy;
            if (dist < best_dist) {
                best_dist = dist;
                best_idx = i;
                best_is_turret = false;
            }
        }

        for (int i=0; i<(int)turrets_.size(); ++i) {
            if (!turrets_[i].IsAlive()) continue;
            int dx = turrets_[i].Pos().X() - ally.Pos().X();
            if (dx < 0) dx = -dx;
            int dy = turrets_[i].Pos().Y() - ally.Pos().Y();
            if (dy < 0) dy = -dy;
            int dist = dx + dy;
            if (dist < best_dist) {
                best_dist = dist;
                best_idx = i;
                best_is_turret = true;
            }
        }

        if (best_idx < 0) continue;

        Coord ally_pos = ally.Pos();
        Coord target_pos = best_is_turret
            ? turrets_[best_idx].Pos()
            : enemies_[best_idx].Pos();

        int step_x = sign(target_pos.X() - ally_pos.X());
        int step_y = sign(target_pos.Y() - ally_pos.Y());

        bool try_x_first =
            (std::abs(target_pos.X()-ally_pos.X()) != std::abs(target_pos.Y()-ally_pos.Y()))
                ? (std::abs(target_pos.X()-ally_pos.X()) >
                   std::abs(target_pos.Y()-ally_pos.Y()))
                : (std::uniform_int_distribution<int>(0,1)(rng_) == 1);

        auto try_step = [&](int sx, int sy)->bool {
            Coord next_pos(ally_pos.X()+sx, ally_pos.Y()+sy);

            int enemy_idx = EnemyIndexAt(next_pos);
            if (enemy_idx >= 0) {
                enemies_[enemy_idx].TakeDamage(ally.Damage());
                std::cout << "Ally hits enemy for " << ally.Damage() << ".\n";
                Notify({GameEventType::DamageDealt, "Ally", "Enemy", next_pos, ally.Damage(), ""});
                return true;
            }

            int turret_idx = TurretIndexAt(next_pos);
            if (turret_idx >= 0) {
                turrets_[turret_idx].TakeDamage(ally.Damage());
                std::cout << "Ally hits turret for " << ally.Damage() << ".\n";
                Notify({GameEventType::DamageDealt, "Ally", "Turret", next_pos, ally.Damage(), ""});
                return true;
            }

            int tower_idx = TowerIndexAt(next_pos);
            if (tower_idx >= 0) {
                std::cout << "Ally hits tower, but it resists.\n";
                return true;
            }

            if (next_pos == player_.Pos()) return false;
            if (!field_.IsPassable(next_pos)) return false;
            if (IsEnemyAt(next_pos)) return false;
            if (IsTowerAt(next_pos)) return false;
            if (IsTurretAt(next_pos)) return false;
            if (IsAllyAt(next_pos)) return false;

            ally_pos = next_pos;
            return true;
        };

        bool acted = false;
        if (try_x_first) {
            acted = try_step(step_x,0) || try_step(0,step_y);
        } else {
            acted = try_step(0,step_y) || try_step(step_x,0);
        }

        if (acted && ally_pos != ally.Pos()) {
            ally.SetPos(ally_pos);
        }
    }
}

void Game::EnemiesTurn() {
    auto sign = [](int x) {
        return (x > 0) - (x < 0);
    };

    Coord player_pos = player_.Pos();
    std::uniform_int_distribution<int> coin(0,1);

    for (auto& enemy : enemies_) {
        if (!enemy.IsAlive()) continue;

        Coord enemy_pos = enemy.Pos();

        int step_x = sign(player_pos.X() - enemy_pos.X());
        int step_y = sign(player_pos.Y() - enemy_pos.Y());

        bool try_x_first =
            (std::abs(player_pos.X()-enemy_pos.X()) != std::abs(player_pos.Y()-enemy_pos.Y()))
                ? (std::abs(player_pos.X()-enemy_pos.X()) >
                   std::abs(player_pos.Y()-enemy_pos.Y()))
                : (coin(rng_) == 1);

        auto try_step = [&](int sx, int sy)->bool {
            Coord next_pos(enemy_pos.X()+sx, enemy_pos.Y()+sy);

            if (next_pos == player_pos) {
                player_.TakeDamage(enemy.Damage());
                std::cout << "Enemy hits you for " << enemy.Damage() << "!\n";
                Notify({GameEventType::DamageDealt, "Enemy", player_.Name(), next_pos, enemy.Damage(), ""});
                return true;
            }

            int ally_idx = AllyIndexAt(next_pos);
            if (ally_idx >= 0) {
                allies_[ally_idx].TakeDamage(enemy.Damage());
                std::cout << "Enemy hits ally for " << enemy.Damage() << "!\n";
                Notify({GameEventType::DamageDealt, "Enemy", "Ally", next_pos, enemy.Damage(), ""});
                return true;
            }

            if (!field_.IsPassable(next_pos)) return false;
            if (IsEnemyAt(next_pos)) return false;
            if (IsTowerAt(next_pos)) return false;
            if (IsTurretAt(next_pos)) return false;
            if (IsAllyAt(next_pos)) return false;
            if (next_pos == player_.Pos()) return false;

            enemy_pos = next_pos;

            int trap_idx = TrapIndexAt(enemy_pos);
            if (trap_idx >= 0) {
                int trap_dmg = traps_[trap_idx].Damage();
                enemy.TakeDamage(trap_dmg);
                std::cout << "Enemy steps on trap (" << trap_dmg << ").\n";
                Notify({GameEventType::TrapTriggered, "Trap", "Enemy", enemy_pos, trap_dmg, ""});
                RemoveTrapByIndex(trap_idx);
            }

            return true;
        };

        bool acted = false;
        if (try_x_first) {
            acted = try_step(step_x,0) || try_step(0,step_y);
        } else {
            acted = try_step(0,step_y) || try_step(step_x,0);
        }

        if (acted && enemy_pos != enemy.Pos()) {
            enemy.SetPos(enemy_pos);
        }

        if (!player_.IsAlive()) return;
    }
}

void Game::TurretsTurn() {
    for (auto& turret : turrets_) {
        if (!turret.IsAlive()) continue;

        turret.CooldownTick();
        bool fired = turret.TryFireAtPlayer(player_.Pos(), player_);
        if (fired) {
            std::cout << "Turret hits you for " << turret.Damage() << "\n";
            Notify({GameEventType::DamageDealt, "Turret", player_.Name(), player_.Pos(), turret.Damage(), ""});
        }
        if (!player_.IsAlive()) return;
    }
}

void Game::CleanupDead() {
    for (auto& e : enemies_) {
        if (!e.IsAlive()) {
            player_.AddScore(100);
            enemies_killed_++;
        }
    }
    for (auto& r : turrets_) {
        if (!r.IsAlive()) {
            player_.AddScore(150);
        }
    }

    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(),
                   [](const Enemy& e){ return !e.IsAlive(); }),
                   enemies_.end());

    allies_.erase(std::remove_if(allies_.begin(), allies_.end(),
                   [](const Ally& a){ return !a.IsAlive(); }),
                   allies_.end());

    turrets_.erase(std::remove_if(turrets_.begin(), turrets_.end(),
                   [](const Turret& r){ return !r.IsAlive(); }),
                   turrets_.end());

}

void Game::TrySpawnFromTowers() {
    const int dirs[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    for (auto& t : towers_) {
        if (!t.TickAndReady()) continue;

        for (auto d : dirs) {
            Coord p(t.Pos().X()+d[0], t.Pos().Y()+d[1]);
            if (!IsBlocked(p) && 0 <= p.X() && p.X() < w_ && 0 <= p.Y() && p.Y() < h_) {
                enemies_.emplace_back(
                    enemy_hp_,
                    enemy_damage_,
                    p
                );
                Notify({GameEventType::Spawned, "Tower", "", p, 0, ""});
                std::cout << "A tower spawns a new enemy.\n";
                break;
            }
        }
    }
}

bool Game::SpellDirectDamageRay(int dx, int dy, int range, int damage) {
    return RayDamageFromPlayer(dx, dy, range, damage, true);
}
void Game::AreaDamageAt(Coord top_left, int size, int damage) {
    SpellAreaDamageSquare(top_left, size, damage);
}
bool Game::PlaceTrapAdj(int dx, int dy, int damage) {
    return SpellPlaceTrap(dx, dy, damage);
}
bool Game::SummonAlliesAroundPlayer(int count, int ally_hp, int ally_damage) {
    return SpellSummonAllies(count, ally_hp, ally_damage);
}

bool Game::RayDamageFromPlayer(int dx, int dy, int range, int damage, bool from_spell) {
    Coord cur = player_.Pos();
    const char* label = from_spell ? "Spell" : "Attack";

    for (int step = 0; step < range; ++step) {
        cur.Set(cur.X() + dx, cur.Y() + dy);

        if (!field_.Inside(cur)) return false;

        int tower_idx = TowerIndexAt(cur);
        if (tower_idx >= 0) {
            std::cout << label << " hits a tower, but it resists.\n";
            return true;
        }

        int turret_idx = TurretIndexAt(cur);
        if (turret_idx >= 0) {
            int before_hp = turrets_[turret_idx].HP();
            turrets_[turret_idx].TakeDamage(damage);
            int after_hp = turrets_[turret_idx].HP();
            std::cout << label << " deals " << damage
                      << " damage to turret (HP " << before_hp
                      << " -> " << after_hp << ").\n";
            Notify({GameEventType::DamageDealt, label, "Turret", cur, damage, ""});
            if (!turrets_[turret_idx].IsAlive()) std::cout << "Turret destroyed!\n";
            return true;
        }

        int enemy_idx = EnemyIndexAt(cur);
        if (enemy_idx >= 0) {
            int before_hp = enemies_[enemy_idx].HP();
            enemies_[enemy_idx].TakeDamage(damage);
            int after_hp = enemies_[enemy_idx].HP();
            std::cout << label << " deals " << damage
                      << " damage to enemy (HP " << before_hp
                      << " -> " << after_hp << ").\n";
            Notify({GameEventType::DamageDealt, label, "Enemy", cur, damage, ""});
            if (!enemies_[enemy_idx].IsAlive()) std::cout << "Enemy killed!\n";
            return true;
        }

        if (!field_.IsPassable(cur)) {
            std::cout << label << " was blocked by a wall.\n";
            return false;
        }
    }
    return false;
}

void Game::SpellAreaDamageSquare(Coord top_left, int size, int damage) {
    bool someone_hit = false;

    for (int dy = 0; dy < size; ++dy) {
        for (int dx = 0; dx < size; ++dx) {
            Coord p(top_left.X() + dx, top_left.Y() + dy);
            if (!field_.Inside(p)) continue;

            int enemy_idx = EnemyIndexAt(p);
            if (enemy_idx >= 0) {
                someone_hit = true;

                int before_hp = enemies_[enemy_idx].HP();
                enemies_[enemy_idx].TakeDamage(damage);
                int after_hp = enemies_[enemy_idx].HP();

                std::cout << "Area spell hits enemy at ("
                          << p.X() << "," << p.Y() << ") for "
                          << damage << " damage (HP "
                          << before_hp << " -> " << after_hp << ").\n";
                Notify({GameEventType::DamageDealt, "AreaSpell", "Enemy", p, damage, ""});

                if (!enemies_[enemy_idx].IsAlive()) {
                    std::cout << "Enemy killed!\n";
                }
            }

            int turret_idx = TurretIndexAt(p);
            if (turret_idx >= 0) {
                someone_hit = true;

                int before_hp = turrets_[turret_idx].HP();
                turrets_[turret_idx].TakeDamage(damage);
                int after_hp = turrets_[turret_idx].HP();

                std::cout << "Area spell hits turret at ("
                          << p.X() << "," << p.Y() << ") for "
                          << damage << " damage (HP "
                          << before_hp << " -> " << after_hp << ").\n";
                Notify({GameEventType::DamageDealt, "AreaSpell", "Turret", p, damage, ""});

                if (!turrets_[turret_idx].IsAlive()) {
                    std::cout << "Turret destroyed!\n";
                }
            }

        }
    }

    if (!someone_hit) {
        std::cout << "Area spell explodes, but hits no targets.\n";
    }
}

bool Game::SpellPlaceTrap(int dx, int dy, int damage) {
    Coord base = player_.Pos();
    Coord place(base.X()+dx, base.Y()+dy);

    if (!field_.IsPassable(place)) return false;
    if (place == player_.Pos()) return false;
    if (IsEnemyAt(place)) return false;
    if (IsAllyAt(place)) return false;
    if (IsTowerAt(place)) return false;
    if (IsTurretAt(place)) return false;
    if (IsTrapAt(place)) return false;

    traps_.push_back(Trap(place, damage));
    std::cout << "Placed trap.\n";
    return true;
}

bool Game::SpellSummonAllies(int count, int ally_hp, int ally_damage) {
    const int dirs[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    int spawned = 0;

    for (int d = 0; d < 4 && spawned < count; ++d) {
        Coord p(player_.Pos().X()+dirs[d][0], player_.Pos().Y()+dirs[d][1]);
        if (!IsBlocked(p) && 0 <= p.X() && p.X() < w_ && 0 <= p.Y() && p.Y() < h_) {
            allies_.emplace_back(ally_hp, ally_damage, p);
            spawned++;
        }
    }

    if (spawned > 0) {
        std::cout << "Summoned " << spawned << " ally(-es).\n";
        return true;
    }
    return false;
}
