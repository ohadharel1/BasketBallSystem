#ifndef MAINWINDOWLAYOUT_H
#define MAINWINDOWLAYOUT_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "dbmanager.h"

namespace Ui {
class MainWindowLayout;
}

class MainWindowLayout : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowLayout(QWidget *parent = 0);
    ~MainWindowLayout();

private slots:
    void slotPopulateComboBox(QSqlQueryModel*);

signals:
    void signalPoulateComboBox(const QString&);

private:
    Ui::MainWindowLayout *ui;
    void init();
    void initConnections();

    DBManager* m_dbManager;

};

#endif // MAINWINDOWLAYOUT_H
