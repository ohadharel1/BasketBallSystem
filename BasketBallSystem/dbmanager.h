#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>
#include <QTableView>

class DBManager : public QObject
{
    Q_OBJECT
public:
    const DBManager& getInstance() const;
    void init();
    void initDBLink();
    void initConnections();
    QSqlTableModel* getTableModel() const;
    ~DBManager();

public slots:
    void slotDisplayQuery(const QString);
    void slotDisplayTable(const QString);
    void slotHandleRequest();

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
