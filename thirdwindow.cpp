#include "thirdwindow.h"
#include "ui_thirdwindow.h"
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

ThirdWindow::ThirdWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ThirdWindow)
{
    ui->setupUi(this);
    loadSettings();
    connect(ui->deleteButton, &QPushButton::clicked, this, &ThirdWindow::onDeleteButtonClicked);
}

ThirdWindow::~ThirdWindow()
{
    delete ui;
}

void ThirdWindow::on_saveButton_clicked()
{
    qDebug() << "Функция onSaveButtonClicked() вызвана";

    // Открываем файл для записи
    QFile file("E:\\table2_data.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Файл успешно открыт для записи";

        QTextStream out(&file);

        // Получаем количество строк и столбцов в таблице
        int rowCount = ui->tableWidget2->rowCount();
        int colCount = ui->tableWidget2->columnCount();

        // Проходим по каждой ячейке таблицы и записываем значения в файл
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                QTableWidgetItem *item = ui->tableWidget2->item(row, col);

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

void ThirdWindow::closeEvent(QCloseEvent *event)
{
    // Сохраняем данные при закрытии окна
    saveSettings();

    QCoreApplication::processEvents();

    // Игнорируем событие закрытия окна
    event->ignore();
    QDialog::closeEvent(event);
}

void ThirdWindow::loadSettings()
{
    // Открываем файл для чтения
    QFile file("E:\\table2_data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Считываем значения из файла и устанавливаем их в ячейки таблицы
        int row = 0;
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList values = line.split("\t", Qt::SkipEmptyParts);

            for (int col = 0; col < values.size(); ++col) {
                // Проверяем, что индексы не выходят за границы таблицы
                if (row < ui->tableWidget2->rowCount() && col < ui->tableWidget2->columnCount()) {
                    // Создаем ячейку, если она не существует
                    if (!ui->tableWidget2->item(row, col)) {
                        ui->tableWidget2->setItem(row, col, new QTableWidgetItem);
                    }

                    // Устанавливаем значение в ячейку
                    ui->tableWidget2->item(row, col)->setText(values[col]);
                }
            }

            ++row;
        }

        // Закрываем файл после чтения
        file.close();
    }
}

void ThirdWindow::saveSettings()
{
    QSettings settings("MyApp", "MyApp");
    settings.beginReadArray("table2_data");

    // Получаем количество строк и столбцов в таблице
    int rowCount = ui->tableWidget2->rowCount();
    int colCount = ui->tableWidget2->columnCount();

    // Проходим по каждой ячейке таблицы и записываем значения в настройки
    for (int row = 0; row < rowCount; ++row) {
        settings.setArrayIndex(row);

        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem *item = ui->tableWidget2->item(row, col);

            // Если элемент существует, записываем его значение в настройки
            if (item) {
                settings.setValue(QString("Column_%1").arg(col), item->text());
            }
        }
    }

    settings.endArray();
}

void ThirdWindow::onDeleteButtonClicked()
{
    // Получаем список выбранных ячеек
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget2->selectedItems();

    // Удаляем значения в выбранных ячейках
    foreach (QTableWidgetItem *item, selectedItems) {
        item->setText("");  // Очищаем значение ячейки
    }

    // Выводим сообщение об успешном удалении данных
    QMessageBox::information(this, "Успех", "Данные успешно удалены!");
}

void ThirdWindow::showChart()
{
    // Создаем график
    QChart *chart = new QChart;
    QLineSeries *series = new QLineSeries;

    // Добавляем данные из таблицы
    addDataToSeries(series, ui->tableWidget2);

    // Добавляем оси координат
    QDateTimeAxis *axisX = new QDateTimeAxis;
    QValueAxis *axisY = new QValueAxis;

    chart->addSeries(series);
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    // Настраиваем ось X
    axisX->setFormat("dd.MM.yyyy");
    axisX->setTitleText("Дата");
    axisX->setTickCount(4); // Устанавливаем количество делений

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


void ThirdWindow::addDataToSeries(QLineSeries *series, QTableWidget *tableWidget)
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

void ThirdWindow::on_graphButton_clicked()
{
    showChart();
}
