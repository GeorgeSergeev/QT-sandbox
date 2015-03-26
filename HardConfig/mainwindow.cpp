#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "environmentinfo.h"
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addToTextEdit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToTextEdit()
{
    ui->textEdit->append(EnvironmentInfo::loadIPAsDigitSequence());
    ui->textEdit->append(EnvironmentInfo::loadOSInfo());
    ui->textEdit->append(EnvironmentInfo::loadProceessorInfo());
    ui->textEdit->append(EnvironmentInfo::loadMemoryInfo());
    ui->textEdit->append(EnvironmentInfo::loadIPAddress());
    ui->textEdit->append(EnvironmentInfo::loadSystemEnvironment());

//    ui->textEdit->append(EnvironmentInfo::pingHost("213.180.204.11"));
//    QStringList adrList;
//    adrList<<"213.180.204.11"<<"217.69.139.200"<<"localhost";
//    ui->textEdit->append(EnvironmentInfo::pingHostList(adrList));
//    ui->textEdit->append(EnvironmentInfo::processRequest("http://localhost:8080/MangoCRMServer/Health"));
}
