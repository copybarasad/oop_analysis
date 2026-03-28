#ifndef VIEW_H
#define VIEW_H

#include <memory>
#include "constants.h"

template<typename TRenderer>
class View
{
public:
    View() : m_renderer(std::make_shared<TRenderer>()) {}

    void renderInitState()
    {
        m_renderer->renderInitState();
    }
    void renderLevelComplete()
    {
        m_renderer->renderLevelComplete();
    }
    void renderLevelFailed()
    {
        m_renderer->renderLevelFailed();
    }

private:
    std::shared_ptr<TRenderer> m_renderer;
};

#endif // VIEW_H
