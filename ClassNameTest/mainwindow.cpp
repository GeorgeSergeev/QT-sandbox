#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commands/command.h"
#include "commands/childcommand.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillListView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillListView()
{
    Command *cmd = new Command();
    Command *chCmd = new ChildCommand();
    QString cmdName=cmd->getClassName();
    QString chCmdName=chCmd->getClassName();
       ui->listWidget->addItem(new QListWidgetItem (cmdName));
       ui->listWidget->addItem(new QListWidgetItem (chCmdName));
}

void MainWindow::on_pushButton_clicked()
{



}
