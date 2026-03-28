#include "GameClass/GameClass.hpp"

GameClass::GameClass() {
    this->state = GameState::Menu;
}

GameState GameClass::getState() {
    return state;
}

void GameClass::loadLevel(Motion motion) {
    try {
        this->shootedFields.clear();

        LevelManager lm;

        if (motion == Motion::NewGame) {
            stats = lm.loadLevelStatistic(1);
            CastC = std::make_unique<CastController>(stats.lvl);
            CastC->addSpell();
            MapC = std::make_unique<MapController>(stats.lvl);
            TowerC = std::make_unique<TowerController>(stats.lvl, *MapC);
            EnemiesC = std::make_unique<EnemyController>(stats.lvl, *MapC);

            player = std::make_unique<Player>(lm.loadPlayer(stats.lvl));
            
            MapC->getField(player->getPosition())->setStatus(FieldType::PLAYER);

            this->state = GameState::OnWalk;
        }
        if (motion == Motion::LoadSave) {
            stats = lm.loadLevelStatistic(0);

            CastC = std::make_unique<CastController>(0);
            MapC = std::make_unique<MapController>(stats.lvl);
            TowerC = std::make_unique<TowerController>(0, *MapC);
            EnemiesC = std::make_unique<EnemyController>(0, *MapC);
        
            player = std::make_unique<Player>(lm.loadPlayer(0));
            MapC->getField(player->getPosition())->setStatus(FieldType::PLAYER);

            this->state = GameState::OnWalk;
            CastC->executeContracts(*MapC, *EnemiesC, *TowerC, *player);
        }
        if (motion == Motion::FinishLevel) {
            if (stats.lvl + 1 > 3) {
                state = GameState::Win;
                return;
            }

            stats = lm.loadLevelStatistic(stats.lvl + 1);

            MapC = std::make_unique<MapController>(stats.lvl);
            TowerC = std::make_unique<TowerController>(stats.lvl, *MapC);
            EnemiesC = std::make_unique<EnemyController>(stats.lvl, *MapC);

            player->setHealthPoints(player->getMaxHP());
            player->setPosition({0, 0});
            MapC->getField(player->getPosition())->setStatus(FieldType::PLAYER);
            CastC->goToNextLevel();

            this->state = GameState::Improve;
        }
    } catch(FileException& e) {
        std::cerr << "Ошибка при загрузке уровня: " << e.what() << '\n';
        std::cerr << "Возврат в главное меню" << '\n';
        this->state = GameState::Menu;
    }
}


void GameClass::saveGame() {
    try {
        LevelManager lm;
        GameDTO dto = toDTO();
        lm.saveLevelMap(dto.map);
        lm.saveLevelTowers(dto.towers);
        lm.saveLevelEnemies(dto.enemies);
        lm.savePlayer(dto.player);
        lm.saveHand(dto.cast.hand);
        lm.saveLevelStatistic(dto.stats);
        lm.saveTraps(dto.cast.traps);
    }
    catch(FileException& e) {
        std::cerr << std::string("Игра не сохранилась:") + e.what() << '\n';
    }
}


GameDTO GameClass::toDTO() {
    GameDTO dto;
    if (state == GameState::OnCast || state == GameState::OnWalk) {
        dto.map = MapC->toDTO();
        dto.enemies = EnemiesC->toDTO();
        dto.player = player->toDTO();
        dto.towers = TowerC->toDTO();
        dto.shooted = shootedFields;
        dto.cast = CastC->toDTO();
        dto.stats = this->stats;
    }

    dto.state = this->state;
    return dto;
}


void GameClass::gameLoop(Motion motion) {
    
    handleMotion(motion);
    AIsteps();
}


