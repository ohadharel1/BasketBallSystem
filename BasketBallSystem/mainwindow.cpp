#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "popupmessagedialog.h"
#include <QMovie>

MainWindow *MainWindow::m_instance = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_players(),
    m_pointGaurds(),
    m_shootingGaurd(),
    m_smallForward(),
    m_powerForward(),
    m_centers(),
    m_curTableEnum(TABLE_NONE),
    m_curQueryEnum(Query_NONE),
    m_curTable(),
    m_curQuery(),
    m_tableModel(NULL),
    m_isNewRecord(false),
    m_fileExplorer(),
    m_animation(new QMovie())
{
    ui->setupUi(this);
    init();
}


MainWindow::~MainWindow()
{
    this->deletePlayers();
    delete m_animation;
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "BasketBall System",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        this->~MainWindow();
        event->accept();
    }
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
    ui->MainWindowBottomToolbar->setVisible(false);
}

void MainWindow::initConnections()
{
    connect(this, SIGNAL(signalPoulateComboBox(const QString)), DBManager::getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotPopulateComboBox(QSqlQueryModel *)));
    connect(GuiFormManager::getInstance(), SIGNAL(signalChangeForm(GuiForms)), this, SLOT(slotChangeForm(GuiForms)));
    connect(this, SIGNAL(signalGetPlayersInTeam(QString,QStringList, bool)), DBManager::getInstance(), SLOT(slotDisplayQueryWithArgs(QString,QStringList, bool)));
    connect(this, SIGNAL(signalDisplatQueryWithArgs(QString,QStringList, bool)), DBManager::getInstance(), SLOT(slotDisplayQueryWithArgs(QString,QStringList, bool)));
    connect(DBManager::getInstance(), SIGNAL(signalParameterQueryResult(QSqlQuery*)), this, SLOT(slotReceivePlayearsInTeam(QSqlQuery *)));
    connect(this, SIGNAL(signalSortPlayers()), this, SLOT(slotSortPlayers()));
    connect(GuiFormManager::getInstance(), SIGNAL(signalEntrance(bool)), this, SLOT(slotEntranceForm(bool)));
    connect(this, SIGNAL(signalDisplayQuery(const QString)), DBManager::getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotHandleQuery(QSqlQueryModel*)));
    connect(this, SIGNAL(signalDisplayTable(const QString)), DBManager::getInstance(), SLOT(slotDisplayTable(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalTableResult(QSqlTableModel *)), this, SLOT(slotHandleTable(QSqlTableModel*)));
    connect(this, SIGNAL(signalSubmitReq()), DBManager::getInstance(), SLOT(slotHandleRequest()));
    connect(this, SIGNAL(signalProccessCSV(QString)), DBManager::getInstance(), SLOT(slotHandleCSVProccessRequest(QString)));
    connect(&m_fileExplorer, SIGNAL(signalPublishFilePath(QString)), this, SLOT(slotHandleFilePath(QString)));
    connect(GuiFormManager::getInstance(), SIGNAL(signalTeamSelection(bool)), this, SLOT(slotTeamSelectionForm(bool)));
    connect(GuiFormManager::getInstance(), SIGNAL(signalEditData(bool)), this, SLOT(slotEditDataForm(bool)));
    connect(GuiFormManager::getInstance(), SIGNAL(signalGameManagment(bool)), this, SLOT(slotGameManagment(bool)));
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
    while(m_pointGaurds.size() != 0)
    {
        player* p = m_pointGaurds.last();
        if(p != NULL)
        {
            p->playerDelete();
        }
        m_pointGaurds.removeLast();
    }
    while(m_shootingGaurd.size() != 0)
    {
        player* p = m_shootingGaurd.last();
        if(p != NULL)
        {
            p->playerDelete();
        }
        m_shootingGaurd.removeLast();
    }
    while(m_smallForward.size() != 0)
    {
        player* p = m_smallForward.last();
        if(p != NULL)
        {
            p->playerDelete();
        }
        m_smallForward.removeLast();
    }
    while(m_powerForward.size() != 0)
    {
        player* p = m_powerForward.last();
        if(p != NULL)
        {
            p->playerDelete();
        }
        m_powerForward.removeLast();
    }
    while(m_centers.size() != 0)
    {
        player* p = m_centers.last();
        if(p != NULL)
        {
            p->playerDelete();
        }
        m_centers.removeLast();
    }
    ui->MainWindowPlayerSelectionPowerForwardTable->clear();
    ui->MainWindowPlayerSelectionShootingGaurdTable->clear();
    ui->MainWindowPlayerSelectionPointGaurdsTable->clear();
    ui->MainWindowPlayerSelectionCentersTable->clear();
    ui->MainWindowPlayerSelectionSmallForwardTable->clear();
//    m_centers.clear();
//    m_pointGaurds.clear();
//    m_powerForward.clear();
//    m_smallForward.clear();
//    m_shootingGaurd.clear();
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
            //player* p = vector[i]->copyPlayer();
            table->setCellWidget(row, col, vector[i]);
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
        player* p;
        if(m_players[i]->getPlayerPosition(POINT_GUARD))
        {
            p = m_players[i]->copyPlayer();
            m_pointGaurds.append(p);
        }
        if(m_players[i]->getPlayerPosition(SHOOTING_GUARD))
        {
            p = m_players[i]->copyPlayer();
            m_shootingGaurd.append(p);
        }
        if(m_players[i]->getPlayerPosition(SMALL_FORWARD))
        {
            p = m_players[i]->copyPlayer();
            m_smallForward.append(p);
        }
        if(m_players[i]->getPlayerPosition(POWER_FORWARD))
        {
            p = m_players[i]->copyPlayer();
            m_powerForward.append(p);
        }
        if(m_players[i]->getPlayerPosition(CENTER))
        {
            p = m_players[i]->copyPlayer();
            m_centers.append(p);
        }
    }
