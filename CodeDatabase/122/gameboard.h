#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include <memory>

#include "cell.h"
#include "observable.h"
#include "observer.h"
#include "eventdispatcher.h"
#include "player.h"

class GameBoard : public Observable
{
public:
    GameBoard(int size=BoardConfig::Size);
    virtual ~GameBoard() = default;
    GameBoard(const GameBoard &other);
    GameBoard(GameBoard &&other);
    GameBoard &operator=(const GameBoard &other);
    GameBoard &operator=(GameBoard &&other);

    void generateCells();
    int getSize() const { return m_size; }
    void UpdateBoardAfterMove(int oldXPos, int oldYPos, int newXPos, int newYPos);
    void UpdateBoardAfterMove(int newXPos, int newYPos);
    void UpdateBoardAfterBlock(int newXPos, int newYPos);
    bool isValidCell(int newXPos, int newYPos) const;
    Cell& currentCell(int xPos, int yPos);
    void addObserver(std::unique_ptr<Observer> observer) override;
    void removeDestroyedObservers() override;
    void notifyObserver(int x, int y) override;
    void clearObservers();
    void registerDefaultObservers(std::vector<std::unique_ptr<Player>>& players);

private:
    int m_size{BoardConfig::Size};
    std::vector<std::vector<Cell>> m_board;
    std::vector<std::unique_ptr<Observer>> m_observers;
};

#endif // GAMEBOARD_H
