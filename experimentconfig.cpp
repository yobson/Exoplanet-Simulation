#include "experimentconfig.h"
#include "ui_experimentconfig.h"

experimentConfig::experimentConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::experimentConfig)
{
    ui->setupUi(this);
}

void experimentConfig::setSerialPort(QSerialPort *SP)
{
    serial = SP;
}

experimentConfig::~experimentConfig()
{
    delete ui;
}
