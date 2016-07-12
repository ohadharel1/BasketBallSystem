#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    initConnections();
    initLayOut();

}

void MainWindow::initLayOut()
{
    ui->MainWindowStackedWidget->setCurrentIndex(GUI_FORM_ENTRANCE);
    QTimer::singleShot(3000, this, SLOT(slotTime()));
    emit signalPoulateComboBox("teamNames");
}

void MainWindow::initConnections()
{
    connect(this, SIGNAL(signalPoulateComboBox(const QString)), DBManager::getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotPopulateComboBox(QSqlQueryModel *)));
    connect(GuiFormManager::getInstance(), SIGNAL(signalChangeForm(GuiForms)), this, SLOT(slotChangeForm(GuiForms)));
    connect(this, SIGNAL(signalGetPlayersInTeam(QString,QString)), DBManager::getInstance(), SLOT(slotDisplayQueryWithArg(QString,QString)));
    connect(DBManager::getInstance(), SIGNAL(signalParameterQueryResult(QSqlQuery*)), this, SLOT(slotReceivePlayearsInTeam(QSqlQuery *)));
    connect(this, SIGNAL(signalSortPlayers()), this, SLOT(slotSortPlayers()));
}

void MainWindow::deletePlayers()
{
    while(m_players.size() != 0)
    {
        player* p = m_players.last();
        if(p != NULL)
        {
            p->playerDelete();
        }
        m_players.removeLast();
    }
    m_pointGaurds.clear();
    m_shootingGaurd.clear();
    m_smallForward.clear();
    m_powerForward.clear();
    m_centers.clear();
}

bool MainWindow::containID(int id)
{
    for(int i = 0; i < m_players.size(); ++i)
    {
        if(m_players[i]->id() == id)
        {
            return true;
        }
    }
    return false;
}

player* MainWindow::getPlayer(int id)
{
    for(int i = 0; i < m_players.size(); ++i)
    {
        if(m_players[i]->id() == id)
        {
            return m_players[i];
        }
    }
    return NULL;
}

void MainWindow::insertPlayersToSelection(QVector<player *> vector)
{
    QLayoutItem *item;
    while((item = ui->MainWindowPlayerSelectionGridLayout->takeAt(0)) != 0)
    {
        ui->MainWindowPlayerSelectionGridLayout->removeItem(item);
    }
    int row = 0;
    int col = 0;
    for(int i = 0; i < vector.size(); ++i)
    {
        if(vector[i] != NULL)
        {
            if(col == 2)
            {
                row++;
                col = 0;
            }
            ui->MainWindowPlayerSelectionGridLayout->addWidget(vector[i], row, col, 100, 3);
            col++;
        }
    }
}

void MainWindow::slotPopulateComboBox(QSqlQueryModel *model)
{
    ui->MainWindowTeamSelectionComboBox->setModel(model);
}

void MainWindow::slotChangeForm(GuiForms form)
{
    ui->MainWindowStackedWidget->setCurrentIndex(form);
}

void MainWindow::slotTime()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_TEAM_SELECTION);
}

void MainWindow::slotReceivePlayearsInTeam(QSqlQuery *query)
{
    int id;
    QString firstName;
    QString surName;
    QString teamColor;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    switch(qrand() % 2 + 0)
    {
        case 0:
            teamColor = "greenTeam";
            break;
        case 1:
            teamColor = "redTeam";
            break;
        default:
            break;
    }
    while(query->next())
    {
        player* p;
        id = query->value(0).toInt();
        if(this->containID(id))
        {
            p = this->getPlayer(id);
            p->setPosition(query->value(4).toString());
        }
        else
        {
            firstName = query->value(1).toString();
            surName = query->value(2).toString();
            p = new player(id, firstName, surName, teamColor, this);
            p->setPosition(query->value(3).toString());
            if(query->value(4).isValid())
            {
                p->setPosition(query->value(4).toString());
            }
            m_players.append(p);
        }
    }
    emit signalSortPlayers();
}

void MainWindow::slotSortPlayers()
{
    for(int i = 0; i < m_players.size(); ++i)
    {
        if(m_players[i]->getPlayerPosition(POINT_GUARD))
        {
            m_pointGaurds.append(m_players[i]);
        }
        if(m_players[i]->getPlayerPosition(SHOOTING_GUARD))
        {
            m_shootingGaurd.append(m_players[i]);
        }
        if(m_players[i]->getPlayerPosition(SMALL_FORWARD))
        {
            m_smallForward.append(m_players[i]);
        }
        if(m_players[i]->getPlayerPosition(POWER_FORWARD))
        {
            m_powerForward.append(m_players[i]);
        }
        if(m_players[i]->getPlayerPosition(CENTER))
        {
            m_centers.append(m_players[i]);
        }
    }
//    qDebug() << "all players: " << m_players;
//    qDebug() << "point: " << m_pointGaurds;
//    qDebug() << "shot: " << m_shootingGaurd;
//    qDebug() << "small: " << m_smallForward;
//    qDebug() << "power: " << m_powerForward;
//    qDebug() << "center: " << m_centers;
}

void MainWindow::on_MainWindowTeamSelectionSelectBtn_released()
{
    ui->MainWindowTeamSelectionEditDataBtn->setEnabled(true);
    ui->MainWindowTeamSelectionGameManagmentBtn->setEnabled(true);
    emit signalGetPlayersInTeam("getPlayersInTeam", ui->MainWindowTeamSelectionComboBox->currentText());
}

void MainWindow::on_MainWindowTeamSelectionEditDataBtn_released()
{

}

void MainWindow::on_MainWindowTeamSelectionGameManagmentBtn_released()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_PLAYER_SELECTION);
}


void MainWindow::on_MainWindowTeamSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
}

void MainWindow::on_MainWindowPlayerSelectionAllBtn_released()
{
    this->insertPlayersToSelection(m_players);
}

void MainWindow::on_MainWindowPlayerSelectionPoinGaurdBtn_released()
{
    this->insertPlayersToSelection(m_pointGaurds);
}

void MainWindow::on_MainWindowPlayerSelectionShootingGaurdBtn_released()
{
    this->insertPlayersToSelection(m_shootingGaurd);
}

void MainWindow::on_MainWindowPlayerSelectionSmallForwardBtn_released()
{
    this->insertPlayersToSelection(m_smallForward);
}

void MainWindow::on_MainWindowPlayerSelectionPowerForwardBtn_released()
{
    this->insertPlayersToSelection(m_powerForward);
}

void MainWindow::on_MainWindowPlayerSelectionCenterBtn_released()
{
    this->insertPlayersToSelection(m_centers);
}
