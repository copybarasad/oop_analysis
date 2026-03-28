
#include <QPaintEvent>
#include <QPainter>
#include "observer.h"
#include "gamecontroller.h"
#include <QPushButton>
#include "handlerevent.h"
#include <vector>
#include "constants.h"

class GameView : public QWidget, public Observer
{
    Q_OBJECT
private:
    QPixmap bolotoPixmap;
    QPixmap userPixmap;
    QPixmap enemyPixmap;
    QPixmap plantPixmap;
    QPixmap towerPixmap;
    QPixmap mapPixmap;
    QPixmap trapPixmap;
    QPixmap buildPixmap;
    QPixmap leonPixmap;
    GameModel* gm;
    HandlerEvent* input;
protected:
    void  keyPressEvent(QKeyEvent* e) override;
    void paintEvent(QPaintEvent*) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
public:
    GameView(GameModel* gm);
    void drawing_game(QPainter& painter, GameModel& myGame);
    void drawing_menu(QPainter& painter);
    void drawing_upgrade_menu(QPainter& painter);
    void drawing_user(QPainter& painter, GameModel& gm);
    void drawing_enemy(QPainter& painter, GameModel& gm);
    void drawing_map(QPainter& painter, GameModel& gm);
    void drawing_tower(QPainter& painter, GameModel& gm);
    void drawing_zone(QPainter& painter, GameModel& gm);
    void drawing_info(QPainter& painter, GameModel& gm);
    void drawing_gameOver(QPainter& painter);
    void drawing_leon(QPainter& painter, GameModel& gm);
    void load_sourse();
    void show_buttons(GameModel& gm);
    void set_handler(HandlerEvent* input);
    void updating() override;
};

