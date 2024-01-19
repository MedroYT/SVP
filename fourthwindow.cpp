#include "fourthwindow.h"
#include "ui_fourthwindow.h"
#include <QSettings>
#include <QCloseEvent>
#include <QFile>
#include <QDate>
#include <QMessageBox>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QTableWidget>

FourthWindow::FourthWindow(QWidget *parent) : QDialog(parent), ui(new Ui::FourthWindow)
{
    ui->setupUi(this);
    loadSettings();
    connect(ui->deleteButton, &QPushButton::clicked, this, &FourthWindow::onDeleteButtonClicked);
}

FourthWindow::~FourthWindow()
{
    delete ui;
}

void FourthWindow::on_saveButton_clicked()
{
    qDebug() << "Функция onSaveButtonClicked() вызвана";

    // Открываем файл для записи
    QFile file("E:\\table3_data.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Файл успешно открыт для записи";

        QTextStream out(&file);

        // Получаем количество строк и столбцов в таблице
        int rowCount = ui->tableWidget3->rowCount();
        int colCount = ui->tableWidget3->columnCount();

        // Проходим по каждой ячейке таблицы и записываем значения в файл
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                QTableWidgetItem *item = ui->tableWidget3->item(row, col);

                // Если элемент существует, записываем его значение в файл
                if (item) {
                    if (col == 0 && !item->text().isEmpty()) { // Проверяем первую колонку (дата)
                        QDate date = QDate::fromString(item->text(), "dd.MM.yyyy");
                        if (!date.isValid()) {
                            QMessageBox::critical(this, "Ошибка", "Некорректная дата в строке " + QString::number(row + 1));
                            return;
                        }
                    } else if (col == 1 && !item->text().isEmpty()) { // Проверяем вторую колонку (рост)
                        bool ok;
                        int height = item->text().toInt(&ok);
                        if (!ok || height < 0) {
                            QMessageBox::critical(this, "Ошибка", "Некорректный (отрицательный) рост в строке " + QString::number(row + 1));
                            return;
                        }
                    }

                    out << item->text() << "\t";
                }
            }
            out << "\n";
        }

        // Закрываем файл после записи
        file.close();
        qDebug() << "Файл успешно закрыт";

        // Выводим сообщение об успешном сохранении данных
        QMessageBox::information(this, "Успех", "Данные успешно сохранены!");
    } else {
        qDebug() << "Не удалось открыть файл для записи";
        // Выводим сообщение об ошибке открытия файла
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи");
    }
}

void FourthWindow::closeEvent(QCloseEvent *event)
{
    // Сохраняем данные при закрытии окна
    saveSettings();

    QCoreApplication::processEvents();

    // Игнорируем событие закрытия окна
    event->ignore();
    QDialog::closeEvent(event);
}

void FourthWindow::loadSettings()
{
    // Открываем файл для чтения
    QFile file("E:\\table3_data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Считываем значения из файла и устанавливаем их в ячейки таблицы
        int row = 0;
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList values = line.split("\t", Qt::SkipEmptyParts);

            for (int col = 0; col < values.size(); ++col) {
                // Проверяем, что индексы не выходят за границы таблицы
                if (row < ui->tableWidget3->rowCount() && col < ui->tableWidget3->columnCount()) {
                    // Создаем ячейку, если она не существует
                    if (!ui->tableWidget3->item(row, col)) {
                        ui->tableWidget3->setItem(row, col, new QTableWidgetItem);
                    }

                    // Устанавливаем значение в ячейку
                    ui->tableWidget3->item(row, col)->setText(values[col]);
                }
            }

            ++row;
        }

        // Закрываем файл после чтения
        file.close();
    }
}

void FourthWindow::saveSettings()
{
    QSettings settings("MyApp", "MyApp");
    settings.beginReadArray("table3_data");

    // Получаем количество строк и столбцов в таблице
    int rowCount = ui->tableWidget3->rowCount();
    int colCount = ui->tableWidget3->columnCount();

    // Проходим по каждой ячейке таблицы и записываем значения в настройки
    for (int row = 0; row < rowCount; ++row) {
        settings.setArrayIndex(row);

        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem *item = ui->tableWidget3->item(row, col);

            // Если элемент существует, записываем его значение в настройки
            if (item) {
                settings.setValue(QString("Column_%1").arg(col), item->text());
            }
        }
    }

    settings.endArray();
}

void FourthWindow::onDeleteButtonClicked()
{
    // Получаем список выбранных ячеек
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget3->selectedItems();

    // Удаляем значения в выбранных ячейках
    foreach (QTableWidgetItem *item, selectedItems) {
        item->setText("");  // Очищаем значение ячейки
    }

    // Выводим сообщение об успешном удалении данных
    QMessageBox::information(this, "Успех", "Данные успешно удалены!");
}

void FourthWindow::showChart()
{
    // Создаем график
    QChart *chart = new QChart;
    QLineSeries *series = new QLineSeries;

    // Добавляем данные из таблицы
    addDataToSeries(series, ui->tableWidget3);

    // Добавляем оси координат
    QDateTimeAxis *axisX = new QDateTimeAxis;
    QValueAxis *axisY = new QValueAxis;

    chart->addSeries(series);
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    // Настраиваем ось X
    axisX->setFormat("dd.MM.yyyy");
    axisX->setTitleText("Дата");
    axisX->setTickCount(2); // Устанавливаем количество делений

    // Настраиваем ось Y
    axisY->setTitleText("Рост");

    // Настраиваем внешний вид графика
    chart->setTitle("График роста по датам");

    // Создаем окно для отображения графика
    QDialog *chartDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout;
    QChartView *chartView = new QChartView(chart);

    layout->addWidget(chartView);
    chartDialog->setLayout(layout);
    chartDialog->resize(900, 600);

    // Отображаем окно с графиком
    chartDialog->exec();
}


void FourthWindow::addDataToSeries(QLineSeries *series, QTableWidget *tableWidget)
{
    // Получаем количество строк и столбцов в таблице
    int rowCount = tableWidget->rowCount();
    int colDate = 0;
    int colHeight = 1;

    // Проходим по каждой строке таблицы и добавляем значения в график
    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem *itemDate = tableWidget->item(row, colDate);
        QTableWidgetItem *itemHeight = tableWidget->item(row, colHeight);

        // Если элементы существуют, добавляем их в график
        if (itemDate && itemHeight && !itemDate->text().isEmpty() && !itemHeight->text().isEmpty()) {
            QDate date = QDate::fromString(itemDate->text(), "dd.MM.yyyy");
            bool ok;
            int height = itemHeight->text().toInt(&ok);

            if (date.isValid() && ok) {
                QDateTime dateTime(date, QTime(0, 0, 0)); // Указываем начальное время



                // Используем toMSecsSinceEpoch() для корректного представления даты и времени
                series->append(dateTime.toMSecsSinceEpoch(), height);
            }
        }
    }
}

void FourthWindow::on_graphButton_clicked()
{
    showChart();
}
