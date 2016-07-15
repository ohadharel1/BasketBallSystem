#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QStackedWidget>
#include <QGridLayout>
#include <QVector>
#include <QTableWidget>
#include "player.h"
#include "dbmanager.h"
#include "guiformmanager.h"

enum positionLayout
{
    POSITION_LAYOUT_ALL_PLAYERS,
    POSITION_LAYOUT_POINT_GAURDS,
    POSITION_LAYOUT_SHOOTING_GAURD,
    POSITION_LAYOUT_SMALL_FORWARD,
    POSITION_LAYOUT_POWER_FORWARD,
    POSITION_LAYOUT_CENTERS,

    POSITION_LAYOUT_SIZE
};

enum Querys
{
    Query_1,
    Query_2,
    Query_3,
    Query_4,
    Query_5,
    Query_6,
    Query_7,
    Query_size
};

enum Tables
{
    TABLE_AssistanceTrainer,
    TABLE_Cheerleaders,
    TABLE_Game,
    TABLE_League,
    TABLE_MainTrainer,
    TABLE_Player,
    TABLE_PlayesIn,
    TABLE_Position,
    TABLE_Season,
    TABLE_SeasonCycle,
    TABLE_SecondaryPosition,
    TABLE_Statistic,
    TABLE_Team,
    TABLE_TeamInLeague,
    TABLE_Trainer,

    TABLE_SIZE

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* getInstance();

private slots:
    void slotPopulateComboBox(QSqlQueryModel*);
    void slotChangeForm(GuiForms);
    void slotTime();
    void slotReceivePlayearsInTeam(QSqlQuery*);
    void slotSortPlayers();
    void slotHandelePlayerPress(int id);
    void slotEntranceForm(bool);
    void slotHandleQuery(QSqlQueryModel *model);
    void slotHandleTable(QSqlTableModel*);

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




    void on_MainWindowGameManagmentCortImageSFB_released();

    void on_MainWindowGameManagmentCortImagePGB_released();

    void on_MainWindowGameManagmentCortImageSGB_released();

    void on_MainWindowGameManagmentCortImagePFB_released();

    void on_MainWindowGameManagmentCortImageCB_released();

    void on_MainWindowBottomToolbarReturnBtn_released();

    void on_MainWindowBottomToolbarHomeBtn_released();

    void on_MainWindowEditPlayersQCB_currentIndexChanged(int index);



    void on_MainWindowEditPlayersTCB_currentIndexChanged(int index);

    void on_pushButton_released();

signals:
    void signalPoulateComboBox(const QString&);
    void signalGetPlayersInTeam(const QString&, const QString&);
    void signalSortPlayers();
    void signalDisplayQuery(const QString&);
    void signalDisplayTable(const QString &tableName);

private:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    void initLayOut();
    void initConnections();
    void deletePlayers();
    bool containID(int id);
    player* getPlayer(int id);
    void insertPlayersToSelection(QVector<player*> vector, QTableWidget *table);
    void insertComboBox(Tables table, QSqlTableModel* model);

    static MainWindow* m_instance;
    Ui::MainWindow *ui;
    QVector<player*> m_players;
    QVector<player*> m_pointGaurds;
    QVector<player*> m_shootingGaurd;
    QVector<player*> m_smallForward;
    QVector<player*> m_powerForward;
    QVector<player*> m_centers;
    Tables m_curTableEnum;
    Querys m_curQueryEnum;
    QString m_curTable;
    QString m_curQuery;

};

#endif // MAINWINDOW_H
