cmake_minimum_required(VERSION 3.20)
project(OOP_Lab_Game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        main.cpp
        AreaSpell.cpp
        ConsoleInputHandler.cpp
        ConsoleRenderer.cpp
        DirectSpell.cpp
        Enemy.cpp
        EnemyTower.cpp
        Entity.cpp
        Field.cpp
        Game.cpp
        GameManager.cpp
        Player.cpp
        PlayerHand.cpp
        Spell.cpp
        TrapSpell.cpp
)

set(HEADER_FILES
        AreaSpell.h
        Command.h
        ConsoleInputHandler.h
        ConsoleRenderer.h
        DirectSpell.h
        Enemy.h
        EnemyTower.h
        Entity.h
        Field.h
        Game.h
        GameController.h
        GameManager.h
        GameObserver.h
        GameVisualizer.h
        Pair_operations.h
        Player.h
        PlayerHand.h
        Spell.h
        TrapSpell.h
)

include_directories(.)


add_executable(OOP_Lab_Game ${SOURCE_FILES} ${HEADER_FILES})

if(MSVC)
    add_compile_options(/utf-8)
endif()