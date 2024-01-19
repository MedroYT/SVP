#ifndef THIRDWINDOW_H
#define THIRDWINDOW_H
#include <QSettings>
#include <QCloseEvent>
#include <QFile>
#include <QtCharts/QLineSeries>
#include <QDialog>
#include <QTableWidget>


namespace Ui {
class ThirdWindow;
}

class ThirdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ThirdWindow(QWidget *parent = nullptr);
    ~ThirdWindow();

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
    Ui::ThirdWindow *ui;
};

#endif // THIRDWINDOW_H
