#include "StageManager.h"
#include <nlohmann/json.hpp>
#include <fstream>

StageManager::StageManager() {
    this->gl_ = new GameLogic(5, 5, 3);
    this->gl_->addObject(this->factory.create("Player"), Coordinates(0, 0, 1));
    this->upgrade_.setPlayer(this->gl_->getPlayer());
    this->gl_->getPlayer()->activateSpellBook();
    this->gl_->getPlayer()->getSpellBook()->addRandomSpell();

    this->controller_ = new GameController<InputHandler>();
    this->visualizer_ = new GameVisualizer<Renderer>(this->gl_);
    this->visualizer_->run();
}

StageManager::~StageManager() {
    delete this->visualizer_;
    delete this->gl_;
}

void StageManager::generateLevel() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> width_dist(10, 19);
    static std::uniform_int_distribution<int> height_dist(7, 12);

    int width = width_dist(gen);
    int height = height_dist(gen);

    std::uniform_int_distribution<int> x_dist(1, width / 2 - 1);
    std::uniform_int_distribution<int> y_dist(1, height / 2 - 1);

    this->gl_->clear();
    this->gl_->newMap(width, height);

    this->placeUnits("SlowTrap",
                     width * height * ((this->difficulty_ > 5) ? 0.4 : this->difficulty_ / 5.0 * 0.4),
                     0);
    this->placeUnits("Block", width * height * 0.15);
    while (this->gl_->addObject(this->gl_->getPlayer(),
                                Coordinates(x_dist(gen), y_dist(gen), 1)) == 1) {
    }
    int enemy_x = (int) width - this->gl_->getPlayer()->getPosition().x_ - 1;
    int enemy_y = (int) height - this->gl_->getPlayer()->getPosition().y_ - 1;
    Object *obj = this->gl_->getMap().getFromCell(Coordinates(enemy_x, enemy_y, 1));
    if (obj != nullptr) {
        this->gl_->removeObject(obj->getId());
    }
    obj = this->gl_->getMap().getFromCell(Coordinates(enemy_x, enemy_y + 1, 1));
    if (obj != nullptr) {
        this->gl_->removeObject(obj->getId());
    }
    this->gl_->addObject(this->factory.create("EnemyBase"), Coordinates(enemy_x, enemy_y, 1));
    this->placeUnits("Enemy", this->difficulty_);
    this->placeUnits("StrongerEnemy", this->difficulty_ - 1);
    this->placeUnits("SpellTower", this->difficulty_ / 2);
}

void StageManager::placeUnits(const std::string &objName, float edge, int layer) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> x_dist(0, static_cast<int>(this->gl_->getMap().getSize().x_));
    std::uniform_int_distribution<int> y_dist(0, static_cast<int>(this->gl_->getMap().getSize().y_));
    float count = 0;
    while (count < edge) {
        const int posted = this->gl_->addObject(
            this->factory.create(objName),
            Coordinates(x_dist(gen), y_dist(gen), layer)
        );
        if (!posted) {
            count++;
        }
    }
}

void StageManager::startGame() {
    Player *pl = this->gl_->getPlayer();
    bool exitToMenu = false;
    while (this->visualizer_->isOpen() && !exitToMenu) {
        sf::Event event{};
        while (this->visualizer_->pollEvent(event)) {
            CommandType event_type = controller_->parseEvent(event);
            if (event_type == CommandType::CloseGame) {
                this->endGame();
                return;;
            }
            if (event_type == CommandType::Exit) {
                exitToMenu = true;
                break;
            }

            if (!pl->isAlive()) {
                this->visualizer_->switchOver(true);
                continue;
            }

            if (event_type == CommandType::SaveGame) {
                try {
                    this->save();
                    std::cout << "Game saved successfully!" << std::endl;
                } catch (const std::exception &e) {
                    std::cerr << "Save error: " << e.what() << std::endl;
                }
            }
            if (event_type == CommandType::LoadGame) {
                try {
                    this->load();
                    pl = this->gl_->getPlayer();
                    std::cout << "Game load successfully!" << std::endl;
                } catch (const std::exception &e) {
                    std::cerr << "Load error: " << e.what() << std::endl;
                }
            }
            if (pl->isBookOpen()) {
                int miss = 1;
                switch (event_type) {
                    case CommandType::MoveUp:
                        pl->getSpellBook()->changeTargets(0, -1);
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::MoveDown:
                        pl->getSpellBook()->changeTargets(0, 1);
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::MoveLeft:
                        pl->getSpellBook()->changeTargets(-1, 0);
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::MoveRight:
                        pl->getSpellBook()->changeTargets(1, 0);
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::ArrowLeft:
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->changeSpell(-1);
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::ArrowRight:
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->changeSpell(1);
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::ArrowUp:
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->changeSpell(-1);
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::ArrowDown:
                        pl->getSpellBook()->hideTargets();
                        pl->getSpellBook()->changeSpell(1);
                        pl->getSpellBook()->showTargets();
                        break;
                    case CommandType::ToggleBook:
                        pl->switchBookState();
                        pl->getSpellBook()->hideTargets();
                        break;
                    case CommandType::ToggleSpellInfo:
                        this->visualizer_->switchSpellInfoState();
                        break;
                    case CommandType::CastOrConfirm:
                        miss = pl->getSpellBook()->castSpell();
                        if (!miss) {
                            pl->getSpellBook()->hideTargets();
                            this->gl_->update();
                            pl->getSpellBook()->updateTargets();
                            pl->getSpellBook()->showTargets();
                        }
                        break;
                    default:
                        break;
                }
            } else {
                switch (event_type) {
                    case CommandType::MoveUp:
                        pl->move(0, -1);
                        this->gl_->update();
                        break;
                    case CommandType::MoveDown:
                        pl->move(0, 1);
                        this->gl_->update();
                        break;
                    case CommandType::MoveLeft:
                        pl->move(-1, 0);
                        this->gl_->update();
                        break;
                    case CommandType::MoveRight:
                        pl->move(1, 0);
                        this->gl_->update();
                        break;
                    case CommandType::ArrowLeft:
                        pl->findTarget(-1, 0);
                        this->gl_->update();
                        break;
                    case CommandType::ArrowRight:
                        pl->findTarget(1, 0);
                        this->gl_->update();
                        break;
                    case CommandType::ArrowUp:
                        pl->findTarget(0, -1);
                        this->gl_->update();
                        break;
                    case CommandType::ArrowDown:
                        pl->findTarget(0, 1);
                        this->gl_->update();
                        break;
                    case CommandType::ChangeWeapon:
                        pl->changeWeapon();
                        this->gl_->update();
                        break;
                    case CommandType::ToggleBook:
                        pl->switchBookState();
                        pl->getSpellBook()->updateTargets();
                        pl->getSpellBook()->showTargets();
                        break;
                    default:
                        break;
                }
            }
        }

        if (this->gl_->getScore() > 11) {
            this->gl_->addScore(-12);
            this->nextLevel();
        }
        this->visualizer_->renderGame();
    }

    if (this->visualizer_->isOpen()) {
        this->mainMenu();
    }
}


