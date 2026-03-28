#include "game.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <queue>
#include <sstream>
#include "spell.hpp"
#include "spell_direct.hpp"
#include "spell_aoe.hpp"
#include "spell_trap.hpp"
#include "spell_towerbolt.hpp"
#include "input.hpp"
#include "save_manager.hpp"
#include "event_bus.hpp"
#include "event.hpp"

static int Idx(int x, int y, int W) { return y * W + x; }

Game::Game(int width, int height, double wall_density,
           const std::string& player_name, int player_hp, int player_melee, int player_ranged,
           int enemies_count, int enemy_hp, int enemy_damage, unsigned seed)
    : board_(width, height, wall_density, seed),
      player_(player_name, player_hp, player_melee, player_ranged, {0,0}),
      enemies_(),
      traps_(),
      towers_(),
      kills_since_spell_(0),
      acting_tower_x_(-1),
      acting_tower_y_(-1),
      wall_density_(wall_density),
      spell_power_bonus_(0),
      spell_rng_(seed ? seed : std::random_device{}()),
      event_bus_(nullptr) {
    InitializeLevel(width, height, wall_density, enemies_count, enemy_hp, enemy_damage, seed, true);
}

Game::~Game() = default;

bool Game::IsOccupiedByEnemy(int x, int y, std::size_t* out_index) const {
    for (std::size_t i = 0; i < enemies_.size(); ++i) {
        if (enemies_[i].IsAlive() && enemies_[i].Pos().x == x && enemies_[i].Pos().y == y) {
            if (out_index) *out_index = i;
            return true;
        }
    }
    return false;
}

Point Game::FindAnyFloor(unsigned seed) const {
    std::mt19937 gen(seed ? seed : std::random_device{}());
    std::uniform_int_distribution<int> rx(0, board_.Width() - 1);
    std::uniform_int_distribution<int> ry(0, board_.Height() - 1);
    for (int t = 0; t < 10000; ++t) {
        int x = rx(gen), y = ry(gen);
        if (board_.IsPassable(x, y)) return {x, y};
    }
    for (int y = 0; y < board_.Height(); ++y)
        for (int x = 0; x < board_.Width(); ++x)
            if (board_.IsPassable(x, y)) return {x, y};
    return {0, 0};
}

std::vector<int> Game::BuildDistanceFrom(const Point& start) const {
    const int W = board_.Width(), H = board_.Height();
    std::vector<int> dist(W * H, -1);
    if (!board_.IsPassable(start.x, start.y)) return dist;
    std::queue<Point> q;
    dist[Idx(start.x, start.y, W)] = 0;
    q.push(start);
    const int DX[4] = {1, -1, 0, 0};
    const int DY[4] = {0, 0, 1, -1};
    while (!q.empty()) {
        Point p = q.front(); q.pop();
        int d = dist[Idx(p.x, p.y, W)];
        for (int k = 0; k < 4; ++k) {
            int nx = p.x + DX[k], ny = p.y + DY[k];
            if (!board_.InBounds(nx, ny) || !board_.IsPassable(nx, ny)) continue;
            int id = Idx(nx, ny, W);
            if (dist[id] == -1) {
                dist[id] = d + 1;
                q.push({nx, ny});
            }
        }
    }
    return dist;
}

std::vector<Point> Game::LargestConnectedComponent() const {
    const int W = board_.Width(), H = board_.Height();
    std::vector<char> vis(W * H, 0);
    const int DX[4] = {1, -1, 0, 0};
    const int DY[4] = {0, 0, 1, -1};
    std::vector<Point> best;
    for (int y = 0; y < H; ++y) for (int x = 0; x < W; ++x) {
        if (!board_.IsPassable(x, y)) continue;
        int id0 = Idx(x, y, W);
        if (vis[id0]) continue;
        std::vector<Point> comp;
        std::queue<Point> q;
        q.push({x, y});
        vis[id0] = 1;
        comp.push_back({x, y});
        while (!q.empty()) {
            Point p = q.front(); q.pop();
            for (int k = 0; k < 4; ++k) {
                int nx = p.x + DX[k], ny = p.y + DY[k];
                if (!board_.InBounds(nx, ny) || !board_.IsPassable(nx, ny)) continue;
                int id = Idx(nx, ny, W);
                if (!vis[id]) {
                    vis[id] = 1;
                    q.push({nx, ny});
                    comp.push_back({nx, ny});
                }
            }
        }
        if (comp.size() > best.size()) best.swap(comp);
    }
    return best;
}

