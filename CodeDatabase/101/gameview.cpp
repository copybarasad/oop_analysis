#include "gameview.h"

GameView::GameView(GameModel* gm) {
    this->gm = gm;
    resize(DOT_WIDTH * (FIELD_WIDTH), DOT_HEIGHT * (FIELD_HEIGHT + 3));
    this->setWindowTitle("Game");
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    input = nullptr;
    load_sourse();

    show_buttons(*gm);

    gm->get_buttons().updateVisibility(gm->get_current_option());

    update();
}

void GameView::load_sourse()
{
    QPixmap image_map(":/images/images/map.jpg");
    mapPixmap = image_map.copy();

    QPixmap image_boloto(":/images/images/boloto1.jpg");
    bolotoPixmap = image_boloto.copy();
    bolotoPixmap.setMask(image_boloto.createMaskFromColor(Qt::white));

    QPixmap image_user(":/images/images/user1.jpg");
    userPixmap = image_user.copy();
    userPixmap.setMask(image_user.createMaskFromColor(Qt::white));

    QPixmap image_enemy(":/images/images/enemy.jpg");
    enemyPixmap = image_enemy.copy();
    enemyPixmap.setMask(image_enemy.createMaskFromColor(Qt::white));


    QPixmap image_plant(":/images/images/plant.jpg");
    plantPixmap = image_plant.copy();
    plantPixmap.setMask(image_plant.createMaskFromColor(Qt::white));

    QPixmap tower(":/images/images/tower1.jpg");
    towerPixmap = tower.copy();
    towerPixmap.setMask(tower.createMaskFromColor(Qt::white));

    QPixmap trap(":/images/images/trap.jpg");
    trapPixmap = trap.copy();
    trapPixmap.setMask(trap.createMaskFromColor(Qt::white));

    QPixmap build(":/images/images/build.png");
    buildPixmap = build.copy();
    buildPixmap.setMask(build.createMaskFromColor(Qt::white));

    QPixmap ln(":/images/images/leon.jpg");
    leonPixmap = ln.copy();
    leonPixmap.setMask(ln.createMaskFromColor(Qt::white));
}

void GameView::show_buttons(GameModel &gm)
{
    auto& buttons_ref = gm.get_buttons().get_my_buttons();

    for (auto it = buttons_ref.begin(); it != buttons_ref.end(); ++it) {
        auto* btn = *it;
        if (btn) {
            btn->setParent(this);
            btn->show();
        }
    }
}


void GameView::set_handler(HandlerEvent *input)
{
    this->input = input;
}

void GameView::updating()
{
    update();
}



void GameView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    OptionGame currentOption = gm->get_current_option();

    if (currentOption == upgradeMenu) {
        drawing_upgrade_menu(painter);
        return;
    }

    if (currentOption == mainMenu || currentOption == pauseMenu) {
        drawing_menu(painter);
    } else {
        drawing_game(painter, *gm);
    }
}



void GameView::drawing_upgrade_menu(QPainter& painter) {
    painter.fillRect(rect(), Qt::darkBlue);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 20));

    painter.drawText(rect(), Qt::AlignCenter,
                     QString("LEVEL %1 COMPLETE!\n\n"
                             "Skill Points: %2\n\n"
                             "1 - +20 Health\n"
                             "2 - +20%% Damage\n"
                             "3 - +25%% Spells\n\n"
                             "4 - Continue to level %3")
                         .arg(gm->get_current_level())
                         .arg(gm->get_skill_points())
                         .arg(gm->get_current_level() + 1));
}

void GameView::drawing_menu(QPainter& painter) {

    painter.fillRect(rect(), Qt::darkGray);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 24, QFont::Bold));

    if (gm->is_level_completed()) {
        painter.drawText(rect(), Qt::AlignCenter,
                         QString("LEVEL %1 COMPLETED!").arg(gm->get_current_level()));

        painter.setFont(QFont("Arial", 16, QFont::Normal));
        painter.drawText(rect().adjusted(0, 100, 0, 0), Qt::AlignCenter,
                         QString("Skill Points: %1\n\n"
                                 "1 - +20 Max Health\n"
                                 "2 - +20%% Damage\n"
                                 "3 - +25%% Spell Power\n\n"
                                 "4 - Continue to Level %2")
                             .arg(gm->get_skill_points())
                             .arg(gm->get_current_level() + 1));
        return;
    }
    painter.fillRect(rect(), Qt::darkGray);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 24, QFont::Bold));

    painter.setFont(QFont("Arial", 16, QFont::Normal));

    if (gm->get_current_option() == mainMenu) {
        painter.drawText(rect().adjusted(0, 100, 0, 0), Qt::AlignCenter,
                         "1 - НОВАЯ ИГРА\n"
                         "2 - ЗАГРУЗИТЬ\n"
                         "3 - ВЫХОД");
    } else if (gm->get_current_option() == pauseMenu) {
        painter.drawText(rect().adjusted(0, 100, 0, 0), Qt::AlignCenter,
                         "1 - ПРОДОЛЖИТЬ\n"
                         "2 - СОХРАНИТЬ\n"
                         "3 - В ГЛАВНОЕ МЕНЮ\n"
                         "ESC - НАЗАД");
    }
}



void GameView::drawing_game(QPainter& painter, GameModel& gm)
{
    if (gm.get_player().get_health() != 0 && gm.get_inGame()) {
        drawing_map(painter, gm);
        if(!gm.get_leons().empty()) drawing_leon(painter, gm);
        drawing_user(painter, gm);

        if(gm.get_player().get_zoneX() != 100){
            drawing_zone(painter, gm);
        }
        gm.get_player().set_zoneX(100);

        if(gm.get_enemy().get_alive()){
            drawing_enemy(painter, gm);
        }
        if(gm.get_tower().get_alive() == true){
            drawing_tower(painter, gm);
        }
        drawing_info(painter, gm);
    }
    else if(gm.get_player().get_health() == 0){
        drawing_gameOver(painter);
        gm.change_inGame(false);
    }
}

