#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class fileExplorer;
}

class fileExplorer : public QDialog
{
    Q_OBJECT

public:
    explicit fileExplorer(QWidget *parent = 0);
    ~fileExplorer();

    void init();
    void initTreeView();
    void initListView();

private slots:
    void on_fileExplorerTreeView_clicked(const QModelIndex &index);
    void on_fileExplorerListView_clicked(const QModelIndex &index);

signals:
    void signalPublishFilePath(const QString&);

private:
    Ui::fileExplorer *ui;

    QFileSystemModel *m_dirModel;
    QFileSystemModel *m_fileModel;
};

#endif // FILEEXPLORER_H