void Game::PlaceActorsInLargestComponent(int enemies_count, int enemy_hp, int enemy_damage, unsigned seed) {
    std::mt19937 gen(seed ? seed : std::random_device{}());
    for (int attempts = 0; attempts < 200; ++attempts) {
        auto comp = LargestConnectedComponent();
        if ((int)comp.size() >= 1 + enemies_count) {
            std::shuffle(comp.begin(), comp.end(), gen);
            player_.SetPos(comp.back()); comp.pop_back();
            enemies_.clear();
            enemies_.reserve((std::size_t)enemies_count);
            for (int i = 0; i < enemies_count; ++i) {
                Point p = comp.back(); comp.pop_back();
                enemies_.emplace_back(enemy_hp, enemy_damage, p);
            }
            return;
        }
        unsigned new_seed = gen();
        board_ = Board(board_.Width(), board_.Height(), 0.18, new_seed);
    }
    throw std::runtime_error("Game: placement failed");
}

void Game::PlaceOneTowerInComponent(unsigned seed){
    auto comp = LargestConnectedComponent();
    if (comp.empty()) return;
    std::mt19937 gen(seed ? seed : std::random_device{}());
    std::shuffle(comp.begin(), comp.end(), gen);
    for (auto it = comp.rbegin(); it != comp.rend(); ++it){
        Point p = *it;
        if (!board_.IsPassable(p.x,p.y)) continue;
        if ((p.x==player_.Pos().x && p.y==player_.Pos().y)) continue;
        bool occ=false;
        for (auto& e:enemies_) if (e.IsAlive() && e.Pos().x==p.x && e.Pos().y==p.y) { occ=true; break; }
        if (occ) continue;
        towers_.emplace_back(10, 4, p, 3);
        break;
    }
}

bool Game::PlayerAttack(int dx, int dy) {
    if (dx == 0 && dy == 0) return false;
    if (player_.Mode() == CombatMode::kMelee) {
        int tx = player_.Pos().x + dx;
        int ty = player_.Pos().y + dy;
        std::size_t idx = 0;
        if (IsOccupiedByEnemy(tx, ty, &idx)) {
            enemies_[idx].TakeDamage(player_.CurrentDamage());
            if (!enemies_[idx].IsAlive()) { player_.AddScore(1); ++kills_since_spell_; }
            CleanupDeadEnemies();
            if (event_bus_) {
                Event ev{EventType::Damage, player_.Name(), "Enemy", player_.CurrentDamage(), tx, ty, ""};
                event_bus_->Publish(ev);
                if (!IsOccupiedByEnemy(tx, ty)) {
                    Event kill{EventType::Kill, player_.Name(), "Enemy", 0, tx, ty, ""};
                    event_bus_->Publish(kill);
                }
            }
        }
        return true;
    } else {
        int range = player_.RangedRange();
        int x = player_.Pos().x;
        int y = player_.Pos().y;
        for (int step = 1; step <= range; ++step) {
            int tx = x + dx * step;
            int ty = y + dy * step;
            if (!board_.InBounds(tx, ty)) break;
            if (board_.GetCellType(tx, ty) == CellType::kWall) break;
            std::size_t idx = 0;
            if (IsOccupiedByEnemy(tx, ty, &idx)) {
                enemies_[idx].TakeDamage(player_.CurrentDamage());
                if (!enemies_[idx].IsAlive()) { player_.AddScore(1); ++kills_since_spell_; }
                CleanupDeadEnemies();
                if (event_bus_) {
                    Event ev{EventType::Damage, player_.Name(), "Enemy", player_.CurrentDamage(), tx, ty, ""};
                    event_bus_->Publish(ev);
                    if (!IsOccupiedByEnemy(tx, ty)) {
                        Event kill{EventType::Kill, player_.Name(), "Enemy", 0, tx, ty, ""};
                        event_bus_->Publish(kill);
                    }
                }
                break;
            }
        }
        return true;
    }
}

