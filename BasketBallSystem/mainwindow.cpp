#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "popupmessagedialog.h"

MainWindow *MainWindow::m_instance = NULL;

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
    //popupMessageDialog *p = new popupMessageDialog(this);
    //p->show();
}

void MainWindow::initLayOut()
{
    ui->MainWindowStackedWidget->setCurrentIndex(GUI_FORM_ENTRANCE);
    QTimer::singleShot(3000, this, SLOT(slotTime()));
    emit signalPoulateComboBox("teamNames");
    ui->MainWindowBottomToolbar->setVisible(false);
}

void MainWindow::initConnections()
{
    connect(this, SIGNAL(signalPoulateComboBox(const QString)), DBManager::getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotPopulateComboBox(QSqlQueryModel *)));
    connect(GuiFormManager::getInstance(), SIGNAL(signalChangeForm(GuiForms)), this, SLOT(slotChangeForm(GuiForms)));
    connect(this, SIGNAL(signalGetPlayersInTeam(QString,QString)), DBManager::getInstance(), SLOT(slotDisplayQueryWithArg(QString,QString)));
    connect(DBManager::getInstance(), SIGNAL(signalParameterQueryResult(QSqlQuery*)), this, SLOT(slotReceivePlayearsInTeam(QSqlQuery *)));
    connect(this, SIGNAL(signalSortPlayers()), this, SLOT(slotSortPlayers()));
    connect(GuiFormManager::getInstance(), SIGNAL(signalEntrance(bool)), this, SLOT(slotEntranceForm(bool)));
    connect(this, SIGNAL(signalDisplayQuery(const QString)), DBManager::getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotHandleQuery(QSqlQueryModel*)));
    connect(this, SIGNAL(signalDisplayTable(const QString)), DBManager::getInstance(), SLOT(slotDisplayTable(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalTableResult(QSqlTableModel *)), this, SLOT(slotHandleTable(QSqlTableModel*)));
       connect(this, SIGNAL(signalSubmitReq()), DBManager::getInstance(), SLOT(slotHandleRequest()));

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
        int _id = m_players[i]->id();
        if(_id == id)
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

void MainWindow::insertPlayersToSelection(QVector<player *> vector, QTableWidget *table)
{
    table->setColumnCount(3);
    table->setColumnWidth(0, 200);
    table->setColumnWidth(1, 200);
    table->setColumnWidth(2, 200);
    table->setRowCount(1);
    table->setRowHeight(0, 170);
    int row = 0;
    int col = 0;
    for(int i = 0; i < vector.size(); ++i)
    {
        if(vector[i] != NULL)
        {
            if(col == 3)
            {
                row++;
                table->setRowCount(row + 1);
                table->setRowHeight(row, 170);
                col = 0;
            }
            player* p = vector[i]->copyPlayer();
            table->setCellWidget(row, col, p);
            col++;
        }
    }
}

MainWindow* MainWindow::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new MainWindow();
    }
    return m_instance;
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
    QString face = NULL;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    if(m_players.size() != 0)
    {
        this->deletePlayers();
    }
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
            p = new player(id, firstName, surName, teamColor, face, this);
            p->setPosition(query->value(3).toString());
            if(query->value(4).isValid())
            {
                p->setPosition(query->value(4).toString());
            }
            m_players.append(p);
        }
    }
    qDebug() << "all players: " << m_players;
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
    this->insertPlayersToSelection(m_players, ui->MainWindowPlayerSelectionAllPlayersTable);
    this->insertPlayersToSelection(m_pointGaurds, ui->MainWindowPlayerSelectionPointGaurdsTable);
    this->insertPlayersToSelection(m_smallForward, ui->MainWindowPlayerSelectionSmallForwardTable);
    this->insertPlayersToSelection(m_powerForward, ui->MainWindowPlayerSelectionPowerForwardTable);
    this->insertPlayersToSelection(m_shootingGaurd, ui->MainWindowPlayerSelectionShootingGaurdTable);
    this->insertPlayersToSelection(m_centers, ui->MainWindowPlayerSelectionCentersTable);
