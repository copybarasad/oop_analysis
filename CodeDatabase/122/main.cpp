#include <QApplication>
#include <QIcon>

#include <iostream>
#include <memory>

#include "qtplatformmanager.h"

int main(int argc, char* argv[])
{
    std::shared_ptr<QApplication> m_app = std::make_shared<QApplication>(argc, argv);
    m_app->setWindowIcon(QIcon(":/resources/player.jpg"));
    std::shared_ptr<QTPlatformManager> m_manager = std::make_shared<QTPlatformManager>();
    return m_app->exec();
}
