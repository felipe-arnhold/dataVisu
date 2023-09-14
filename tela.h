#ifndef TELA_H
#define TELA_H

#include "websocketiodevice.h"

#include <QMainWindow>
#include <QMqttClient>
#include <QtWebSockets/QWebSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class tela; }
QT_END_NAMESPACE

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

private:
    Ui::tela *ui;

    //MQTT Var
    QMqttClient *m_client;
    WebSocketIODevice m_device;
    QMqttSubscription *m_dataSub;

    QString trigTopic;
    int m_version;
};
#endif // TELA_H
