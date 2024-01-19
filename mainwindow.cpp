#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include "thirdwindow.h"
#include "fourthwindow.h"
#include "fifthwindow.h"
#include <QDate>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

QT_USE_NAMESPACE

            QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Пример инициализации указателей на окна

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_table1Button_clicked()
{
    openTableWindow(1);
}

void MainWindow::on_table2Button_clicked()
{
    openTableWindow(2);
}

void MainWindow::on_table3Button_clicked()
{
    openTableWindow(3);
}

void MainWindow::on_table4Button_clicked()
{
    openTableWindow(4);
}

void MainWindow::openTableWindow(int tableId)
{
    switch (tableId) {
    case 1: {
        SecondWindow *window = new SecondWindow(this);
        window->setModal(true);
        window->show();
        break;
    }
    case 2: {
        ThirdWindow *window = new ThirdWindow(this);
        window->setModal(true);
        window->show();
        break;
    }
    case 3: {
        FourthWindow *window = new FourthWindow(this);
        window->setModal(true);
        window->show();
        break;
    }
    case 4: {
        FifthWindow *window = new FifthWindow(this);
        window->setModal(true);
        window->show();
        break;
    }
    default:
        // Обработка других значений tableId (если необходимо)
        break;
    }
}


