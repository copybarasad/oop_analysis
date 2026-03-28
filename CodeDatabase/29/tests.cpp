#if __has_include("src/board.hpp")
#include "src/board.hpp"
#include "src/cell.hpp"
#include "src/actor.hpp"
#include "src/player.hpp"
#include "src/enemy.hpp"
#include "src/game.hpp"
#else
#include "board.hpp"
#include "cell.hpp"
#include "actor.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "game.hpp"
#endif
#include <iostream>

#if __has_include("src/board.cpp")
#include "src/board.cpp"
#include "src/cell.cpp"
#include "src/actor.cpp"
#include "src/player.cpp"
#include "src/enemy.cpp"
#include "src/game.cpp"
#else
#include "board.cpp"
#include "cell.cpp"
#include "actor.cpp"
#include "player.cpp"
#include "enemy.cpp"
#include "game.cpp"
#endif

static void AttackTest() {
    std::cout << "Attack test\n";
    Board b(10,10,0.0);
    Player p("Warrior", 180, 7, 4, {2,3});
    Enemy e(10, 93, {2,2});
    std::cout << p.Name() << " HP before: " << p.HP() << "\n";
    int dmg=0;
    (void)e.StepOrHit(b, p.Pos(), dmg);
    if (dmg>0) { std::cout << "Enemy attacks " << p.Name() << "\n"; p.TakeDamage(dmg); }
    std::cout << p.Name() << " HP after: " << p.HP() << "\n\n";
}

static void UnpassableTest() {
    std::cout << "Unpassable test\n";
    Board b(10,10,0.0);
    b.SetWall(1,1);
    if (!b.IsPassable(1,1)) std::cout << "This cell is unpassable\n\n";
    else std::cout << "ERROR: expected wall at (1,1)\n\n";
}

static void WeaponSwitchTest() {
    std::cout << "Switch test\n";
    Player p("Warrior", 30, 7, 4, {0,0});
    if (p.CurrentDamage()!=7) std::cout<<"ERROR: wrong initial damage\n";
    p.ToggleMode();
    if (p.CurrentDamage()!=4) std::cout<<"ERROR: wrong ranged damage\n";
    std::cout << "Switch test done\n\n";
}

static void FieldTest() {
    std::cout << "Field test\n";
    Board b(12,12,0.0);
    int x=1,y=7;
    if (b.InBounds(x,y)) std::cout << "Go to cell("<<x<<","<<y<<"), passable="<<(b.IsPassable(x,y)?1:0)<<"\n\n";
    else std::cout << "ERROR: out of bounds\n\n";
}

static void CopySizeTest() {
    std::cout << "Copy size test\n";
    Board b1(12,12,0.0);
    Board b2=b1;
    std::cout << "Copied field size: " << b2.Width() << "x" << b2.Height() << "\n\n";
}

static void MovementTest() {
    std::cout << "Movement test\n";
    Board original(15,15,0.0);
    Board moved=std::move(original);
    std::cout << "Moved field size: " << moved.Width() << "x" << moved.Height() << "\n";
    std::cout << "Original field after move: " << original.Width() << "x" << original.Height() << "\n\n";
}

int main() {
    try {
        UnpassableTest();
        FieldTest();
        CopySizeTest();
        AttackTest();
        MovementTest();
        WeaponSwitchTest();
    } catch (const std::exception& e) {
        std::cerr << "test error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}