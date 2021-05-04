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


}

StatisticWidget::~StatisticWidget()
{
    m_statistic->unsubscribe(m_id);
    delete ui;
}

subId StatisticWidget::getId()
{
    return m_id;
}

void StatisticWidget::subscriptionCallback(modelTime_t time, double value)
{
    double plotTime = (double)time / g_oneMillisecond;
    ui->plot->graph(0)->addData(plotTime, value);
}

void StatisticWidget::addAllData(const std::vector<double>& time, const std::vector<double>& value)
{
    ui->plot->graph(0)->addData(QVector<double>(time.begin(), time.end()), QVector<double>(value.begin(), value.end()), true);
}

void StatisticWidget::filterChanged()
{
    Filter filter;

    if (ui->m_provideCheckBox->isChecked()) filter.roles.push_back(RoleType::PROVIDER);
    if (ui->m_producerCheckBox->isChecked()) filter.roles.push_back(RoleType::PRODUCER);
    if (ui->m_consumerCheckBox->isChecked()) filter.roles.push_back(RoleType::CONSUMER);

    filter.metric = static_cast<MetricType>(ui->m_metricTypeBox->currentData().toInt());

    filter.addr = ui->m_nodeIdBox->value();

    ui->plot->graph(0)->data()->clear();

    using namespace std::placeholders;
    m_statistic->subscribe(m_id, std::bind(&StatisticWidget::subscriptionCallback, this, _1, _2), std::bind(&StatisticWidget::addAllData, this, _1, _2), filter);
    ui->plot->yAxis->setLabel(ui->m_metricTypeBox->currentText());
    ui->plot->xAxis->setLabel("Time (ms)");
    ui->plot->rescaleAxes(true);
    ui->plot->replot();
}

void StatisticWidget::on_m_metricTypeBox_currentIndexChanged(const QString &arg1)
{
    filterChanged();
}

void StatisticWidget::on_m_consumerCheckBox_stateChanged(int arg1)
{
    filterChanged();
}

void StatisticWidget::on_m_producerCheckBox_stateChanged(int arg1)
{
    filterChanged();
}

void StatisticWidget::on_m_provideCheckBox_stateChanged(int arg1)
{
    filterChanged();
}

void StatisticWidget::on_m_updateButton_clicked()
{
    ui->plot->rescaleAxes(true);
    ui->plot->replot();
}

void StatisticWidget::on_m_nodeIdBox_valueChanged(int arg1)
{
    filterChanged();
}
