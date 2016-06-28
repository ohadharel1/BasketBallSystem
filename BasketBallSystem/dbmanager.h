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

signals:
    void signalQueryResult(QSqlQueryModel*);

private:
    explicit DBManager(QObject *parent = 0);
    DBManager(const DBManager &other);
    DBManager& operator=(const DBManager &other);

    QSqlDatabase db;
    QSqlQuery query;
    QSqlQueryModel *queryModel;
};

#endif // DBMANAGER_H
