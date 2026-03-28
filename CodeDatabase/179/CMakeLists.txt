cmake_minimum_required(VERSION 3.10)
project(FantasyBattleGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы (ДОБАВЛЯЕМ новые файлы)
set(SOURCES
    main.cpp
    Game.cpp
    GameField.cpp
    Player.cpp
    Enemy.cpp
    EnemyBuilding.cpp
    EnemyTower.cpp
    Ally.cpp
    Cell.cpp
    Spell.cpp
    SpellHand.cpp
    GameRenderer.cpp
    Trap.cpp
    GameObject.cpp
    CommandReader.cpp      # ДОБАВЛЯЕМ
    Logger.cpp            # ДОБАВЛЯЕМ
    GameEventNotifier.cpp # ДОБАВЛЯЕМ
)

# Заголовочные файлы (ДОБАВЛЯЕМ новые файлы)
set(HEADERS
    Game.h
    GameField.h
    Player.h
    Enemy.h
    EnemyBuilding.h
    EnemyTower.h
    Ally.h
    Cell.h
    CellType.h
    Spell.h
    SpellHand.h
    GameRenderer.h
    IGameRenderer.h       # ДОБАВЛЯЕМ
    ICommandReader.h      # ДОБАВЛЯЕМ (НОВЫЙ)
    CommandReader.h       # ДОБАВЛЯЕМ
    Trap.h
    GameObject.h
    Position.h
    GameCommand.h
    FightMode.h
    GameException.h
    Logger.h              # ДОБАВЛЯЕМ
    GameEventNotifier.h   # ДОБАВЛЯЕМ
    GameController.h      # ДОБАВЛЯЕМ
    GameView.h            # ДОБАВЛЯЕМ
)

add_executable(FantasyBattleGame ${SOURCES} ${HEADERS})

if(WIN32)
    target_compile_definitions(FantasyBattleGame PRIVATE _WIN32)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(FantasyBattleGame PRIVATE -Wall -Wextra -Wpedantic)
endif()
