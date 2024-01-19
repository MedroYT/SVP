#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H
#include <QSettings>
#include <QCloseEvent>
#include <QFile>
#include <QtCharts/QLineSeries>
#include <QDialog>
#include <QTableWidget>
namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

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
    Ui::SecondWindow *ui;

};

#endif // SECONDWINDOW_H