void GameView::drawing_enemy(QPainter& painter, GameModel& gm)
{
    int e_x = gm.get_enemy().get_coord_x();
    int e_y = gm.get_enemy().get_coord_y();

    painter.drawPixmap(QRect(e_x * gm.get_cell_height(), e_y * gm.get_cell_width(), gm.get_height(), gm.get_width()), enemyPixmap);
    if(gm.get_flagAtack() == 1){
        painter.setBrush(QColor(255, 0, 0, 50));
        painter.drawEllipse((e_x-1) * gm.get_cell_height(), (e_y-1) * gm.get_cell_width(), gm.get_height() * 3, gm.get_width() * 3);
    }
}

void GameView::drawing_zone(QPainter& painter, GameModel& gm)
{
    painter.setBrush(QColor(0, 255, 0, 50));
    painter.drawRect(gm.get_player().get_zoneX() * gm.get_cell_height(), gm.get_player().get_zoneY() * gm.get_cell_width(), gm.get_height() * 3, gm.get_width() * 3);
}

void GameView::drawing_tower(QPainter& painter, GameModel& gm)
{
    int left_x = gm.get_tower().get_left_x();
    int down_y = gm.get_tower().get_down_y();

    painter.drawPixmap(QRect(left_x * gm.get_cell_height(), down_y * gm.get_cell_width(), gm.get_height() * 3, gm.get_width() * 3), towerPixmap);
}

void GameView::drawing_info(QPainter& painter,  GameModel& gm)
{
    painter.setFont(QFont("Arial", 15));
    painter.setPen(Qt::green);
    painter.drawText(10, 20, QString("Score %1").arg(gm.get_score()));
    painter.drawText(10, 40, QString("Health %1").arg(gm.get_player().get_health()));
    painter.drawText(10, 60, QString("Enemy %1").arg(gm.get_enemy().get_health()));
    painter.drawText(10, 80, QString("Money %1").arg(gm.get_player().get_money()));
    painter.drawText(10, 100, QString("Tower %1").arg(gm.get_tower().get_health()));
}

void GameView::drawing_gameOver(QPainter& painter)
{
    painter.drawText(painter.window(), Qt::AlignCenter, "Game Over");
}

void GameView::drawing_leon(QPainter &painter, GameModel& gm)
{
    for(Leon* l : gm.get_leons()){
        if(l != nullptr){
            int x = l->get_leon_x();
            int y = l->get_leon_y();
            qDebug() << x << y;
            painter.drawPixmap(QRect(x * gm.get_cell_height(), y * gm.get_cell_width(), gm.get_height(), gm.get_width()), leonPixmap);
        }

    }
}

void GameView::drawing_user(QPainter& painter, GameModel& gm)
{
    int x = gm.get_player().get_x();
    int y = gm.get_player().get_y();

    painter.setBrush(QColor(0, 0, 255, 50));
    painter.drawEllipse((x-1) * gm.get_cell_height() , (y-1) * gm.get_cell_width() , gm.get_height() * 3, gm.get_width() * 3);
    painter.drawPixmap(QRect(x * gm.get_cell_height(), y * gm.get_cell_width(), gm.get_height(), gm.get_width()), userPixmap);
}

void GameView::drawing_map(QPainter& painter, GameModel& gm)
{
    painter.drawPixmap(QRect(0 , 0, gm.get_cell_height() * gm.get_height(), DOT_WIDTH * gm.get_width()), mapPixmap);
    for(int i = 0; i < gm.get_height(); i++){
        for(int j = 0; j < gm.get_width(); j++){
            if(gm.get_map().get_point(i, j).get_point_opt() == lock){
                painter.drawPixmap(QRect(i * gm.get_cell_height(), j * gm.get_cell_width(), gm.get_height(), gm.get_width()), plantPixmap);
            }
            if(gm.get_map().get_point(i, j).get_point_opt() == slow){
                painter.drawPixmap(QRect(i * gm.get_cell_height(), j* gm.get_cell_width(), gm.get_height(), gm.get_width()), bolotoPixmap);
            }
            if(gm.get_map().get_point(i, j).get_point_opt() == trap){
                painter.drawPixmap(QRect(i * gm.get_cell_height(), j* gm.get_cell_width(), gm.get_height(), gm.get_width()), trapPixmap);
            }
            if(gm.get_map().get_point(i, j).get_point_opt() == build){
                painter.drawPixmap(QRect(i * gm.get_cell_height(), j* gm.get_cell_width(), gm.get_height(), gm.get_width()), buildPixmap);
            }

        }
    }
}

void GameView::mouseMoveEvent(QMouseEvent* e) {
    if (input) {
        std::vector<QPushButton*> buttons = gm->get_buttons().get_my_buttons();
        input->mouseMoveHandler(e);
    }
}

void GameView::dragEnterEvent(QDragEnterEvent* e) {
    if (e->mimeData()->hasText()) {
        e->acceptProposedAction();
    }
    if (input) {
        input->mouseDragHandler(e);
    }
}

void GameView::dropEvent(QDropEvent* e) {

    if (input) {
        input->mouseDropHandler(e);
    }
    e->acceptProposedAction();
}

void GameView::keyPressEvent(QKeyEvent* e) {
    if (input) {
        input->keyboardHandler(e);
    }
}



