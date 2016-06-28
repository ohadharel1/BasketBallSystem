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

signals:
    void signalQueryTest1();

private slots:
    void on_pushButton_released();

private:
    Ui::MainWindow *ui;
    DBManager *dbManager;
};

#endif // MAINWINDOW_H
