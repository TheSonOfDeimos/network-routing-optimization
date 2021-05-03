#ifndef CONFIGURATIONWIDGET_H
#define CONFIGURATIONWIDGET_H

#include <QWidget>

namespace Ui {
class ConfigurationWidget;
}

class ConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigurationWidget(QWidget *parent = nullptr);
    ~ConfigurationWidget();

private slots:
    void on_m_alorithm_currentIndexChanged(int index);

    void on_m_topology_currentIndexChanged(int index);

    void on_m_nodeCount_valueChanged(int arg1);

    void on_m_isAnomalyServer_stateChanged(int arg1);

    void on_m_isAnomalySwitch_stateChanged(int arg1);

    void on_m_isAnomalyBorder_stateChanged(int arg1);

private:
    Ui::ConfigurationWidget *ui;
};

#endif // CONFIGURATIONWIDGET_H