//    qDebug() << "all players: " << m_players;
//    qDebug() << "point: " << m_pointGaurds;
//    qDebug() << "shot: " << m_shootingGaurd;
//    qDebug() << "small: " << m_smallForward;
//    qDebug() << "power: " << m_powerForward;
    //    qDebug() << "center: " << m_centers;
}

void MainWindow::slotHandelePlayerPress(int id)
{
    player* p = this->getPlayer(id);
    positionLayout position = (positionLayout)ui->MainWindowPlayerSelectionStackedWidget->currentIndex();
    switch(position)
    {
        case POSITION_LAYOUT_POINT_GAURDS:
            ui->MainWindowGameManagmentCortImagePG->addWidget(p);
            ui->MainWindowGameManagmentCortImagePG->setCurrentWidget(p);
            break;
        case POSITION_LAYOUT_CENTERS:

            ui->MainWindowGameManagmentCortImageC->addWidget(p);
            ui->MainWindowGameManagmentCortImageC->setCurrentWidget(p);

            break;
        case POSITION_LAYOUT_POWER_FORWARD:

        ui->MainWindowGameManagmentCortImagePF->addWidget(p);
        ui->MainWindowGameManagmentCortImagePF->setCurrentWidget(p);

            break;
        case POSITION_LAYOUT_SHOOTING_GAURD:

        ui->MainWindowGameManagmentCortImageSG->addWidget(p);
        ui->MainWindowGameManagmentCortImageSG->setCurrentWidget(p);

            break;
        case POSITION_LAYOUT_SMALL_FORWARD:

        ui->MainWindowGameManagmentCortImageSF->addWidget(p);
        ui->MainWindowGameManagmentCortImageSF->setCurrentWidget(p);

            break;
        default:
            qDebug() << "bad input at player selection " << position;
    }
    GuiFormManager::getInstance()->changeForm( GUI_FORM_GAME_MANAGMENT);
}

void MainWindow::slotEntranceForm(bool enter)
{
    if(enter == true)
    {
        ui->MainWindowBottomToolbar->setVisible(false);
    }
    else
    {
        ui->MainWindowBottomToolbar->setVisible(true);
    }
}

void MainWindow::on_MainWindowTeamSelectionSelectBtn_released()
{
    ui->MainWindowTeamSelectionEditDataBtn->setEnabled(true);
    ui->MainWindowTeamSelectionGameManagmentBtn->setEnabled(true);
    emit signalGetPlayersInTeam("getPlayersInTeam", ui->MainWindowTeamSelectionComboBox->currentText());
}

void MainWindow::on_MainWindowTeamSelectionEditDataBtn_released()
{
   GuiFormManager::getInstance()->changeForm(GUI_FORM_EDIT_PLAYERS);
}

void MainWindow::on_MainWindowTeamSelectionGameManagmentBtn_released()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_GAME_MANAGMENT);
}


void MainWindow::on_MainWindowTeamSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
}

void MainWindow::on_MainWindowPlayerSelectionAllBtn_released()
{
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_ALL_PLAYERS);
}

void MainWindow::on_MainWindowPlayerSelectionPoinGaurdBtn_released()
{
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_POINT_GAURDS);
}

void MainWindow::on_MainWindowPlayerSelectionShootingGaurdBtn_released()
{
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_SHOOTING_GAURD);
}

void MainWindow::on_MainWindowPlayerSelectionSmallForwardBtn_released()
{
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_SMALL_FORWARD);
}

void MainWindow::on_MainWindowPlayerSelectionPowerForwardBtn_released()
{
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_POWER_FORWARD);
}

void MainWindow::on_MainWindowPlayerSelectionCenterBtn_released()
{
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_CENTERS);
}


void MainWindow::on_MainWindowGameManagmentCortImageSFB_released()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_PLAYER_SELECTION);
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_SMALL_FORWARD);
}

void MainWindow::on_MainWindowGameManagmentCortImagePGB_released()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_PLAYER_SELECTION);
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_POINT_GAURDS);
}

