#include "statisticWidget.hpp"
#include "ui_statisticWidget.h"

#include <QVector>

#include "qcustomplot/qcustomplot.hpp"
#include "statistic.hpp"

StatisticWidget::StatisticWidget(std::shared_ptr<Statistic> stat, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticWidget),
    m_statistic(stat)
{
    static std::atomic<int> counter;
    m_id = counter++;

    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->graph(0)->setAdaptiveSampling(true);

    ui->m_metricTypeBox->addItem("Speed (Mbit/s)", QVariant::fromValue(static_cast<int>(MetricType::SPEED)));
    ui->m_metricTypeBox->addItem("Packetloss (%)", QVariant::fromValue(static_cast<int>(MetricType::PACKET_LOSS)));
    ui->m_metricTypeBox->addItem("Ping (ms)", QVariant::fromValue(static_cast<int>(MetricType::PING)));
    ui->m_metricTypeBox->addItem("Average package time in system (ms)", QVariant::fromValue(static_cast<int>(MetricType::AVG_PACKAGE_TIME_IN_SYSTEM)));
}

StatisticWidget::~StatisticWidget()
{
    delete ui;
}

subId StatisticWidget::getId()
{
    return m_id;
}

void StatisticWidget::update()
{
    // Update filter
    Filter filter;
    if (ui->m_provideCheckBox->isChecked()) filter.roles.push_back(RoleType::PROVIDER);
    if (ui->m_producerCheckBox->isChecked()) filter.roles.push_back(RoleType::PRODUCER);
    if (ui->m_consumerCheckBox->isChecked()) filter.roles.push_back(RoleType::CONSUMER);
    filter.metric = static_cast<MetricType>(ui->m_metricTypeBox->currentData().toInt());
    filter.addr = ui->m_nodeIdBox->value();

    // Update data
    ui->plot->graph(0)->data()->clear();
    auto list =  m_statistic->get(filter);
    ui->plot->graph(0)->addData(QVector<double>(list.first.begin(), list.first.end()), QVector<double>(list.second.begin(), list.second.end()), true);

    // Update plot view
    ui->plot->yAxis->setLabel(ui->m_metricTypeBox->currentText());
    ui->plot->xAxis->setLabel("Time (ms)");
    ui->plot->rescaleAxes(true);
    ui->plot->replot();
}

void StatisticWidget::on_m_updateButton_clicked()
{
    update();
}

