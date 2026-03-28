#include "Game_Manager.hpp"
#include "LevelUpDialog.hpp"
#include <iostream>
#include <vector> 
#include <chrono>
#include "GameSaver.hpp"
#include <thread>


Game_Manager::Game_Manager() {
    logger_consol = new GameLogger();
    render_system = nullptr;
    field = nullptr;
    entity_manager = nullptr;
    hero_controller = nullptr;
    window = nullptr;
    status = game_running;
    current_level = 1;
    counter = 0;
}

void Game_Manager::startNewGame(sf::Vector2u size, int startLevel) {
    if (field) delete field;
    if (entity_manager) delete entity_manager;
    if (hero_controller) delete hero_controller;
    if (render_system) delete render_system;
    if (window) delete window;
    
    field = new Field(size);
    entity_manager = new Entity_Manager(field);
    render_system = new Render_System();
    
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window = new sf::RenderWindow(desktop, "FullScreen Field", sf::Style::Fullscreen);
    
    current_level = startLevel;
    entity_manager->initializeEntities(current_level);
    
    hero_controller = new HeroController(
        field, 
        *logger_consol,
        entity_manager
    );
    
    initLevelText();
    logger_consol->log("New game started! Level " + std::to_string(current_level) + "\n");
}

void Game_Manager::loadGame(const std::string& filename) {
    if (field) delete field;
    if (entity_manager) delete entity_manager;
    if (hero_controller) delete hero_controller;
    if (render_system) delete render_system;
    if (window) delete window;
    
    try {
        GameSaver::loadGame(entity_manager, field, filename, current_level);
        
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window = new sf::RenderWindow(desktop, "FullScreen Field", sf::Style::Fullscreen);
        
        render_system = new Render_System();
        hero_controller = new HeroController(
            field,
            *logger_consol,
            entity_manager
        );
        
        initLevelText();
        logger_consol->log("Game loaded from " + filename + "\n");
        
    } catch (const std::exception& e) {
        logger_consol->log(std::string("Loading error: ") + e.what() + "\n");
        startNewGame();
    }
}

void Game_Manager::run() {
    if (!window) {
        logger_consol->log("Error: Game not initialized!\n");
        return;
    }
    
    while (window->isOpen()) {
        handleEvents();
        update();
        render();
    }
}


Game_Manager::~Game_Manager() {
    delete hero_controller;
    delete entity_manager;
    delete field;
    delete logger_consol;
    delete render_system;
    delete window;
}


void Game_Manager::handleEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();
        
        if (status == game_over_dialog || status == level_complete || status == level_up_dialog) {
            handleDialogEvents(event);
            continue;
        }


        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::F5) {
                try {
                    GameSaver::saveGame(entity_manager->get_hero(), entity_manager, field, "save.json", current_level);
                    logger_consol->log("The game is saved in save.json\n");
                } catch (const std::exception& e) {
                    logger_consol->log(std::string("Saving error: ") + e.what() + "\n");
                }
            }

            if (event.key.code == sf::Keyboard::F9) {
                try {
                    Entity_Manager* tempManager = entity_manager;
                    Field* tempField = field;
                    
                    GameSaver::loadGame(tempManager, tempField, "save.json", current_level);
                    
                    entity_manager = tempManager;
                    field = tempField;
                    
                    logger_consol->log("The game was uploaded from save.json\n");
                    
                    delete hero_controller;
                    delete render_system;
                    
                    hero_controller = new HeroController(
                        field,
                        *logger_consol,
                        entity_manager
                    );
                    render_system = new Render_System();
                    
                    updateLevelText();
                    
                } catch (const std::exception& e) {
                    logger_consol->log(std::string("Uploading error: ") + e.what() + "\n");
                }
            }
        }



        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window->close();
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            bool spellCasted = hero_controller->handleMouseClick(*window, *field);

            if (spellCasted && !hero_controller->get_awaiting_spell_target()) {
                if (!hero_controller->shouldSkipEnemyTurn()) {
                    auto& cells = field->get_cells();

                    if (entity_manager->get_statue())
                        entity_manager->get_statue()->attack(*field, *logger_consol, counter);

                    if (entity_manager->get_portal()) {
                        if (counter % entity_manager->get_portal()->get_frequency() == 0)
                            entity_manager->spawn_enemis_by_portal(cells, *field);
                    }

                    entity_manager->moveAllEnemies(*field, *logger_consol);
                    entity_manager->moveAllAllies(*field, *logger_consol);
                    counter++;
                } else {
                    hero_controller->setSkipEnemyTurn(false);
                }
            }

            continue;
        }


        bool heroActionPerformed = hero_controller->handleInput(event, *field);

        if (heroActionPerformed && !hero_controller->get_awaiting_spell_target()) {
            if (hero_controller->wasSpellCanceled()) {
                continue;
            }


            if (!hero_controller->shouldSkipEnemyTurn()) {

                entity_manager->moveAllAllies(*field, *logger_consol);
                entity_manager->moveAllEnemies(*field, *logger_consol);



                auto& cells = field->get_cells();
                Hero* hero = entity_manager->get_hero();

                if (entity_manager->get_statue()) {
                    entity_manager->get_statue()->attack(*field, *logger_consol, counter);
                }


                if (entity_manager->get_portal()) {
                    if (counter % entity_manager->get_portal()->get_frequency() == 0) {
                        entity_manager->spawn_enemis_by_portal(cells, *field);
                    }
                }

                
                

                if (entity_manager->allEnemiesDead()) {
                    status = victory;
                }
                if (hero->get_helpth() <= 0) {
                    status = loss;
                }

                counter++;
            } else {
                hero_controller->setSkipEnemyTurn(false);
            }
            
            
        }
            
    }
}


