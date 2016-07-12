#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QStackedWidget>
#include "dbmanager.h"
#include "guiformmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotPopulateComboBox(QSqlQueryModel*);
    void slotChangeForm(GuiForms);
    void updateCaption();

    void on_MainWindowTeamSelectionSelectBtn_released();

    void on_MainWindowTeamSelectionEditDataBtn_released();

    void on_MainWindowTeamSelectionGameManagmentBtn_released();

    void on_MainWindowTeamSelectionComboBox_currentIndexChanged(const QString &arg1);

signals:
    void signalPoulateComboBox(const QString&);

private:
    Ui::MainWindow *ui;
    void init();
    void initLayOut();
    void initConnections();

};

#endif // MAINWINDOW_H