bool Game::PlayerTurn(const std::string& command, Input& input) {
    std::string line = command;
    auto parse_int = [](const std::string& s, std::size_t& i, int& out)->bool{
        while (i<s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
        bool neg=false; if (i<s.size() && (s[i]=='-'||s[i]=='+')){ neg=(s[i]=='-'); ++i; }
        if (i>=s.size() || !std::isdigit(static_cast<unsigned char>(s[i]))) return false;
        long v=0;
        while (i<s.size() && std::isdigit(static_cast<unsigned char>(s[i]))){ v=v*10+(s[i]-'0'); ++i; }
        out = neg ? -static_cast<int>(v) : static_cast<int>(v);
        return true;
    };
    std::size_t i=0;
    while (i<line.size() && std::isspace(static_cast<unsigned char>(line[i]))) ++i;
    if (i<line.size() && std::isdigit(static_cast<unsigned char>(line[i]))){
        int slot = 0;
        while (i<line.size() && std::isdigit(static_cast<unsigned char>(line[i]))){
            slot = slot*10 + (line[i]-'0');
            ++i;
        }
        int tx,ty;
        bool got = parse_int(line,i,tx) && parse_int(line,i,ty);
        if (!got){
            tx = input.ReadIntRange("Target X", 0, board_.Width()-1);
            ty = input.ReadIntRange("Target Y", 0, board_.Height()-1);
        }
        if (slot>=1 && (std::size_t)slot<=player_.Hand().Size()){
            bool used = player_.Hand().Use((std::size_t)(slot-1), *this, tx, ty);
            if (used && event_bus_) {
                const ISpell* sp = player_.Hand().Get((std::size_t)(slot-1));
                std::string name = sp ? sp->Name() : "";
                Event ev{EventType::SpellCast, player_.Name(), "", 0, tx, ty, name};
                event_bus_->Publish(ev);
            }
            if (used) return true;
            std::cout<<"Invalid target\n";
            return false;
        } else {
            std::cout<<"No card in slot\n";
            return false;
        }
    }
    char c = 0;
    for (char ch : line) {
        if (!std::isspace(static_cast<unsigned char>(ch))) { c = (char)std::toupper((unsigned char)ch); break; }
    }
    if (c == 0) return false;
    auto try_move = [&](int dx, int dy) -> bool {
        int nx = player_.Pos().x + dx;
        int ny = player_.Pos().y + dy;
        if (!board_.InBounds(nx, ny)) { std::cout << "Blocked: out of bounds\n"; return false; }
        if (!board_.IsPassable(nx, ny)) { std::cout << "Blocked: wall\n"; return true; }
        std::size_t idx = 0;
        if (IsOccupiedByEnemy(nx, ny, &idx)) {
            enemies_[idx].TakeDamage(player_.CurrentDamage());
            if (!enemies_[idx].IsAlive()) { player_.AddScore(1); ++kills_since_spell_; }
            CleanupDeadEnemies();
            if (event_bus_) {
                Event ev{EventType::Damage, player_.Name(), "Enemy", player_.CurrentDamage(), nx, ny, ""};
                event_bus_->Publish(ev);
                if (!IsOccupiedByEnemy(nx, ny)) {
                    Event kill{EventType::Kill, player_.Name(), "Enemy", 0, nx, ny, ""};
                    event_bus_->Publish(kill);
                }
            }
            return true;
        }
        for (auto& t : towers_) {
            if (t.IsAlive() && t.Pos().x==nx && t.Pos().y==ny) {
                t.TakeDamage(player_.CurrentDamage());
                if (event_bus_) {
                    Event ev{EventType::Damage, player_.Name(), "Tower", player_.CurrentDamage(), nx, ny, ""};
                    event_bus_->Publish(ev);
                    if (!t.IsAlive()) {
                        Event kill{EventType::Kill, player_.Name(), "Tower", 0, nx, ny, ""};
                        event_bus_->Publish(kill);
                    }
                }
                return true;
            }
        }
        player_.SetPos({nx, ny});
        if (event_bus_) {
            Event ev{EventType::Move, player_.Name(), "", 0, nx, ny, ""};
            event_bus_->Publish(ev);
        }
        return true;
    };
    switch (c) {
        case 'W': return try_move(0, -1);
        case 'S': return try_move(0, 1);
        case 'A': return try_move(-1, 0);
        case 'D': return try_move(1, 0);
        case 'U': return PlayerAttack(0, -1);
        case 'L': return PlayerAttack(-1, 0);
        case 'V': return PlayerAttack(0,  1);
        case 'R': return PlayerAttack(1,  0);
        case 'T': player_.ToggleMode(); return true;
        case 'Q':
            std::cout << "Use EXIT command to quit the game.\n";
            return false;
        default:
            std::cout << "Unknown command\n";
            return false;
    }
}

void Game::CleanupDeadEnemies() {
    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(),
                   [](const Enemy& e){ return !e.IsAlive(); }), enemies_.end());
    towers_.erase(std::remove_if(towers_.begin(), towers_.end(),
                   [](const EnemyTower& t){ return !t.IsAlive(); }), towers_.end());
}

