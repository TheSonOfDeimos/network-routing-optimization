#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ModelCore;
class ConfigurationWidget;
class StatisticWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::shared_ptr<ModelCore> model, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_m_addGraphButton_clicked();

    void on_m_removeGraphButton_clicked();

    void on_m_startButton_clicked();

    void on_m_stopButton_clicked();

    void on_m_restartButton_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ModelCore> m_model;

    ConfigurationWidget* m_confWidget;
    std::vector<StatisticWidget*> m_staticWidgets;

};

#endif // MAINWINDOW_H
