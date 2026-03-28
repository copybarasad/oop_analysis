#include "gameboard.h"
#include "healthpoint.h"

GameBoard::GameBoard(int size) : m_size(size)
{
    m_board.resize(m_size, std::vector<Cell>(m_size));
}

GameBoard::GameBoard(const GameBoard &other)
    : m_size(other.m_size),
    m_board(other.m_board),
    m_observers()
{}

void GameBoard::generateCells()
{
    std::uniform_int_distribution<> die{0, m_size - 1};
    int count = m_size * m_size / 15;

    for (int i = 0; i < count; i++)
    {
        int xSlow = die(MyRandom::Mersenne);
        int ySlow = die(MyRandom::Mersenne);

        int xBlock = die(MyRandom::Mersenne);
        int yBlock = die(MyRandom::Mersenne);

        m_board[xSlow][ySlow].setType(CellType::SLOWED);
        m_board[xBlock][yBlock].block();
        EventDispatcher::instance().notify(NewCellEvent(CellType::SLOWED, {xSlow, ySlow}));
        EventDispatcher::instance().notify(NewCellEvent(CellType::BLOCKED, {xBlock, yBlock}));
    }
    m_board[0][m_size-1].original();
    m_board[m_size-1][2].original();
}

GameBoard& GameBoard::operator=(const GameBoard &other)
{
    if (this != &other)
    {
        m_size = other.m_size;
        m_board = other.m_board;
        m_observers.clear();
    }
    return *this;
}

GameBoard::GameBoard(GameBoard &&other)
    : m_size(other.m_size),
    m_board(std::move(other.m_board)),
    m_observers(std::move(other.m_observers))
{
    other.m_size = 0;
}


GameBoard &GameBoard::operator=(GameBoard &&other)
{
    if (this != &other) {
        m_size = other.m_size;
        m_board = std::move(other.m_board);
        m_observers = std::move(other.m_observers);

        other.m_size = 0;
    }
    return *this;
}

void GameBoard::UpdateBoardAfterMove(int oldXPos, int oldYPos, int newXPos, int newYPos)
{
    m_board[oldXPos][oldYPos].free();
    m_board[newXPos][newYPos].occupy();

    if (m_board[newXPos][newYPos].getType() == CellType::POISON_TRAP ||
        m_board[newXPos][newYPos].getType() == CellType::HEALTH_POINT)
    {
        notifyObserver(newXPos, newYPos);
    }
}

void GameBoard::UpdateBoardAfterMove(int newXPos, int newYPos)
{
    m_board[newXPos][newYPos].occupy();
}

void GameBoard::UpdateBoardAfterBlock(int newXPos, int newYPos)
{
    m_board[newXPos][newYPos].block();
}

bool GameBoard::isValidCell(int newXPos, int newYPos) const //имяяя
{
    if (newXPos < 0 || newXPos >= m_size || newYPos < 0 || newYPos >= m_size)
    {
        return false;
    }

    if (!(m_board[newXPos][newYPos].getState() == Cell::FREE) || m_board[newXPos][newYPos].getType() == CellType::BLOCKED)
    {
        return false;
    }
    return true;
}

Cell& GameBoard::currentCell(int xPos, int yPos)
{
    return m_board[xPos][yPos];
}

void GameBoard::addObserver(std::unique_ptr<Observer> observer)
{
    m_observers.push_back(std::move(observer));
}

void GameBoard::removeDestroyedObservers()
{
    m_observers.erase(
        std::remove_if(m_observers.begin(), m_observers.end(),
                       [](const std::unique_ptr<Observer>& observer){
                        return observer->isDestroyed();
                        }),
        m_observers.end()
    );
}

void GameBoard::notifyObserver(int x, int y)
{
    for (auto& observer : m_observers) {
        if (observer->handleEvent(x, y))
        {
            m_board[x][y].original();
            EventDispatcher::instance().notify(NewCellEvent(CellType::ORIGINAL, {x, y}));
        }
    }
}

void GameBoard::clearObservers()
{
    m_observers.clear();
}

void GameBoard::registerDefaultObservers(std::vector<std::unique_ptr<Player>>& players)
{
    currentCell(BoardConfig::HealthPointPos.first, BoardConfig::HealthPointPos.second).setType(CellType::HEALTH_POINT);
    auto observer = std::make_unique<HealthPoint>(players, BoardConfig::HealthPointPos.first, BoardConfig::HealthPointPos.second);
    addObserver(std::move(observer));
    EventDispatcher::instance().notify(NewCellEvent(CellType::HEALTH_POINT, {BoardConfig::HealthPointPos.first, BoardConfig::HealthPointPos.second}));
}
