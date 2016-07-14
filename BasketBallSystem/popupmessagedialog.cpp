#include "popupmessagedialog.h"
#include "ui_popupmessagedialog.h"
#include "mainwindow.h"
#include "mainwindowlayout.h"
#include <QCloseEvent>

popupMessageDialog *popupMessageDialog::m_instance = NULL;

popupMessageDialog::popupMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::popupMessageDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    ui->popupMessageDialogTextEdit->setVisible(false);
}

popupMessageDialog::~popupMessageDialog()
{
    delete ui;
}

popupMessageDialog *popupMessageDialog::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new popupMessageDialog();
    }
    return m_instance;
}

void popupMessageDialog::addText(const QString &text)
{
    ui->popupMessageDialogTextEdit->append(text);
}

void popupMessageDialog::showPopupMessage(popupMessageType type)
{
    ui->popupMessageDialogStackedWidget->setCurrentIndex(type);
    this->show();
}

void popupMessageDialog::setCSVResult(int lines, int errors)
{
    ui->popupMessageDialogCSVLinesLabel->setText(QString::number(lines));
    ui->popupMessageDialogCSVErrorsLabel->setText(QString::number(lines - errors));
}

void popupMessageDialog::closeEvent(QCloseEvent *event)
{
    ui->popupMessageDialogTextEdit->clear();
    event->accept();
}

void popupMessageDialog::on_popupMessageDialogOKBtn_released()
{
    this->close();
}

void popupMessageDialog::on_popupMessageDialogDetailBtn_toggled(bool checked)
{
    if(checked)
    {
        ui->popupMessageDialogTextEdit->setVisible(true);
    }
    else
    {
        ui->popupMessageDialogTextEdit->setVisible(false);
    }
}
