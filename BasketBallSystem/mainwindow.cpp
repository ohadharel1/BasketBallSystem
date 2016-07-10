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

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::init()
{
    initConnections();
}

void MainWindow::initConnections()
{
    connect(this, SIGNAL(signalDisplayQuery(const QString)), &m_dbManager->getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(&m_dbManager->getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotHandleQuery(QSqlQueryModel*)));
    connect(this, SIGNAL(signalDisplayTable(const QString)), &m_dbManager->getInstance(), SLOT(slotDisplayTable(const QString)));
    connect(&m_dbManager->getInstance(), SIGNAL(signalTableResult(QSqlTableModel *)), this, SLOT(slotHandleTable(QSqlTableModel*)));
    connect(this, SIGNAL(signalSubmitReq()), &m_dbManager->getInstance(), SLOT(slotHandleRequest()));
    connect(&m_fileExplorer, SIGNAL(signalPublishFilePath(QString)), this, SLOT(slotHandleFilePath(QString)));
    connect(this, SIGNAL(signalProccessCSV(QString)), &m_dbManager->getInstance(), SLOT(slotHandleCSVProccessRequest(QString)));
}

void MainWindow::slotHandleQuery(QSqlQueryModel *model)
{
    m_ui->tableView->setModel(model);
}

void MainWindow::slotHandleTable(QSqlTableModel *model)
{
    model->insertRows(model->rowCount(), 1);
    m_ui->tableView->setModel(model);
}

void MainWindow::slotHandleFilePath(const QString &path)
{
    m_ui->filePathTextBox->setText(path);
}


void MainWindow::on_tableComboBox_currentIndexChanged(const QString &tableName)
{
    this->m_curTable = tableName;
    emit(signalDisplayTable(tableName));
}

void MainWindow::on_queryComboBox_currentIndexChanged(const QString &queryName)
{
    this->m_curQuery = queryName;
    emit(signalDisplayQuery(queryName));
}

void MainWindow::on_pushSubmit_released()
{
    emit(signalSubmitReq());
    emit(signalDisplayTable(m_curTable));
}

void MainWindow::on_pushUndo_released()
{
    emit(signalDisplayTable(m_curTable));
}

void MainWindow::on_pushDelete_released()
{
    QSqlTableModel *model = m_dbManager->getInstance().getTableModel();
    model->removeRow(this->m_ui->tableView->currentIndex().row());
    model->submitAll();
    model->database().commit();
    model->select();
    model->insertRows(model->rowCount(), 1);
    m_ui->tableView->setModel(model);
}

void MainWindow::on_browseFileBtn_released()
{
    m_fileExplorer.setModal(true);
    m_fileExplorer.exec();
}

void MainWindow::on_uploadFileBtn_released()
{
//    if(m_ui->filePathTextBox->hasSelectedText() == false)
//    {
//        //do popup error message
//        //qDebug() << "You must fill in file path";
//    }
//    else
//    {
        emit signalProccessCSV(m_ui->filePathTextBox->text());
//    }
}
