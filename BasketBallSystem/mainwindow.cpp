#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dbManager(NULL)
{
    ui->setupUi(this);
    dbManager->getInstance();
    init();
}

void MainWindow::init()
{
    initConnections();
}

void MainWindow::initConnections()
{
    connect(this, SIGNAL(signalQueryTest1()), &dbManager->getInstance(), SLOT(slotQuery1()));
    connect(&dbManager->getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotHandleQuery(QSqlQueryModel*)));
}

void MainWindow::slotHandleQuery(QSqlQueryModel *model)
{
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_released()
{
    emit(signalQueryTest1());
}