void Game_Manager::update() {
    if (status == loss) {
        status = game_over_dialog;
        logger_consol->log("You've lost! Game over.\n");
    }
    if (status == victory) {
        status = level_up_dialog;
        logger_consol->log("Level " + std::to_string(current_level) + " completed! Choose your upgrade.\n");
    }
}

void Game_Manager::handleDialogEvents(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (status == game_over_dialog) {
            if (event.key.code == sf::Keyboard::Y) {
                restartGame();
            } else if (event.key.code == sf::Keyboard::N) { 
                window->close();
            }
        } else if (status == level_up_dialog) {
            if (event.key.code == sf::Keyboard::Enter) {
                showLevelUpDialog();
            }
        } else if (status == level_complete) {
            if (event.key.code == sf::Keyboard::Enter) {
                nextLevel();
            }
        }
    }
}

void Game_Manager::renderLevelCompleteDialog() {
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window->draw(overlay);
    
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return;
    }
    
    sf::Text message;
    message.setFont(font);
    message.setString("Level " + std::to_string(current_level) + " Complete!\n\n"
                     "Moving to level " + std::to_string(current_level + 1) + "...\n\n"
                     "Half of your spells will be lost!\n\n"
                     "Press ENTER to continue");
    message.setCharacterSize(40);
    message.setFillColor(sf::Color::White);
    message.setPosition(window->getSize().x / 2 - 250, window->getSize().y / 2 - 100);
    
    window->draw(message);
}




void Game_Manager::restartGame() {
    
    delete entity_manager;
    delete hero_controller;
    delete field;
    
    current_level = 1;
    sf::Vector2u initialSize(15, 15);
    
    field = new Field(initialSize);
    entity_manager = new Entity_Manager(field);
    hero_controller = new HeroController(
        field, 
        *logger_consol,
        entity_manager
    );
    
    Hero* hero = entity_manager->get_hero();
    hero->set_helpth(entity_manager->get_hero()->get_max_helpth());
    hero->setTotalKill(0);
    hero->get_SpellHand().reset();
    
    status = game_running;
    counter = 0;
    show_restart_dialog = false;
    
    updateLevelText();
    
    logger_consol->log("New game started! Level " + std::to_string(current_level) + "\n");
}

void Game_Manager::nextLevel() { 
    current_level++;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> sizeDist(10, 25);
    
    int newSizeX = sizeDist(gen);
    int newSizeY = sizeDist(gen);

    field = new Field(sf::Vector2u(newSizeX, newSizeY));
    Hero* hero = entity_manager->get_hero();

    entity_manager->updateField(field, hero, current_level);
    hero_controller->update_controller(entity_manager, field);

    auto& spellHand = hero->get_SpellHand();
    spellHand.moveFirstHalfToUnindicted();

    status = game_running;
    counter = 0;

    updateLevelText();

    logger_consol->log("Level " + std::to_string(current_level) + " started! " +
                      "Field size: " + std::to_string(newSizeX) + "x" + std::to_string(newSizeY) + 
                      ", Kept " + std::to_string(spellHand.getVectorSpells().size()) + " spells.\n");
}


void Game_Manager::updateLevelText() {
    levelText.setString("Level: " + std::to_string(current_level));
    
    sf::Vector2u windowSize = window->getSize();
    levelText.setPosition(windowSize.x - levelText.getLocalBounds().width - 20, 
                         windowSize.y - levelText.getLocalBounds().height - 20);
}


void Game_Manager::renderDefeatDialog() {
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window->draw(overlay);
    
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return;
    }
    
    sf::Text message;
    message.setFont(font);
    message.setString("You have been defeated!\n\nRestart game?\n\nPress Y - Yes\nPress N - No");
    message.setCharacterSize(40);
    message.setFillColor(sf::Color::White);
    message.setPosition(window->getSize().x / 2 - 200, window->getSize().y / 2 - 100);
    
    window->draw(message);
}

void Game_Manager::render() {
    window->clear();
    render_system->drawField(*window, *field);
    render_system->drawEntities(*window, *entity_manager);
    entity_manager->get_hero()->drawSpellHand(*window);
    logger_consol->draw(*window);
    
    window->draw(levelText);

    if (status == game_over_dialog) {
        renderDefeatDialog();
    } else if (status == level_up_dialog) {
        renderLevelUpDialog();
    } else if (status == level_complete) {
        renderLevelCompleteDialog();
    }

    window->display();
}

void Game_Manager::initLevelText() {
    if (!levelFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return;
    }
    
    levelText.setFont(levelFont);
    levelText.setCharacterSize(30);
    levelText.setFillColor(sf::Color::White);
    levelText.setStyle(sf::Text::Bold);
    
    updateLevelText();
}


void Game_Manager::renderLevelUpDialog() {
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window->draw(overlay);
    
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return;
    }
    
    sf::Text message;
    message.setFont(font);
    message.setString("Level " + std::to_string(current_level) + " Complete!\n\n"
                     "You can now upgrade your hero!\n\n"
                     "Press ENTER to choose upgrade");
    message.setCharacterSize(40);
    message.setFillColor(sf::Color::White);
    message.setPosition(window->getSize().x / 2 - 300, window->getSize().y / 2 - 100);
    
    window->draw(message);
}

void Game_Manager::showLevelUpDialog() {
    LevelUpDialog dialog(*window, entity_manager->get_hero());
    int result = dialog.show();
    
    if (result != -1) {
        status = level_complete;
        logger_consol->log("Upgrade applied! Press ENTER to continue to next level.\n");
    } else {
        status = level_complete;
        logger_consol->log("No upgrade chosen. Press ENTER to continue to next level.\n");
    }
}