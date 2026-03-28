#ifndef VISITOR_H
#define VISITOR_H
// #include "gamemodel.h"
class GameModel;

class Visitor
{
private:
    GameModel* gm;
public:
    Visitor(GameModel* gm);
    bool visitEnemy(int x, int y, int damage, int radius, int width_attack);
    bool visitGame(int x, int y, int count);
    bool visitMap(int x, int y);
    bool visitMapForImprove(int x, int y);
    bool visitPlayer(int price);
    void visitButtons(int number_spell, int count);

};

#endif // VISITOR_H
