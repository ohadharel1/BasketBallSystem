#include "dbmanager.h"

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
    this->db = QSqlDatabase::addDatabase("QODBC");
    this->db.setConnectOptions();
    QString dsn = QString("DRIVER={SQL Server};Server=%1;Database=%2;Trusted_Conction=Yes;").arg(serverName).arg(dbName);
    this->db.setDatabaseName(dsn);

    if(this->db.open())
    {
        qDebug() << "DB open!!";
    }
    else
    {
        qDebug() << "Error = " << this->db.lastError().text();
    }
}

void DBManager::initConnections()
{

}

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    this->init();
}

DBManager::~DBManager()
{
    this->db.close();
}

void DBManager::slotQuery1()
{
    QStringList results;
    this->queryModel = new QSqlQueryModel();
    this->query = db.exec("test1");
    while(query.next())
    {
      QString result = query.record().value(1).toString();
      results.append(result);
    }
    qDebug() << results;
    this->queryModel->setQuery(this->query);
    emit signalQueryResult(this->queryModel);
}
