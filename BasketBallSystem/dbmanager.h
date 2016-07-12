#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>
#include <QTableView>

class DBManager : public QObject
{
    Q_OBJECT
public:
    const static DBManager* getInstance();
    ~DBManager();
    void init();
    void initDBLink();
    void initConnections();
    QSqlTableModel* getTableModel() const;
    void processLineFromCSV(QString line);

public slots:
    void slotDisplayQuery(const QString);
    void slotDisplayQueryWithArg(const QString proc, const QString arg);
    void slotDisplayTable(const QString);
    void slotHandleRequest();
    void slotHandleCSVProccessRequest(const QString&);

signals:
    void signalQueryResult(QSqlQueryModel*);
    void signalTableResult(QSqlTableModel*);
    void signalParameterQueryResult(QSqlQuery*);

private:
    explicit DBManager(QObject *parent = 0);
    DBManager(const DBManager &other);
    DBManager& operator=(const DBManager &other);

    static DBManager* m_instance;
    QSqlDatabase m_db;
    QSqlQuery m_query;
    QSqlQueryModel *m_queryModel;
    QSqlTableModel *m_tableModel;
};

#endif // DBMANAGER_H
