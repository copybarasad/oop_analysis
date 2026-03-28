cmake_minimum_required(VERSION 3.20)
project(gamelb1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

add_executable(gamelb1
        main.cpp
        Cell.h
        Cell.cpp
        Player.h
        Player.cpp
        Enemy.h
        Enemy.cpp
        EnemyBuilding.h
        EnemyBuilding.cpp
        Field.h
        Field.cpp
        GameView.h
        GameView.cpp
        Entity.h
        Entity.cpp
        Creature.h
        Creature.cpp
        Game.h
        Game.cpp
        Game.cpp
        ISpell.h
        DirectDamageSpell.h
        DirectDamageSpell.cpp
        AoESpell.h
        AoESpell.cpp
        PlayerHand.h
        PlayerHand.cpp
        Ally.h
        EnemyTower.h
        TrapSpell.h
        TrapSpell.cpp
        SummonSpell.h
        SummonSpell.cpp
        BoostSpell.h
        BoostSpell.cpp
        Ally.cpp
        EnemyTower.cpp
        Trap.h
        Trap.cpp
        GameExceptions.h
        InputHandler.cpp
        InputHandler.h
        Command.h
        Logger.h
        GameLauncher.h
        GameExceptions.h
)

target_link_libraries(gamelb1 PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)