//    this->insertPlayersToSelection(m_players, ui->MainWindowPlayerSelectionAllPlayersTable);
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
//    ui->MainWindowTeamSelectionEditDataBtn->setEnabled(true);
//    ui->MainWindowTeamSelectionGameManagmentBtn->setEnabled(true);
//    QStringList args;
//    args << ui->MainWindowTeamSelectionComboBox->currentText();
//    emit signalGetPlayersInTeam("getPlayersInTeam", args);
}

void MainWindow::on_MainWindowTeamSelectionEditDataBtn_released()
{
   GuiFormManager::getInstance()->changeForm(GUI_FORM_EDIT_PLAYERS);
}

void MainWindow::on_MainWindowTeamSelectionGameManagmentBtn_released()
{
    QStringList args;
    args << ui->MainWindowTeamSelectionComboBox->currentText();
    emit signalGetPlayersInTeam("getPlayersInTeam", args, false);
    GuiFormManager::getInstance()->changeForm(GUI_FORM_GAME_MANAGMENT);
}


void MainWindow::on_MainWindowTeamSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
//    ui->MainWindowTeamSelectionGameManagmentBtn->setEnabled(true);
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
    m_isNewRecord = false;
    m_curTableEnum = TABLE_NONE;
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

    case Query_8:
      emit(signalDisplayQuery("Query8"));
     break;

    case Query_NONE:
        ui->MainWindowEditPlayersQTV->setModel(NULL);
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
    m_isNewRecord = false;
    m_curQueryEnum = Query_NONE;
    switch (index)
    {
        case TABLE_AssistanceTrainer:
            emit(signalDisplayTable("AssistanceTrainer"));
            m_curTableEnum = TABLE_AssistanceTrainer;
            m_curTable = "AssistanceTrainer";
            break;

        case TABLE_Cheerleaders:
            emit(signalDisplayTable("Cheerleaders"));
            m_curTableEnum = TABLE_Cheerleaders;
            m_curTable = "Cheerleaders";
            break;

        case TABLE_Game:
            emit(signalDisplayTable("Game"));
            m_curTableEnum = TABLE_Game;
            m_curTable = "Game";
            break;

        case TABLE_League:
            emit(signalDisplayTable("League"));
            m_curTableEnum = TABLE_League;
            m_curTable = "League";
            break;

        case TABLE_MainTrainer:
            emit(signalDisplayTable("MainTrainer"));
            m_curTableEnum = TABLE_MainTrainer;
            m_curTable = "MainTrainer";
            break;

        case TABLE_Player:
            emit(signalDisplayTable("Player"));
            m_curTableEnum = TABLE_Player;
            m_curTable = "Player";
            break;

        case TABLE_PlayesIn:
            emit(signalDisplayTable("PlayesIn"));
            m_curTableEnum = TABLE_PlayesIn;
            m_curTable = "PlayesIn";
            break;

        case TABLE_Position:
            emit(signalDisplayTable("Position"));
            m_curTableEnum = TABLE_PlayesIn;
            m_curTable = "Position";
            break;

        case TABLE_Season:
            emit(signalDisplayTable("Season"));
            m_curTableEnum = TABLE_Season;
            m_curTable = "Season";
            break;

        case TABLE_SeasonCycle:
            emit(signalDisplayTable("SeasonCycle"));
            m_curTableEnum = TABLE_SeasonCycle;
            m_curTable = "SeasonCycle";
            break;

        case TABLE_SecondaryPosition:
            emit(signalDisplayTable("SecondaryPosition"));
            m_curTableEnum = TABLE_SecondaryPosition;
            m_curTable = "SecondaryPosition";
            break;

        case TABLE_Statistic:
            emit(signalDisplayTable("Statistic"));
            m_curTableEnum = TABLE_Statistic;
            m_curTable = "Statistic";
            break;

        case TABLE_Team:
            emit(signalDisplayTable("Team"));
            m_curTableEnum = TABLE_Team;
            m_curTable = "Team";
            break;

        case TABLE_TeamInLeague:
            emit(signalDisplayTable("TeamInLeague"));
            m_curTableEnum = TABLE_TeamInLeague;
            m_curTable = "TeamInLeague";
            break;

        case TABLE_Trainer:
            emit(signalDisplayTable("Trainer"));
            m_curTableEnum = TABLE_Trainer;
            m_curTable = "Trainer";
            break;

        case TABLE_NONE:
            ui->MainWindowEditPlayersQTV->setModel(NULL);
            break;

        default:
            qDebug() << "wrong table selected";
   }
}
void MainWindow::slotHandleTable(QSqlTableModel *model)
{
    ui->MainWindowEditPlayersQTV->setModel(model);
    m_tableModel = model;
}

