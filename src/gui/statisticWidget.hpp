#ifndef STATISTICWIDGET_H
#define STATISTICWIDGET_H

#include <QWidget>

#include "statistic.hpp"

namespace Ui {
class StatisticWidget;
}

class StatisticWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticWidget(std::shared_ptr<Statistic> stat, QWidget *parent = nullptr);
    ~StatisticWidget();

    subId getId();


public slots:

    void on_m_metricTypeBox_currentIndexChanged(const QString &arg1);

    void on_m_consumerCheckBox_stateChanged(int arg1);
    void on_m_producerCheckBox_stateChanged(int arg1);
    void on_m_provideCheckBox_stateChanged(int arg1);

private slots:
    void on_m_updateButton_clicked();
    void on_m_nodeIdBox_valueChanged(int arg1);

private:
    void subscriptionCallback(modelTime_t time, double value);
    void addAllData(const std::vector<double> &time, const std::vector<double> &value);
    void filterChanged();

private:
    Ui::StatisticWidget *ui;
    std::shared_ptr<Statistic> m_statistic;
    subId m_id;
};

#endif // STATISTICWIDGET_H
