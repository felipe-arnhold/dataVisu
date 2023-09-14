#include "tela.h"
#include "ui_tela.h"

tela::tela(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::tela)
{
    ui->setupUi(this);


    ui->cbTypeSel->setCurrentIndex(0);
    ui->qw_ampFields->hide();
    ui->qw_voltFields->show();

    brokerDisconnected();

    connect(ui->pb_mqttConnect, &QPushButton::clicked, this, &tela::onPBConnectClicked);
    connect(ui->cbTypeSel, &QComboBox::currentTextChanged, this, &tela::onCBTypeSelChanged);

    // Configure MQTT
    m_client = new QMqttClient(this);
    m_version = 3;
    m_device.setProtocol(m_version == 3 ? "mqttv3.1" : "mqtt");
    connect(&m_device, &WebSocketIODevice::socketConnected, this, [this]() {
        m_client->setProtocolVersion(m_version == 3 ? QMqttClient::MQTT_3_1 : QMqttClient::MQTT_3_1_1);
        m_client->setTransport(&m_device, QMqttClient::IODevice);
        connect(m_client, &QMqttClient::connected, this, &tela::onMqttConnected);
        m_client->connectToHost();
    });

    connect(m_client, &QMqttClient::stateChanged, this, &tela::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &tela::brokerDisconnected);

}

tela::~tela()
{
    delete ui;
}


void tela::on_actionSetup_triggered()
{
    ui->subPages->setCurrentIndex(1);
}


void tela::on_actionDataVisu_triggered()
{
    ui->subPages->setCurrentIndex(0);
}

void tela::onCBTypeSelChanged()
{
    int sel = ui->cbTypeSel->currentIndex();
    if(sel == 0){
        ui->qw_ampFields->hide();
        ui->qw_voltFields->show();
    }else{
        ui->qw_ampFields->show();
        ui->qw_voltFields->hide();
    }
}

void tela::onMqttConnected()
{
    ui->pb_mqttConnect->setDisabled(false);
    ui->pb_mqttConnect->setText(tr("Disconnect"));
    ui->le_mqttBrokerUrl->setDisabled(true);
    ui->le_mqttDataTopic->setDisabled(true);
    ui->le_mqttTriggerTopic->setDisabled(true);
    ui->le_mqttDbgTopic->setDisabled(false);
    ui->le_mqttDbgMessage->setDisabled(false);
    ui->pb_mqttPublish->setDisabled(false);
    ui->pb_mqttSubscribe->setDisabled(false);

    ui->cbTypeSel->setDisabled(false);
    ui->sbMaxPot->setDisabled(false);
    ui->sbMinPot->setDisabled(false);
    ui->sbScanRate->setDisabled(false);
    ui->sbCycles->setDisabled(false);
    ui->sbPot->setDisabled(false);
    ui->sbTime->setDisabled(false);

    // LOG and DEBUG
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
                                + QLatin1String(" Received Topic: ")
                                + topic.name()
                                + QLatin1String(" Message: ")
                                + message
                                + QLatin1Char('\n');
        ui->pt_mqttLOG->insertPlainText(content);
    });
}

void tela::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                            + QLatin1String(": State Change")
                            + QString::number(m_client->state())
                            + QLatin1Char('\n');
    ui->pt_mqttLOG->insertPlainText(content);
}

void tela::brokerDisconnected()
{
    ui->pb_mqttConnect->setDisabled(false);
    ui->pb_mqttConnect->setText(tr("Connect"));
    ui->le_mqttBrokerUrl->setDisabled(false);
    ui->le_mqttDataTopic->setDisabled(false);
    ui->le_mqttTriggerTopic->setDisabled(false);
    ui->le_mqttDbgTopic->setDisabled(true);
    ui->le_mqttDbgMessage->setDisabled(true);
    ui->pb_mqttPublish->setDisabled(true);
    ui->pb_mqttSubscribe->setDisabled(true);

    ui->cbTypeSel->setDisabled(true);
    ui->sbMaxPot->setDisabled(true);
    ui->sbMinPot->setDisabled(true);
    ui->sbScanRate->setDisabled(true);
    ui->sbCycles->setDisabled(true);
    ui->sbPot->setDisabled(true);
    ui->sbTime->setDisabled(true);
    ui->pbTrigger->setDisabled(true);
}

void tela::onPBConnectClicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        ui->pb_mqttConnect->setDisabled(true);
        m_device.setUrl(QUrl(ui->le_mqttBrokerUrl->text()));
        if (!m_device.open(QIODevice::ReadWrite))
            qDebug() << "Could not open socket device";

    } else {
        ui->pb_mqttConnect->setDisabled(false);
        m_device.close();
    }
}