void MainWindow::on_MainWindowGameManagmentCortImageSGB_released()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_PLAYER_SELECTION);
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_SHOOTING_GAURD);
}

void MainWindow::on_MainWindowGameManagmentCortImagePFB_released()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_PLAYER_SELECTION);
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_POWER_FORWARD);
}

void MainWindow::on_MainWindowGameManagmentCortImageCB_released()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_PLAYER_SELECTION);
    ui->MainWindowPlayerSelectionStackedWidget->setCurrentIndex(POSITION_LAYOUT_CENTERS);
}

void MainWindow::on_MainWindowBottomToolbarReturnBtn_released()
{
    GuiFormManager::getInstance()->goBack();
}

void MainWindow::on_MainWindowBottomToolbarHomeBtn_released()
{
    GuiFormManager::getInstance()->goHome();
}

void MainWindow::on_MainWindowEditPlayersQCB_currentIndexChanged(int index)
{
    switch (index)
     {
       case Query_1:
         emit(signalDisplayQuery("Query1"));
        break;

       case Query_2:
         emit(signalDisplayQuery("Query2"));
        break;

       case Query_3:
        emit(signalDisplayQuery("Query3"));
        break;

       case Query_4:
         emit(signalDisplayQuery("Query4"));
        break;

    case Query_5:
      emit(signalDisplayQuery("Query5"));
     break;

    case Query_6:
     emit(signalDisplayQuery("Query6"));
     break;

    case Query_7:
      emit(signalDisplayQuery("Query7"));
     break;
    default:
        qDebug() << "bad query number" ;

    }

}
void MainWindow::slotHandleQuery(QSqlQueryModel *model)
{
    ui->MainWindowEditPlayersQTV->setModel(model);
}



void MainWindow::on_MainWindowEditPlayersTCB_currentIndexChanged(int index)
{
   switch (index)
   {
    case Table_1:
       this->m_curTable = "AssistanceTrainer";
       emit(signalDisplayTable("AssistanceTrainer"));
       break;

   case Table_2:
         this->m_curTable = "Cheerleaders";
      emit(signalDisplayTable("Cheerleaders"));
      break;

   case Table_3:
         this->m_curTable = "Game";
      emit(signalDisplayTable("Game"));
      break;

   case Table_4:
         this->m_curTable = "League";
      emit(signalDisplayTable("League"));
      break;

   case Table_5:
          this->m_curTable = "MainTrainer";
      emit(signalDisplayTable("MainTrainer"));
      break;

   case Table_6:
         this->m_curTable = "Player";
      emit(signalDisplayTable("Player"));
      break;

   case Table_7:
         this->m_curTable = "PlayesIn";
      emit(signalDisplayTable("PlayesIn"));
      break;

   case Table_8:
         this->m_curTable = "Position";
      emit(signalDisplayTable("Position"));
      break;

   case Table_9:
         this->m_curTable = "Season";
      emit(signalDisplayTable("Season"));
      break;

   case Table_10:
         this->m_curTable = "SeasonCycle";
      emit(signalDisplayTable("SeasonCycle"));
      break;

   case Table_11:
         this->m_curTable = "SecondaryPosition";
      emit(signalDisplayTable("SecondaryPosition"));
      break;

   case Table_12:
         this->m_curTable = "Statistic";
      emit(signalDisplayTable("Statistic"));
      break;

   case Table_13:
         this->m_curTable = "Team";
      emit(signalDisplayTable("Team"));
      break;

   case Table_14:
         this->m_curTable = "TeamInLeague";
      emit(signalDisplayTable("TeamInLeague"));
      break;

   case Table_15:
         this->m_curTable = "Trainer";
      emit(signalDisplayTable("Trainer"));
      break;
   }
}
void MainWindow::slotHandleTable(QSqlTableModel *model)
{
    model->insertRows(model->rowCount(), 1);
    ui->MainWindowEditPlayersQTV->setModel(model);
}

void MainWindow::on_MainWindowEditPlayersAdd_released()
{
    emit(signalSubmitReq());
    emit(signalDisplayTable(m_curTable));
}
