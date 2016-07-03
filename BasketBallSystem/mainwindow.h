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
    void signalQueryTest1();
    void signalTableTest1();

private slots:
    void on_pushButton_released();

    void on_pushButton_2_released();

private:
    Ui::MainWindow *m_ui;
    DBManager *m_dbManager;
};

#endif // MAINWINDOW_H
