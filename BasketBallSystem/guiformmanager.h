#ifndef GUIFORMMANAGER_H
#define GUIFORMMANAGER_H

#include <QObject>

enum GuiForms
{
    GUI_FORM_ENTRANCE,
    GUI_FORM_TEAM_SELECTION
};

class GuiFormManager : public QObject
{
    Q_OBJECT
public:
    const GuiFormManager& getInstance() const;

    void changeForm(GuiForms newForm);

signals:

public slots:

private:
    explicit GuiFormManager(QObject *parent = 0);
};

#endif // GUIFORMMANAGER_H
