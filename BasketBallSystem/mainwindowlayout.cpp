#include "mainwindowlayout.h"
#include "ui_mainwindowlayout.h"


MainWindowLayout::MainWindowLayout(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowLayout),
    m_dbManager(NULL)
{
    ui->setupUi(this);
    init();
}

MainWindowLayout::~MainWindowLayout()
{
    delete ui;
}

void MainWindowLayout::slotPopulateComboBox(QSqlQueryModel *_model)
{
    ui->MainWindowLayoutTeamSelectionComboBox->setModel(_model);
}

void MainWindowLayout::init()
{
    m_dbManager->getInstance();
    initConnections();
    emit signalPoulateComboBox("teamNames");

}

void MainWindowLayout::initConnections()
{
    connect(this, SIGNAL(signalPoulateComboBox(const QString)), &m_dbManager->getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(&m_dbManager->getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotPopulateComboBox(QSqlQueryModel *)));
}
