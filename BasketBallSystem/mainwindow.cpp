#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_dbManager(NULL)
{
    m_ui->setupUi(this);
    m_dbManager->getInstance();
    init();
}

void MainWindow::init()
{
    initConnections();
}

void MainWindow::initConnections()
{
    connect(this, SIGNAL(signalQueryTest1()), &m_dbManager->getInstance(), SLOT(slotQuery1()));
    connect(&m_dbManager->getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotHandleQuery(QSqlQueryModel*)));
    connect(this, SIGNAL(signalTableTest1()), &m_dbManager->getInstance(), SLOT(slotTable1()));
    connect(&m_dbManager->getInstance(), SIGNAL(signalTableResult(QSqlTableModel *)), this, SLOT(slotHandleTable(QSqlTableModel*)));
}

void MainWindow::slotHandleQuery(QSqlQueryModel *model)
{
    m_ui->tableView->setModel(model);
}

void MainWindow::slotHandleTable(QSqlTableModel *model)
{
    m_ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::on_pushButton_released()
{
    emit(signalQueryTest1());
}

void MainWindow::on_pushButton_2_released()
{
    emit(signalTableTest1());
}
