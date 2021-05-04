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

    ui->m_graphTab->layout()->addWidget(new StatisticWidget(m_model->getStatisticModule()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_addGraphButton_clicked()
{
    auto sw = new StatisticWidget(m_model->getStatisticModule());
    ui->tabWidget->addTab(sw, "Graph " + QString::number(sw->getId()));
}

void MainWindow::on_m_removeGraphButton_clicked()
{    
    auto sw = ui->tabWidget->currentWidget();
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    sw->deleteLater();
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
