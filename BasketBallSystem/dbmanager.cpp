#include "dbmanager.h"
#include <QFile>

DBManager *DBManager::m_instance = NULL;

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    this->m_db = QSqlDatabase::addDatabase("QODBC");
    this->m_queryModel = new QSqlQueryModel(this);
    this->m_tableModel = new QSqlTableModel(this, this->m_db);
    this->m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->init();
}

const DBManager* DBManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new DBManager();
    }
    return m_instance;
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

QSqlTableModel* DBManager::getTableModel() const
{
    return m_tableModel;
}

void DBManager::processLineFromCSV(QString line)
{
    QStringList strings = line.split(",");
    m_tableModel->setTable("TeamInLeague");
    QSqlRecord record = m_tableModel->record();
    for (int i = 0; i < strings.size(); ++i)
    {
        QVariant var = strings.at(i);
        record.setValue(i, var);
    }
    m_tableModel->insertRecord(-1, record);
    if(m_tableModel->submitAll())
    {
        m_tableModel->database().commit();

    }
    else
    {
        m_tableModel->database().rollback();
        qDebug() << "Database Write Error" <<
                         "The database reported an error: " <<
                           this->m_tableModel->lastError().text();
    }
}


DBManager::~DBManager()
{
    this->m_db.close();
}

void DBManager::slotDisplayQuery(const QString queryName)
{
    //this->m_query.setForwardOnly(true);
    this->m_query = this->m_db.exec("exec " + queryName);
    this->m_queryModel->setQuery(this->m_query);
    emit signalQueryResult(m_queryModel);
}

void DBManager::slotDisplayQueryWithArg(const QString proc, const QString arg)
{
    this->m_query.prepare("{CALL " + proc + " (:arg)}");
    this->m_query.bindValue(":arg", arg);
    this->m_query.exec();
    //this->m_query.lastError();
    emit signalParameterQueryResult(&m_query);
}

void DBManager::slotDisplayTable(QString tableName)
{
    this->m_tableModel->setTable(tableName);
    this->m_tableModel->select();
    //qDebug() << m_tableModel->lastError().text();
    emit signalTableResult(this->m_tableModel);
}

void DBManager::slotHandleRequest()
{
    this->m_tableModel->database().transaction();
    if(this->m_tableModel->submitAll())
    {
        this->m_tableModel->database().commit();
        this->m_tableModel->select();
    }
    else
    {
        this->m_tableModel->database().rollback();
        qDebug() << "Database Write Error" <<
                         "The database reported an error: " <<
                           this->m_tableModel->lastError().text();
    }
}

void DBManager::slotHandleCSVProccessRequest(const QString &path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        qDebug() << "file is open";
        m_tableModel->setTable("TeamInLeague");
        m_tableModel->database().transaction();
        m_tableModel->clear();
        m_tableModel->select();
        while(file.atEnd() == false)
        {
            QString line = file.readLine();
            line.remove(QRegExp("\r"));
            line.remove(QRegExp("\n"));
            processLineFromCSV(line);
        }
        file.close();
        m_tableModel->select();

    }
    else
    {
        qDebug() << "can't open file";
    }
}
