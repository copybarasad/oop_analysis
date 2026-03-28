cmake_minimum_required(VERSION 3.16)
project(Game VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


link_directories(/opt/homebrew/lib)
include_directories(/opt/homebrew/Cellar/sfml/3.0.2/include)
include_directories(${SFML_INCLUDE_DIR})


find_package(SFML 3.0 COMPONENTS Graphics Window System QUIET)

include_directories(${CMAKE_SOURCE_DIR}/include)


set(SOURCES
        src/main.cpp
        src/Cell.cpp
        src/GameField.cpp
        src/Player.cpp
        src/Enemy.cpp
        src/Ally.cpp
        src/EnemyBuilding.cpp
        src/EnemyTower.cpp
        src/Spell.cpp
        src/DirectDamageSpell.cpp
        src/AreaDamageSpell.cpp
        src/TrapSpell.cpp
        src/SummonSpell.cpp
        src/EnhancementSpell.cpp
        src/PlayerHand.cpp
        src/Game.cpp
        src/Command.cpp
        src/GameEvent.cpp
        src/Logger.cpp
        src/KeyBindings.cpp
        src/InputReader.cpp
        src/GameRenderer.cpp
)


set(HEADERS
        include/Cell.h
        include/GameField.h
        include/Player.h
        include/Enemy.h
        include/Ally.h
        include/EnemyBuilding.h
        include/EnemyTower.h
        include/Spell.h
        include/DirectDamageSpell.h
        include/AreaDamageSpell.h
        include/TrapSpell.h
        include/SummonSpell.h
        include/EnhancementSpell.h
        include/PlayerHand.h
        include/Game.h
        include/GameState.h
        include/GameException.h
        include/Command.h
        include/GameEvent.h
        include/Logger.h
        include/KeyBindings.h
        include/InputReader.h
        include/GameRenderer.h
        include/GameController.h
        include/GameView.h
)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


if (SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
endif ()


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/saves)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)


if (EXISTS ${CMAKE_SOURCE_DIR}/config/keybindings.cfg)
    configure_file(${CMAKE_SOURCE_DIR}/config/keybindings.cfg
            ${CMAKE_BINARY_DIR}/config/keybindings.cfg COPYONLY)
endif ()


install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY config/ DESTINATION config)