bool Game::EnemiesTurn() {
    const auto dist = BuildDistanceFrom(player_.Pos());
    const int W = board_.Width();
    const int DX[4] = {1, -1, 0, 0};
    const int DY[4] = {0, 0, 1, -1};
    for (auto& e : enemies_) {
        if (!e.IsAlive()) continue;
        if (std::abs(e.Pos().x - player_.Pos().x) + std::abs(e.Pos().y - player_.Pos().y) == 1) {
            DamagePlayer(e.BaseDamage(), "Enemy", e.Pos().x, e.Pos().y);
            if (!player_.IsAlive()) return true;
            continue;
        }
        int eid = Idx(e.Pos().x, e.Pos().y, W);
        int d_here = (eid >= 0 && eid < (int)dist.size()) ? dist[eid] : -1;
        if (d_here <= 0) {
            continue;
        }
        bool moved = false;
        for (int k = 0; k < 4; ++k) {
            int nx = e.Pos().x + DX[k], ny = e.Pos().y + DY[k];
            if (!board_.InBounds(nx, ny) || !board_.IsPassable(nx, ny)) continue;
            int nid = Idx(nx, ny, W);
            if (dist[nid] == d_here - 1) {
                if (IsOccupiedByEnemy(nx, ny)) continue;
                if (nx == player_.Pos().x && ny == player_.Pos().y) continue;
                bool tower_here=false;
                for (auto& t:towers_) if (t.IsAlive() && t.Pos().x==nx && t.Pos().y==ny) { tower_here=true; break; }
                if (tower_here) continue;
                e.SetPos({nx, ny});
                moved = true;
                int mx = e.Pos().x, my = e.Pos().y;
                for (std::size_t ti=0; ti<traps_.size(); ++ti){
                    if (traps_[ti].X()==mx && traps_[ti].Y()==my){
                        int trap_damage = traps_[ti].Damage();
                        e.TakeDamage(trap_damage);
                        traps_.erase(traps_.begin()+static_cast<long>(ti));
                        if (event_bus_) {
                            Event ev{EventType::Damage, "Trap", "Enemy", trap_damage, mx, my, ""};
                            event_bus_->Publish(ev);
                        }
                        if (!e.IsAlive()){ player_.AddScore(1); ++kills_since_spell_; }
                        if (!e.IsAlive() && event_bus_) {
                            Event kill{EventType::Kill, "Trap", "Enemy", 0, mx, my, ""};
                            event_bus_->Publish(kill);
                        }
                        break;
                    }
                }
                break;
            }
        }
        (void)moved;
    }
    TowersTurn();
    CleanupDeadEnemies();
    MaybeGrantSpell();
    return !player_.IsAlive();
}

