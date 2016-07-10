#include "fileexplorer.h"
#include "ui_fileexplorer.h"
#include <QFileSystemModel>
#include <QStandardPaths>
#include <QDebug>

fileExplorer::fileExplorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fileExplorer),
    m_dirModel(new QFileSystemModel(this)),
    m_fileModel(new QFileSystemModel(this))
{
    ui->setupUi(this);
    qDebug() << QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    this->init();
}

fileExplorer::~fileExplorer()
{
    delete ui;
    delete m_dirModel;
    delete m_fileModel;
}

void fileExplorer::init()
{
    this->initListView();
    this->initTreeView();
}

void fileExplorer::initTreeView()
{
    QString path = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    this->m_dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    this->m_dirModel->setRootPath(path);
    ui->fileExplorerTreeView->setModel(m_dirModel);
}

void fileExplorer::initListView()
{
    QString path = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    this->m_fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    QStringList fileTypes;
    fileTypes << "*.csv";
    this->m_fileModel->setNameFilters(fileTypes);
    this->m_fileModel->setNameFilterDisables(false);
    this->m_fileModel->setRootPath(path);
    ui->fileExplorerListView->setModel(m_fileModel);
}

void fileExplorer::on_fileExplorerTreeView_clicked(const QModelIndex &index)
{
    QString fPath = this->m_dirModel->fileInfo(index).absoluteFilePath();
    ui->fileExplorerListView->setRootIndex(m_fileModel->setRootPath(fPath));
}

void fileExplorer::on_fileExplorerListView_clicked(const QModelIndex &index)
{
    emit(signalPublishFilePath(this->m_fileModel->fileInfo(index).absoluteFilePath()));
    this->close();
}