void GameClass::handleMotion(Motion motion) {
    shootedFields.clear();
    switch (motion)
    {
    case Motion::SaveGame:
        saveGame();
        break;
    case Motion::FinishLevel:
    case Motion::LoadSave:
    case Motion::NewGame:
        loadLevel(motion);
        break;
    case Motion::UpgradeHp:
    case Motion::UpgradeDamage:
    case Motion::UpgradeTargetS:
    case Motion::UpgradeAreaS:
    case Motion::UpgradeTrapS:
        improvePlayer(motion);
        break;
    case Motion::MoveUp:
    case Motion::MoveDown:
    case Motion::MoveLeft:
    case Motion::MoveRight:
    case Motion::ChangeForm:
        playerStep(motion);
        break;
    case Motion::StartCast1:
    case Motion::StartCast2:
    case Motion::StartCast3:
    case Motion::StartCast4:
    case Motion::StartCast5:
        state = GameState::OnCast;
        CastC->startCast(motion, player->getPosition(), MapC->getHeight(), MapC->getWidth());
        break;
    case Motion::MoveUpCast:
    case Motion::MoveDownCast:
    case Motion::MoveLeftCast:
    case Motion::MoveRightCast:
        CastC->moveTarget(motion);
        break;
    case Motion::CancelCast:
        state = GameState::OnWalk;
        break;
    case Motion::ConfirmCast:
        CastC->confirm();
        state = GameState::OnWalk;
        EnemiesC->makeActive();
        TowerC->makeActive();
        CastC->executeContracts(*MapC, *EnemiesC, *TowerC, *player);
        break;
    default:
        break;
    }
}


void GameClass::AIsteps() {
    if (EnemiesC->isActive() && TowerC->isActive()) {
        EnemiesC->makeAllStep(*MapC, *player, stats);
        TowerC->makeAllStep(*MapC, *EnemiesC, *player, stats);
        if (player->getHealthPoints() <= 0) {
            state = GameState::Lose;
            return;
        }

        if (stats.bonus) {
            stats.bonus = false;
            CastC->addSpell();
        }
        CastC->executeContracts(*MapC, *EnemiesC, *TowerC, *player);
    }
}


void GameClass::improvePlayer(Motion motion) {
    if (motion == Motion::UpgradeHp) {
        player->setMaxHP(player->getMaxHP() + 250);
        player->setHealthPoints(player->getMaxHP());
    } else if (motion == Motion::UpgradeDamage) {
        player->setDamagePoints(player->getDamagePoints() + 40);
    } else if (motion == Motion::UpgradeAreaS) {
        CastC->upgradeSpell(SpellType::AoESpell);
    } else if (motion == Motion::UpgradeTargetS) {
        CastC->upgradeSpell(SpellType::TargetSpell);
    } else if (motion == Motion::UpgradeTrapS) {
        CastC->upgradeSpell(SpellType::TrapSpell);
    }
    state = GameState::OnWalk;
}


void GameClass::playerStep(Motion motion) {
    if (motion == Motion::ChangeForm) {
        player->changeRangeForm();
    }

    else if (player->isDisabled()) {
        player->changeDisabled();
    }

    else {
        Coordinates delta;

        switch (motion)
        {
        case Motion::MoveUp:
            delta = {0, -1};
            break;
        case Motion::MoveDown:
            delta = {0, 1};
            break;
        case Motion::MoveLeft:
            delta = {-1, 0};
            break;
        case Motion::MoveRight:
            delta = {1, 0};
            break;
        default:
            break;
        }

        if (player->isRangeForm()) {
            handleRangeStep(delta);
        } else {
            handleMeleeStep(delta);
        }
    }

    EnemiesC->makeActive();
    TowerC->makeActive();
}


void GameClass::handleRangeStep(Coordinates delta) {
    int mapW = MapC->getWidth();
    int mapH = MapC->getHeight();

    Coordinates bulletPos = player->getPosition();
    bulletPos.x += delta.x;
    bulletPos.y += delta.y;
    
    bool stop = false;
    while (bulletPos.x < mapW && bulletPos.y < mapH && bulletPos.x >= 0 && bulletPos.y >= 0 && !stop) {
        FieldType status = MapC->getField(bulletPos)->getStatus();

        switch (status) {
            case FieldType::ENEMY:
                EnemiesC->getEnemy(bulletPos)->minusHealthPoints(player->getDamagePoints());
                stop = true;
                break;
            case FieldType::ATTACK_TOWER:
                TowerC->getAttackTower(bulletPos)->minusHealthPoints(player->getDamagePoints());
                stop = true;
                break;
            case FieldType::FIELD_BLOCK:
            case FieldType::SPAWN_TOWER:
                stop = true;
                break;
            default:
                shootedFields.push_back({bulletPos.x, bulletPos.y});
                break;
        }
        
        bulletPos.x += delta.x;
        bulletPos.y += delta.y;
    }

    Coordinates playerCoord = player->getPosition();
    Coordinates newPos = {playerCoord.x + delta.x, playerCoord.y + delta.y};
    FieldType moveStatus = MapC->getField(newPos)->getStatus();

    switch (moveStatus) {
        case FieldType::FIELD_SLOW:
            movePlayer(playerCoord, newPos);
            player->changeDisabled();
            break;
        case FieldType::ENEMY:
        case FieldType::ATTACK_TOWER:
            break;
        default:
            movePlayer(playerCoord, newPos);
            break;
    }
}