bool Game::IsPlayerDead() const noexcept {
    return !player_.IsAlive();
}

bool Game::IsVictory() const noexcept {
    for (const auto& e : enemies_) {
        if (e.IsAlive()) return false;
    }
    return true;
}

const Board& Game::BoardRef() const noexcept { return board_; }
const std::vector<Enemy>& Game::Enemies() const noexcept { return enemies_; }
const std::vector<EnemyTower>& Game::Towers() const noexcept { return towers_; }
const std::vector<Trap>& Game::Traps() const noexcept { return traps_; }
void Game::PlayerPos(int& x,int& y) const noexcept { x = player_.Pos().x; y = player_.Pos().y; }
bool Game::EnemyAt(int x,int y) const { return IsOccupiedByEnemy(x,y,nullptr); }
bool Game::HasTrapAt(int x,int y) const {
    for (auto& t:traps_) if (t.X()==x && t.Y()==y) return true;
    return false;
}
void Game::PlaceTrap(int x,int y,int dmg){
    traps_.emplace_back(x,y,dmg);
    if (event_bus_) {
        Event ev{EventType::TrapPlaced, player_.Name(), "", dmg, x, y, ""};
        event_bus_->Publish(ev);
    }
}
void Game::DamagePlayer(int dmg, const std::string& source, int sx, int sy){
    player_.TakeDamage(dmg);
    if (event_bus_) {
        Event ev{EventType::Damage, source, player_.Name(), dmg, sx, sy, ""};
        event_bus_->Publish(ev);
    }
}
bool Game::DamageEnemyAt(int x,int y,int dmg){
    std::size_t idx=0;
    if (IsOccupiedByEnemy(x,y,&idx)){
        enemies_[idx].TakeDamage(dmg);
        if (!enemies_[idx].IsAlive()){
            player_.AddScore(1);
            ++kills_since_spell_;
            if (event_bus_) {
                Event kill{EventType::Kill, "Player", "Enemy", 0, x, y, ""};
                event_bus_->Publish(kill);
            }
        }
        if (event_bus_) {
            Event ev{EventType::Damage, "Player", "Enemy", dmg, x, y, ""};
            event_bus_->Publish(ev);
        }
        CleanupDeadEnemies();
        return true;
    }
    return false;
}
bool Game::DamageTowerAt(int x,int y,int dmg){
    for (auto& t:towers_){
        if (!t.IsAlive()) continue;
        if (t.Pos().x==x && t.Pos().y==y){
            t.TakeDamage(dmg);
            if (event_bus_) {
                Event ev{EventType::Damage, "Player", "Tower", dmg, x, y, ""};
                event_bus_->Publish(ev);
                if (!t.IsAlive()) {
                    Event kill{EventType::Kill, "Player", "Tower", 0, x, y, ""};
                    event_bus_->Publish(kill);
                }
            }
            return true;
        }
    }
    return false;
}
void Game::TowersTurn(){
    static TowerBoltSpell bolt(2,3);
    for (auto& t: towers_){
        if (!t.IsAlive()) continue;
        if (t.OnCooldown()){ t.Tick(); continue; }
        int px,py; PlayerPos(px,py);
        int d = std::abs(px - t.Pos().x) + std::abs(py - t.Pos().y);
        if (d<=t.Range()){
            acting_tower_x_ = t.Pos().x;
            acting_tower_y_ = t.Pos().y;
            bool used = bolt.Use(*this, px, py);
            acting_tower_x_ = acting_tower_y_ = -1;
            if (used) t.SetCooldown(1); else t.Tick();
        } else {
            t.Tick();
        }
    }
}
void Game::MaybeGrantSpell(){
    const int need = 3;
    if (kills_since_spell_ >= need){
        kills_since_spell_ = 0;
        if (player_.Hand().Size() < player_.Hand().Capacity()){
            auto s = MakeRandomSpell();
            std::string spell_name = s ? s->Name() : "";
            player_.Hand().Add(std::move(s));
            if (event_bus_ && !spell_name.empty()) {
                Event ev{EventType::SpellGranted, player_.Name(), "", 0, 0, 0, spell_name};
                event_bus_->Publish(ev);
            }
        }
    }
}
std::unique_ptr<ISpell> Game::MakeRandomSpell(){
    std::uniform_int_distribution<int> dist(0,2);
    int r = dist(spell_rng_);
    int bonus = spell_power_bonus_;
    if (r==0) return std::unique_ptr<ISpell>(new DirectDamageSpell(5+bonus,4));
    if (r==1) return std::unique_ptr<ISpell>(new AoESpell2x2(3+bonus,4));
    return std::unique_ptr<ISpell>(new TrapSpell(4+bonus,3));
}
void Game::CurrentActingTowerPos(int& x,int& y) const noexcept { x=acting_tower_x_; y=acting_tower_y_; }
void Game::SetEventBus(EventBus* bus) noexcept { event_bus_ = bus; }

