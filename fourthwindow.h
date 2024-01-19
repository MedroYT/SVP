#ifndef FOURTHWINDOW_H
#define FOURTHWINDOW_H

#include <QSettings>
#include <QCloseEvent>
#include <QFile>
#include <QtCharts/QLineSeries>
#include <QDialog>
#include <QTableWidget>

namespace Ui {
class FourthWindow;
}

class FourthWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FourthWindow(QWidget *parent = nullptr);
    ~FourthWindow();


private slots:
    void on_saveButton_clicked();
    void saveSettings();
    void closeEvent(QCloseEvent *event);
    void loadSettings();
    void onDeleteButtonClicked();
    void addDataToSeries(QLineSeries *series, QTableWidget *tableWidget);
    void on_graphButton_clicked();
    void showChart();

private:
    Ui::FourthWindow *ui;
};

#endif // FOURTHWINDOW_H
