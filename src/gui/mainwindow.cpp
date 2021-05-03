#include "mainwindow.hpp"
#include "ui_mainwindow.h"


#include "modelCore.hpp"
#include "configurationWidget.hpp"
#include "statisticWidget.hpp"




MainWindow::MainWindow(std::shared_ptr<ModelCore> model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_model(model)
{
    ui->setupUi(this);

    m_staticWidgets.emplace_back(new StatisticWidget(m_model->getStatisticModule()));
    m_confWidget = new ConfigurationWidget();

    ui->m_graphTab->layout()->addWidget(m_staticWidgets.back());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_addGraphButton_clicked()
{
    m_staticWidgets.emplace_back(new StatisticWidget(m_model->getStatisticModule()));
    ui->tabWidget->addTab(m_staticWidgets.back(), "Graph " + QString::number(m_staticWidgets.back()->getId()));
}

void MainWindow::on_m_removeGraphButton_clicked()
{
    auto graphToRemove = ui->tabWidget->widget(ui->tabWidget->currentIndex());
    if (qobject_cast<StatisticWidget*>(graphToRemove) != nullptr)
    {
        m_staticWidgets.erase(std::find(m_staticWidgets.begin(), m_staticWidgets.end(), graphToRemove));
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    }

}

void MainWindow::on_m_startButton_clicked()
{
    m_model->start(g_oneSecond / 10);
}

void MainWindow::on_m_stopButton_clicked()
{
    m_model->stop();
}

void MainWindow::on_m_restartButton_clicked()
{

}