void StageManager::endGame() const {
    this->visualizer_->close();
}

void StageManager::nextLevel() {
    this->gl_->getPlayer()->healFull();
    this->gl_->getPlayer()->getSpellBook()->removeRandomSpells();
    this->difficulty_++;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::pair<int, int> upg;
    static std::uniform_int_distribution<int> upg_dist(0, 3);
    upg.first = upg_dist(gen);
    upg.second = upg.first;
    while (upg.first == upg.second) {
        upg.second = upg_dist(gen);
    }

    std::vector<std::string> options = {
        this->upgrades_[upg.first],
        this->upgrades_[upg.second]
    };

    int choice = this->visualizer_->renderMenu(options);

    if (choice == 0) {
        this->upgrade_.applyUpgrade(this->upgrades_[upg.first]);
    } else if (choice == 1) {
        this->upgrade_.applyUpgrade(this->upgrades_[upg.second]);
    } else {
        this->endGame();
        return;
    }

    this->gl_->clear();
    this->generateLevel();
}


void StageManager::save() const {
    nlohmann::json savefile;
    savefile["difficulty"] = this->difficulty_;
    savefile["score"] = this->gl_->getScore();
    Coordinates map_size = this->gl_->getMap().getSize();
    savefile["map_size"] = {map_size.x_, map_size.y_, map_size.layer_};

    this->gl_->saveJson(&savefile);

    const std::string temp_filename = "save.json.tmp";
    std::ofstream out(temp_filename);

    if (out.is_open()) {
        out << savefile.dump(4);
        out.close();

        if (std::rename(temp_filename.c_str(), "save.json") != 0) {
            std::remove(temp_filename.c_str());
            throw std::runtime_error("Failed to rename temporary save file");
        }
    } else {
        throw std::runtime_error("Failed to create temporary save file");
    }
}

int StageManager::load() {
    std::ifstream in("save.json");
    if (!in.is_open()) {
        throw std::runtime_error("File not found");
    }

    nlohmann::json savefile;
    try {
        in >> savefile;
    } catch (...) {
        throw std::runtime_error("Save file is corrupted");
    }

    if (!savefile.contains("map_size")) {
        throw std::runtime_error("Can't find the map");
    }

    if (!savefile.contains("difficulty")) {
        throw std::runtime_error("Can't find the difficulty");
    }

    if (!savefile.contains("objects")) {
        throw std::runtime_error("Can't find the objects");
    }

    if (!savefile.contains("score")) {
        throw std::runtime_error("Can't find the score");
    }

    this->gl_->clear();
    this->difficulty_ = savefile["difficulty"];
    this->gl_->newMap(savefile["map_size"][0], savefile["map_size"][1]);

    this->gl_->setScore(savefile["score"]);

    for (auto i: savefile["objects"]) {
        if (!i.is_object()) {
            continue;
        }
        try {
            Object *obj = this->factory.create(i["name"]);
            if (obj != nullptr) {
                this->gl_->addObject(obj, Coordinates(i["x"], i["y"], i["layer"]));
                obj->loadFromJson(&i);
            }
        } catch (...) {
            throw std::runtime_error("Save file is corrupted");
        }
    }
    this->upgrade_.setPlayer(this->gl_->getPlayer());
    in.close();
    return 0;
}

void StageManager::mainMenu() {
    bool chosen = false;

    while (!chosen) {
        std::vector<std::string> options = {"New game", "Continue"};
        int choice = visualizer_->renderMenu(options);

        if (choice == 0) {
            this->difficulty_ = 1;
            this->gl_->setScore(0);

            this->gl_->clear();
            this->generateLevel();

            chosen = true;
        } else if (choice == 1) {
            try {
                this->load();
                chosen = true;
            } catch (const std::exception &e) {
                std::cerr << "Load error: " << e.what() << std::endl;
            }
        } else {
            this->endGame();
            return;
        }
    }

    this->startGame();
}