void Game::InitializeLevel(int width,int height,double wall_density,
                           int enemies_count,int enemy_hp,int enemy_damage,unsigned seed,
                           bool grant_initial_spell) {
    if (enemies_count < 0) throw std::invalid_argument("Game: enemies_count must be >= 0");
    if (enemy_hp <= 0)      throw std::invalid_argument("Game: enemy_hp must be > 0");
    if (enemy_damage < 0)   throw std::invalid_argument("Game: enemy_damage must be >= 0");
    board_ = Board(width, height, wall_density, seed);
    wall_density_ = wall_density;
    traps_.clear();
    towers_.clear();
    enemies_.clear();
    kills_since_spell_ = 0;
    acting_tower_x_ = acting_tower_y_ = -1;
    int floors = 0;
    for (int y = 0; y < board_.Height(); ++y)
        for (int x = 0; x < board_.Width(); ++x)
            floors += board_.IsPassable(x, y) ? 1 : 0;
    if (floors < 1) throw std::runtime_error("Game: no passable cells");
    PlaceActorsInLargestComponent(enemies_count, enemy_hp, enemy_damage, seed);
    PlaceOneTowerInComponent(seed);
    if (grant_initial_spell){
        auto s = MakeRandomSpell();
        player_.Hand().Add(std::move(s));
        if (event_bus_) {
            const ISpell* p = player_.Hand().Get(player_.Hand().Size()-1);
            if (p){
                Event ev{EventType::SpellGranted, player_.Name(), "", 0, 0, 0, p->Name()};
                event_bus_->Publish(ev);
            }
        }
    }
}

void Game::StartLevel(int width,int height,double wall_density,
                      int enemies_count,int enemy_hp,int enemy_damage,unsigned seed) {
    InitializeLevel(width, height, wall_density, enemies_count, enemy_hp, enemy_damage, seed, false);
}

void Game::SetSpellPowerBonus(int bonus) {
    spell_power_bonus_ = bonus < 0 ? 0 : bonus;
}

int Game::SpellPowerBonus() const noexcept {
    return spell_power_bonus_;
}

void Game::DiscardHalfHand(std::mt19937& rng) {
    player_.Hand().DiscardHalf(rng);
}

Player& Game::PlayerRef() noexcept {
    return player_;
}

const Player& Game::PlayerRef() const noexcept {
    return player_;
}

