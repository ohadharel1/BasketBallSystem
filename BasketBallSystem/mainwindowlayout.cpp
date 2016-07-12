#include "mainwindowlayout.h"
#include "ui_mainwindowlayout.h"


MainWindowLayout::MainWindowLayout(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindowLayout),
    m_dbManager(NULL)
{
    m_ui->setupUi(this);
    init();
}

MainWindowLayout::~MainWindowLayout()
{
    delete m_ui;
}

void MainWindowLayout::init()
{
    initConnections();
}

void MainWindowLayout::initConnections()
{
    connect(this, SIGNAL(signalDisplayQuery(const QString)), DBManager::getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotHandleQuery(QSqlQueryModel*)));
    connect(this, SIGNAL(signalDisplayTable(const QString)), DBManager::getInstance(), SLOT(slotDisplayTable(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalTableResult(QSqlTableModel *)), this, SLOT(slotHandleTable(QSqlTableModel*)));
    connect(this, SIGNAL(signalSubmitReq()), DBManager::getInstance(), SLOT(slotHandleRequest()));
    connect(&m_fileExplorer, SIGNAL(signalPublishFilePath(QString)), this, SLOT(slotHandleFilePath(QString)));
    connect(this, SIGNAL(signalProccessCSV(QString)), DBManager::getInstance(), SLOT(slotHandleCSVProccessRequest(QString)));
}

void MainWindowLayout::slotHandleQuery(QSqlQueryModel *model)
{
    m_ui->comboBox->setModel(model);
    m_ui->tableView->setModel(model);
}

void MainWindowLayout::slotHandleTable(QSqlTableModel *model)
{
    model->insertRows(model->rowCount(), 1);
    m_ui->tableView->setModel(model);
}

void MainWindowLayout::slotHandleFilePath(const QString &path)
{
    m_ui->filePathTextBox->setText(path);
}


void MainWindowLayout::on_tableComboBox_currentIndexChanged(const QString &tableName)
{
    this->m_curTable = tableName;
    emit(signalDisplayTable(tableName));
}

void MainWindowLayout::on_queryComboBox_currentIndexChanged(const QString &queryName)
{
    this->m_curQuery = queryName;
    emit(signalDisplayQuery(queryName));
}

void MainWindowLayout::on_pushSubmit_released()
{
    emit(signalSubmitReq());
    emit(signalDisplayTable(m_curTable));
}

void MainWindowLayout::on_pushUndo_released()
{
    emit(signalDisplayTable(m_curTable));
}

void MainWindowLayout::on_pushDelete_released()
{
    QSqlTableModel *model = DBManager::getInstance()->getTableModel();
    model->removeRow(this->m_ui->tableView->currentIndex().row());
    model->submitAll();
    model->database().commit();
    model->select();
    model->insertRows(model->rowCount(), 1);
    m_ui->tableView->setModel(model);
}

void MainWindowLayout::on_browseFileBtn_released()
{
    m_fileExplorer.setModal(true);
    m_fileExplorer.exec();
}

void MainWindowLayout::on_uploadFileBtn_released()
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
