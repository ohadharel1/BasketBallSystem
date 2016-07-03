#include "dbmanager.h"

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    this->m_db = QSqlDatabase::addDatabase("QODBC");
    this->m_queryModel = new QSqlQueryModel(this);
    this->m_tableModel = new QSqlTableModel(this, this->m_db);
    this->init();
}

const DBManager& DBManager::getInstance()
{
    static DBManager instance;
    return instance;
}

void DBManager::init()
{
    this->initDBLink();
    this->initConnections();
}

void DBManager::initDBLink()
{
    QString serverName = "LOCALHOST\\SQLEXPRESS";
    QString dbName = "BasketBallSystemDB";

    this->m_db.setConnectOptions();
    QString dsn = QString("DRIVER={SQL Server};Server=%1;Database=%2;Trusted_Conction=Yes;").arg(serverName).arg(dbName);
    this->m_db.setDatabaseName(dsn);

    if(this->m_db.open())
    {
        qDebug() << "DB open!!";
    }
    else
    {
        qDebug() << "Error = " << this->m_db.lastError().text();
    }
}

void DBManager::initConnections()
{

}

DBManager::~DBManager()
{
    this->m_db.close();
}

void DBManager::slotQuery1()
{
    this->m_query.setForwardOnly(true);
    this->m_query = this->m_db.exec("exec test1");
    this->m_queryModel->setQuery(this->m_query);
    emit signalQueryResult(this->m_queryModel);
}

void DBManager::slotTable1()
{
    this->m_tableModel->setTable("Team");
    this->m_tableModel->select();
    qDebug() << m_tableModel->lastError().text();
    emit signalTableResult(this->m_tableModel);
}
