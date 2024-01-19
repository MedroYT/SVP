#ifndef FIFTHWINDOW_H
#define FIFTHWINDOW_H

#include <QSettings>
#include <QCloseEvent>
#include <QFile>
#include <QtCharts/QLineSeries>
#include <QDialog>
#include <QTableWidget>

namespace Ui {
class FifthWindow;
}

class FifthWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FifthWindow(QWidget *parent = nullptr);
    ~FifthWindow();
private slots:
    void on_saveButton_clicked();
    void closeEvent(QCloseEvent *event);
    void loadSettings();
    void saveSettings();
    void onDeleteButtonClicked();
    void on_graphButton_clicked();
    void showChart();
    void addDataToSeries(QLineSeries *series, QTableWidget *tableWidget);

private:
    Ui::FifthWindow *ui;
};

#endif // FIFTHWINDOW_H
