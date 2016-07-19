#ifndef POPUPMESSAGEDIALOG_H
#define POPUPMESSAGEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

enum popupMessageType
{
    POPUP_MESSAGE_ERROR,
    POPUP_MESSAGE_INFO,
    POPUP_MESSAGE_CSV_SUM,
    POPUP_MESSAGE_GOOD,

    POPUP_MESSAGE_SIZE
};

namespace Ui {
class popupMessageDialog;
}

class popupMessageDialog : public QDialog
{
    Q_OBJECT

public:
    ~popupMessageDialog();

    static popupMessageDialog* getInstance();
    void addText(const QString&);
    void showPopupMessage(popupMessageType type);
    void setCSVResult(int lines, int errors);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_popupMessageDialogOKBtn_released();

    void on_popupMessageDialogDetailBtn_toggled(bool checked);

private:
    explicit popupMessageDialog(QWidget *parent = 0);
    Ui::popupMessageDialog *ui;

    static popupMessageDialog *m_instance;
};

#endif // POPUPMESSAGEDIALOG_H