void MainWindow::slotHandleFilePath(const QString &path)
{
    ui->filePathTextBox->setText(path);
}

void MainWindow::slotTeamSelectionForm(bool enter)
{
    if(enter == true)
    {
        emit signalPoulateComboBox("teamNames");
        ui->MainWindowTeamSelectionGameManagmentBtn->setEnabled(false);
        m_animation->setFileName(":/Resources/Resources/animated-basketball-image.gif");
        ui->MainWindowTeamSelectionAnimation->setMovie(m_animation);
        m_animation->start();
    }
    else
    {
        m_animation->stop();
    }
}

void MainWindow::slotEditDataForm(bool)
{
    m_tableModel = NULL;
    ui->MainWindowEditPlayersQTV->setModel(m_tableModel);
}

void MainWindow::slotGameManagment(bool enter)
{
    if(enter == true)
    {
        m_animation->setFileName(":/Resources/Resources/basketball_ref_time_out_lw.gif");
        ui->MainWindowGameManagmentAnimation->setMovie(m_animation);
        m_animation->start();
    }
    else
    {
        m_animation->stop();
    }
}

void MainWindow::on_MainWindowEditPlayersAdd_released()
{
    if(m_curTableEnum != TABLE_NONE)
    {
        if(m_isNewRecord == false)
        {
            m_tableModel->insertRows(m_tableModel->rowCount(), 1);
            ui->MainWindowEditPlayersQTV->scrollToBottom();
            m_isNewRecord = true;
        }
    }
}

void MainWindow::on_MainWindowEditPlayersDelete_released()
{
    if(m_curTableEnum != TABLE_NONE)
    {
        QSqlTableModel *model = DBManager::getInstance()->getTableModel();
        model->removeRow(this->ui->MainWindowEditPlayersQTV->currentIndex().row());
        if(model->submitAll())
        {
            model->database().commit();
            popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_GOOD);
        }
        else
        {
            model->database().rollback();
            popupMessageDialog::getInstance()->addText(model->lastError().text());
            popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_ERROR);
        }
        model->select();
    }
}

void MainWindow::on_MainWindowEditPlayersSave_released()
{
    if(m_curTableEnum != TABLE_NONE)
    {
        if(m_isNewRecord == true)
        {
            QVariant content;
            QStringList args;
            for(int i = 0; i < m_tableModel->columnCount(); ++i)
            {
                content = m_tableModel->data(m_tableModel->index(m_tableModel->rowCount() - 1, i), Qt::DisplayRole);
                args << content.toString();
            }
            DBManager::getInstance()->slotDisplayQueryWithArgs("addRecordTo" + m_curTable, args, true);
        }
        else
        {
            emit signalSubmitReq();
        }
        m_tableModel->select();

        m_isNewRecord = false;
    }
}

void MainWindow::on_MainWindowEditPlayersUndo_released()
{
    if(m_curTableEnum != TABLE_NONE)
    {
        if(m_isNewRecord == true)
        {
            m_isNewRecord = false;
        }
        m_tableModel->select();
    }
}

void MainWindow::on_MainWindowEditPlayersBrowesBtn_released()
{
    m_fileExplorer.setModal(true);
    m_fileExplorer.exec();
}

void MainWindow::on_MainWindowEditPlayersUploadBtn_released()
{
    emit signalProccessCSV(ui->filePathTextBox->text());
    this->on_MainWindowEditPlayersTCB_currentIndexChanged(TABLE_TeamInLeague);
}

void MainWindow::on_MainWindowTeamSelectionComboBox_activated(int index)
{
    Q_UNUSED(index);
    ui->MainWindowTeamSelectionGameManagmentBtn->setEnabled(true);
}

//void MainWindow::slotHandleRequest()
//{
//    this->m_tableModel->database().transaction();
//    if(this->m_tableModel->submitAll())
//    {
//        popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_GOOD);
//        if(this->m_tableModel->database().commit())
//        {
//            this->m_tableModel->select();
//            popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_GOOD);
//        }
//        else
//        {
//            popupMessageDialog::getInstance()->addText(m_tableModel->lastError().text());
//            popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_ERROR);
//        }
//    }
//    else
//    {
//        this->m_tableModel->database().rollback();
//        popupMessageDialog::getInstance()->addText(m_tableModel->lastError().text());
//        popupMessageDialog::getInstance()->showPopupMessage(POPUP_MESSAGE_ERROR);
//    }
//}
