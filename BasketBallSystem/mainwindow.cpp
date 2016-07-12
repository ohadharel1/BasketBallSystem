#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

void MainWindow::init()
{
    initConnections();
    initLayOut();

}

void MainWindow::initLayOut()
{
    //GuiFormManager::getInstance()->changeForm(GUI_FORM_ENTRANCE);
    QTimer::singleShot(5000, this, SLOT(updateCaption()));
    emit signalPoulateComboBox("teamNames");
}

void MainWindow::initConnections()
{
    connect(this, SIGNAL(signalPoulateComboBox(const QString)), DBManager::getInstance(), SLOT(slotDisplayQuery(const QString)));
    connect(DBManager::getInstance(), SIGNAL(signalQueryResult(QSqlQueryModel *)), this, SLOT(slotPopulateComboBox(QSqlQueryModel *)));
    connect(GuiFormManager::getInstance(), SIGNAL(signalChangeForm(GuiForms)), this, SLOT(slotChangeForm(GuiForms)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotPopulateComboBox(QSqlQueryModel *model)
{
    ui->MainWindowTeamSelectionComboBox->setModel(model);
}

void MainWindow::slotChangeForm(GuiForms form)
{
    ui->MainWindowStackedWidget->setCurrentIndex(form);
}

void MainWindow::updateCaption()
{
    GuiFormManager::getInstance()->changeForm(GUI_FORM_TEAM_SELECTION);
}

void MainWindow::on_MainWindowTeamSelectionSelectBtn_released()
{
    ui->MainWindowTeamSelectionEditDataBtn->setEnabled(true);
    ui->MainWindowTeamSelectionGameManagmentBtn->setEnabled(true);
}

void MainWindow::on_MainWindowTeamSelectionEditDataBtn_released()
{

}

void MainWindow::on_MainWindowTeamSelectionGameManagmentBtn_released()
{

}


void MainWindow::on_MainWindowTeamSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
}
