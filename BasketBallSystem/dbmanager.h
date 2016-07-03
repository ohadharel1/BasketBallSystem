#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>

class DBManager : public QObject
{
    Q_OBJECT
public:
    const DBManager& getInstance();
    void init();
    void initDBLink();
    void initConnections();
    ~DBManager();

public slots:
    void slotQuery1();
    void slotTable1();

signals:
    void signalQueryResult(QSqlQueryModel*);
    void signalTableResult(QSqlTableModel*);

private:
    explicit DBManager(QObject *parent = 0);
    DBManager(const DBManager &other);
    DBManager& operator=(const DBManager &other);

    QSqlDatabase m_db;
    QSqlQuery m_query;
    QSqlQueryModel *m_queryModel;
    QSqlTableModel *m_tableModel;
};

#endif // DBMANAGER_H
