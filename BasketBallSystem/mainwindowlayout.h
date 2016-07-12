#ifndef MAINWINDOWLAYOUT_H
#define MAINWINDOWLAYOUT_H

#include <QMainWindow>
#include <QVector>
#include "dbmanager.h"
#include "fileexplorer.h"
#include "player.h"

namespace Ui {
class MainWindowLayout;
}

class MainWindowLayout : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowLayout(QWidget *parent = 0);
    ~MainWindowLayout();

    void init();
    void initConnections();

public slots:
    void slotHandleQuery(QSqlQueryModel*);
    void slotHandleTable(QSqlTableModel*);
    void slotHandleFilePath(const QString&);

signals:
    void signalDisplayQuery(const QString &queryName);
    void signalDisplayTable(const QString &tableName);
    void signalSubmitReq();
    void signalProccessCSV(const QString&);

private slots:

    void on_tableComboBox_currentIndexChanged(const QString &arg1);

    void on_queryComboBox_currentIndexChanged(const QString &arg1);

    void on_pushSubmit_released();

    void on_pushUndo_released();

    void on_pushDelete_released();

    void on_browseFileBtn_released();

    void on_uploadFileBtn_released();

private:
    void deletePlayers();

    Ui::MainWindowLayout *m_ui;
    DBManager *m_dbManager;
    fileExplorer m_fileExplorer;
    QString m_curTable;
    QString m_curQuery;
};

#endif // MAINWINDOW_H
