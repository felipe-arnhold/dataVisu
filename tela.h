#ifndef TELA_H
#define TELA_H

#include "websocketiodevice.h"

#include <QMainWindow>
#include <QMqttClient>
#include <QtWebSockets/QWebSocket>
#include <QtWidgets/QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class tela; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class tela : public QMainWindow
{
    Q_OBJECT

public:
    tela(QWidget *parent = nullptr);
    ~tela();

private slots:
    void on_actionSetup_triggered();
    void on_actionDataVisu_triggered();

    void onCBTypeSelChanged();
    void onMqttConnected();
    void updateLogStateChange();
    void brokerDisconnected();
    void onPBConnectClicked();
    void updateData(const QMqttMessage msg);
    void onPBSubscribeClicked();
    void onPBPublishClicked();

    void onPBTriggerClicked();

private:
    Ui::tela *ui;

    //MQTT Var
    QMqttClient *m_client;
    WebSocketIODevice m_device;
    QMqttSubscription *m_dataSub;

    QString trigTopic;
    int m_version;

    // Chart Var
    QChart *m_chart;
    QChartView *m_chartView;
};
#endif // TELA_H