void Game::SaveState(SaveData& data) const {
    data.hasActiveGame = true;
    data.boardWidth = board_.Width();
    data.boardHeight = board_.Height();
    data.wallDensity = wall_density_;
    data.boardCells.clear();
    data.boardCells.reserve(static_cast<std::size_t>(board_.Width()) * static_cast<std::size_t>(board_.Height()));
    for (int y = 0; y < board_.Height(); ++y) {
        for (int x = 0; x < board_.Width(); ++x) {
            data.boardCells.push_back(board_.GetCellType(x, y) == CellType::kWall ? 1 : 0);
        }
    }

    data.playerName = player_.Name();
    data.playerCurrentHealth = player_.HP();
    data.playerMaxHealth = player_.MaxHP();
    data.playerMeleeDamage = player_.MeleeDamage();
    data.playerRangedDamage = player_.RangedDamage();
    data.playerRangedRange = player_.RangedRange();
    data.playerScore = player_.Score();
    data.playerMode = (player_.Mode() == CombatMode::kMelee) ? 0 : 1;
    data.playerX = player_.Pos().x;
    data.playerY = player_.Pos().y;
    data.killsSinceSpell = kills_since_spell_;
    data.serializedSpells = player_.Hand().Serialize();

    data.enemies.clear();
    for (const auto& e : enemies_) {
        if (!e.IsAlive()) continue;
        SavedEnemy rec;
        rec.hp = e.HP();
        rec.damage = e.BaseDamage();
        rec.x = e.Pos().x;
        rec.y = e.Pos().y;
        data.enemies.push_back(rec);
    }

    data.towers.clear();
    for (const auto& t : towers_) {
        if (!t.IsAlive()) continue;
        SavedTower rec;
        rec.hp = t.HP();
        rec.damage = t.BaseDamage();
        rec.x = t.Pos().x;
        rec.y = t.Pos().y;
        rec.range = t.Range();
        rec.cooldown = t.Cooldown();
        data.towers.push_back(rec);
    }

    data.traps.clear();
    for (const auto& tr : traps_) {
        SavedTrap rec;
        rec.x = tr.X();
        rec.y = tr.Y();
        rec.damage = tr.Damage();
        data.traps.push_back(rec);
    }

    data.spellPowerBonus = spell_power_bonus_;
    std::ostringstream rng_stream;
    rng_stream << spell_rng_;
    data.spellRngState = rng_stream.str();
}

void Game::LoadState(const SaveData& data) {
    if (!data.hasActiveGame) return;
    wall_density_ = data.wallDensity;
    board_ = Board(data.boardWidth, data.boardHeight, data.wallDensity, 1);
    for (int y = 0; y < board_.Height(); ++y) {
        for (int x = 0; x < board_.Width(); ++x) {
            std::size_t idx = static_cast<std::size_t>(y) * static_cast<std::size_t>(board_.Width()) + static_cast<std::size_t>(x);
            bool is_wall = (idx < data.boardCells.size()) ? (data.boardCells[idx] != 0) : false;
            if (is_wall) {
                board_.SetWall(x, y);
            } else {
                board_.SetFloor(x, y);
            }
        }
    }

    const std::size_t hand_cap = player_.Hand().Capacity();
    Player restored(data.playerName,
                    std::max(1, data.playerMaxHealth),
                    data.playerMeleeDamage,
                    data.playerRangedDamage,
                    {data.playerX, data.playerY},
                    hand_cap,
                    data.playerRangedRange);
    player_ = std::move(restored);
    player_.SetMaxHP(data.playerMaxHealth);
    player_.SetHP(data.playerCurrentHealth);
    player_.SetScore(data.playerScore);
    player_.SetMode(data.playerMode == 0 ? CombatMode::kMelee : CombatMode::kRanged);
    player_.Hand().Load(data.serializedSpells);

    traps_.clear();
    for (const auto& tr : data.traps) {
        traps_.emplace_back(tr.x, tr.y, tr.damage);
    }

    enemies_.clear();
    enemies_.reserve(data.enemies.size());
    for (const auto& e : data.enemies) {
        enemies_.emplace_back(e.hp, e.damage, Point{e.x, e.y});
    }

    towers_.clear();
    towers_.reserve(data.towers.size());
    for (const auto& t : data.towers) {
        towers_.emplace_back(t.hp, t.damage, Point{t.x, t.y}, t.range);
        towers_.back().SetCooldown(t.cooldown);
    }

    kills_since_spell_ = data.killsSinceSpell;
    spell_power_bonus_ = data.spellPowerBonus;
    acting_tower_x_ = acting_tower_y_ = -1;
    if (!data.spellRngState.empty()) {
        std::istringstream rng_stream(data.spellRngState);
        rng_stream >> spell_rng_;
    }
}
