#include "mainwindow.h"
#include <QPalette>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    MainWindow w;
    QPalette m_pal;

    m_pal.setColor(QPalette::Window, QColor(255, 255, 255)); // Белый цвет для фона окна
    m_pal.setColor(QPalette::WindowText, QColor(10, 10, 10)); // Черный цвет для текста
    m_pal.setColor(QPalette::Button, QColor(210, 225, 2)); // Белый цвет для фона кнопок
    m_pal.setColor(QPalette::ButtonText, QColor(20, 20, 20)); // Черный цвет для текста на кнопках
    m_pal.setColor(QPalette::Highlight, QColor(255, 255, 255)); // Белый цвет для выделения
    m_pal.setColor(QPalette::HighlightedText, QColor(0, 0, 0)); // Черный цвет для текста на выделенном фоне


    w.setPalette(m_pal);
    w.show();
    return a.exec();
}
