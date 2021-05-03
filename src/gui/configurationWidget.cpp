#include "configurationWidget.hpp"
#include "ui_configurationWidget.h"

ConfigurationWidget::ConfigurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigurationWidget)
{
    ui->setupUi(this);
}

ConfigurationWidget::~ConfigurationWidget()
{
    delete ui;
}

void ConfigurationWidget::on_m_alorithm_currentIndexChanged(int index)
{

}

void ConfigurationWidget::on_m_topology_currentIndexChanged(int index)
{

}

void ConfigurationWidget::on_m_nodeCount_valueChanged(int arg1)
{

}

void ConfigurationWidget::on_m_isAnomalyServer_stateChanged(int arg1)
{

}

void ConfigurationWidget::on_m_isAnomalySwitch_stateChanged(int arg1)
{

}

void ConfigurationWidget::on_m_isAnomalyBorder_stateChanged(int arg1)
{

}
