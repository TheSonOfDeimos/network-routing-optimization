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

private slots:
    void on_m_updateButton_clicked();

private:
    void update();

private:
    Ui::StatisticWidget *ui;
    std::shared_ptr<Statistic> m_statistic;
    subId m_id;
};

#endif // STATISTICWIDGET_H
