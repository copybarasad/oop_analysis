cmake_minimum_required(VERSION 3.15)

project(main)

set(CMAKE_CXX_STANDARD 20)

set(SFML_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/SFML-3.0.2)
message(-----> ${SFML_ROOT_DIR})

set(SFML_DIR ${SFML_ROOT_DIR}/lib/cmake/SFML)

find_package(SFML 3.0.2 COMPONENTS System Window Graphics Audio REQUIRED)

# set(TARGET_SRC "")

file(GLOB TARGET_SRC "src/*.cpp")
file(GLOB TARGET_SRC_2 "src/*/*.cpp")
file(GLOB TARGET_SRC_3 "src/*/*/*.cpp")
file(GLOB TARGET_HEADERS "src/*.h")
file(GLOB TARGET_HEADERS_2 "src/*/*.h")
file(GLOB TARGET_HEADERS_3 "src/*/*/*.h")

message(---TARGET_HEADERS----->${TARGET_HEADERS})
message(---TARGET_HEADERS_2----->${TARGET_HEADERS_2})
message(---TARGET_HEADERS_3----->${TARGET_HEADERS_3})
message(---TARGET_SRC----->${TARGET_SRC})
message(---TARGET_SRC_2----->${TARGET_SRC_2})
message(---TARGET_SRC_3----->${TARGET_SRC_3})

add_executable(main ${TARGET_SRC} ${TARGET_HEADERS} ${TARGET_HEADERS_2} ${TARGET_SRC_2} ${TARGET_HEADERS_3} ${TARGET_SRC_3})

target_include_directories(main PUBLIC ${TARGET_HEADERS_3} ${TARGET_HEADERS_2} ${TARGET_HEADERS})

# add_executable(${PROJECT_NAME} 
# src/main.cpp 
# src/Character.cpp 
# src/Entity.cpp 
# src/Player.cpp 
# src/Enemy.cpp 
# src/Map.cpp 
# src/Building.cpp 
# src/Game.cpp 
# src/CharacterInterface.cpp 
# src/DirectDamageSpell.cpp 
# src/SquareDamageSpell.cpp 
# src/Hand.cpp 
# src/HandInterface.cpp
# src/HasFont.cpp
# src/Screens/MainMenuScreen.cpp
# src/Screens/GameScreen.cpp
# src/Screens/PauseScreen.cpp
# src/Screens/TransitionToNewLevelScreen.cpp
# src/Program.cpp
# )

target_link_libraries(main PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME Game)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .exe)