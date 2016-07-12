#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QStackedWidget>
#include <QVector>
#include "player.h"
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
    void slotTime();
    void slotReceivePlayearsInTeam(QSqlQuery*);
    void slotSortPlayers();

    void on_MainWindowTeamSelectionSelectBtn_released();

    void on_MainWindowTeamSelectionEditDataBtn_released();

    void on_MainWindowTeamSelectionGameManagmentBtn_released();

    void on_MainWindowTeamSelectionComboBox_currentIndexChanged(const QString &arg1);

    void on_MainWindowPlayerSelectionAllBtn_released();

    void on_MainWindowPlayerSelectionPoinGaurdBtn_released();

    void on_MainWindowPlayerSelectionShootingGaurdBtn_released();

    void on_MainWindowPlayerSelectionSmallForwardBtn_released();

    void on_MainWindowPlayerSelectionPowerForwardBtn_released();

    void on_MainWindowPlayerSelectionCenterBtn_released();

signals:
    void signalPoulateComboBox(const QString&);
    void signalGetPlayersInTeam(const QString&, const QString&);
    void signalSortPlayers();

private:
    void init();
    void initLayOut();
    void initConnections();
    void deletePlayers();
    bool containID(int id);
    player* getPlayer(int id);
    void insertPlayersToSelection(QVector<player*> vector);

    Ui::MainWindow *ui;
    QVector<player*> m_players;
    QVector<player*> m_pointGaurds;
    QVector<player*> m_shootingGaurd;
    QVector<player*> m_smallForward;
    QVector<player*> m_powerForward;
    QVector<player*> m_centers;

};

#endif // MAINWINDOW_H