void GameClass::handleMeleeStep(Coordinates delta) {
    Coordinates playerCoord = player->getPosition();
    Coordinates newPos = {playerCoord.x + delta.x, playerCoord.y + delta.y};
    FieldType moveStatus = MapC->getField(newPos)->getStatus();

    switch (moveStatus) {
        case FieldType::FIELD_SLOW:
            movePlayer(playerCoord, newPos);
            player->changeDisabled();
            break;
        case FieldType::ENEMY:
            EnemiesC->getEnemy(newPos)->minusHealthPoints(player->getDamagePoints());
            break;
        case FieldType::ATTACK_TOWER:
            TowerC->getAttackTower(newPos)->minusHealthPoints(player->getDamagePoints());
            break;
        default:
            movePlayer(playerCoord, newPos);
    }
}


void GameClass::movePlayer(Coordinates oldPos, Coordinates newPos) {
    MapC->getField(oldPos)->toInitStatus();
    MapC->getField(newPos)->setStatus(FieldType::PLAYER);
    player->setPosition(newPos);
}


std::vector<Motion> GameClass::getAllowedMotions() {
    std::vector<Motion> motoions;


    if (state == GameState::OnWalk || state == GameState::OnCast) {
        if (stats.points >= stats.endLevelPoints) motoions.push_back(Motion::FinishLevel);
        motoions.push_back(Motion::SaveGame);
        motoions.push_back(Motion::LoadSave);
        std::vector<Coordinates> allowed;

        if (this->state == GameState::OnWalk) {
            motoions.push_back(Motion::ChangeForm);
            int count = CastC->handSize();

            for (int i = 1; i <= count; i++) {
                switch (i) {
                    case 1: motoions.push_back(Motion::StartCast1); break;
                    case 2: motoions.push_back(Motion::StartCast2); break;
                    case 3: motoions.push_back(Motion::StartCast3); break;
                    case 4: motoions.push_back(Motion::StartCast4); break;
                    case 5: motoions.push_back(Motion::StartCast5); break;
                }
            }

            allowed = MapC->getAllowedSteps(player->getPosition(), FieldType::PLAYER);
        } else if (this->state == GameState::OnCast) {
            if (CastC->canUseHere(MapC->getField(CastC->getTarget())->getStatus())) {
                motoions.push_back(Motion::ConfirmCast);
            }
            motoions.push_back(Motion::CancelCast);
            allowed = CastC->getAllowedSteps();
        }

        for (auto& step : allowed) {
            if (step.x == 0) {  
                if (step.y == 1) motoions.push_back(this->state == GameState::OnWalk ? Motion::MoveDown : Motion::MoveDownCast);
                else motoions.push_back(this->state == GameState::OnWalk ? Motion::MoveUp : Motion::MoveUpCast);
            }
            if (step.x == 1) motoions.push_back(this->state == GameState::OnWalk ? Motion::MoveRight : Motion::MoveRightCast);
            else if (step.x == -1) motoions.push_back(this->state == GameState::OnWalk ? Motion::MoveLeft : Motion::MoveLeftCast);
        }
    }

    if (state == GameState::Menu || state == GameState::Win || state == GameState::Lose) {
        motoions.push_back(Motion::NewGame);
        motoions.push_back(Motion::LoadSave);
    }

    if (state == GameState::Improve) {
        motoions.push_back(Motion::UpgradeHp);
        motoions.push_back(Motion::UpgradeAreaS);
        motoions.push_back(Motion::UpgradeDamage);
        motoions.push_back(Motion::UpgradeTargetS);
        motoions.push_back(Motion::UpgradeTrapS);
    }


    return motoions;
}
