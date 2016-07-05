#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();
    void initConnections();

public slots:
    void slotHandleQuery(QSqlQueryModel*);
    void slotHandleTable(QSqlTableModel*);

signals:
    void signalDisplayQuery(const QString &queryName);
    void signalDisplayTable(const QString &tableName);
    void signalSubmitReq();

private slots:

    void on_tableComboBox_currentIndexChanged(const QString &arg1);

    void on_queryComboBox_currentIndexChanged(const QString &arg1);

    void on_pushSubmit_released();

    void on_pushUndo_released();

    void on_pushDelete_released();

private:
    Ui::MainWindow *m_ui;
    DBManager *m_dbManager;
    QString m_curTable;
    QString m_curQuery;
};

#endif // MAINWINDOW_H
