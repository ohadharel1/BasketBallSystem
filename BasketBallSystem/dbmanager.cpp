#include "dbmanager.h"
#include <QFile>
#include "popupmessagedialog.h"

DBManager *DBManager::m_instance = NULL;

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    this->m_db = QSqlDatabase::addDatabase("QODBC");
    this->m_queryModel = new QSqlQueryModel(this);
    this->m_tableModel = new QSqlTableModel(this, this->m_db);
    this->m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->init();
}

DBManager* DBManager::getInstance()
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

bool DBManager::processLineFromCSV(QString line)
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
        return true;

    }
    else
    {
        m_tableModel->database().rollback();
        QString error = "Database Write Error. The database reported an error: " + this->m_tableModel->lastError().text();
        popupMessageDialog::getInstance()->addText(error);
        qDebug() << "Database Write Error" <<
                         "The database reported an error: " <<
                           this->m_tableModel->lastError().text();
    }
    return false;
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

void DBManager::slotDisplayQueryWithArgs(const QString proc, const QStringList args)
{
    QString argsForPrepare;
    int i = 0;
    argsForPrepare = " (";
    for(i = 0; i < args.size() - 1; ++i)
    {
        argsForPrepare = argsForPrepare + ":arg" + QString::number(i) + ",";
    }
    argsForPrepare = argsForPrepare + ":arg" + QString::number(i) + ")}";
    this->m_query.prepare("{CALL " + proc + argsForPrepare);
    for(i = 0; i < args.size(); ++i)
    {
        if(args[i] == "")
        {
            continue;
        }
        else
        {
            this->m_query.bindValue(":arg" + QString::number(i), args[i]);
        }
    }
    if(this->m_query.exec() == false)
    {
        popupMessageDialog::getInstance()->addText(m_query.lastError().text());
        popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_ERROR);
    }
    else
    {
        m_db.commit();
    }
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
        popupMessageDialog::getInstance()->addText(m_tableModel->lastError().text());
        popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_ERROR);
    }
}

void DBManager::slotHandleCSVProccessRequest(const QString &path)
{
    int numOfLines = 0;
    int numOfErrors = 0;
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
            ++numOfLines;
            QString line = file.readLine();
            line.remove(QRegExp("\r"));
            line.remove(QRegExp("\n"));
            if(processLineFromCSV(line) == false)
            {
                ++numOfErrors;
            }
        }
        file.close();
        m_tableModel->select();
        popupMessageDialog::getInstance()->setCSVResult(numOfLines, numOfErrors);
        popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_CSV_SUM);
    }
    else
    {
        qDebug() << "can't open file";
    }
}
