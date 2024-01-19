#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

QT_USE_NAMESPACE

    namespace QtCharts { // Добавьте эту строку
    class QChart; // Добавьте эту строку
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_table1Button_clicked();
    void on_table2Button_clicked();
    void on_table3Button_clicked();
    void on_table4Button_clicked();
    void openTableWindow(int tableId);

private:
    Ui::MainWindow *ui;
    QtCharts::QChart *chrt;

};
#endif // MAINWINDOW_H
