#ifndef GUIFORMMANAGER_H
#define GUIFORMMANAGER_H

#include <QObject>
#include <QStack>

enum GuiForms
{
    GUI_FORM_ENTRANCE,
    GUI_FORM_TEAM_SELECTION,
    GUI_FORM_GAME_MANAGMENT,
    GUI_FORM_PLAYER_SELECTION,

    GUI_FORM_SIZE
};

class GuiFormManager : public QObject
{
    Q_OBJECT
public:
    static GuiFormManager* getInstance();

    void changeForm(GuiForms newForm);
    GuiForms getCurrentForm();
    void goHome();
    void goBack();

signals:
    void signalChangeForm(GuiForms);
    void signalEntrance(bool);
    void signalTeamSelection(bool);
    void signalGameManagment(bool);
    void signalPlayerSelection(bool);
public slots:

private:
    explicit GuiFormManager(QObject *parent = 0);
    ~GuiFormManager();

    void exitForm();
    void enterForm(GuiForms);

    static GuiFormManager* m_instance;
    GuiForms m_currentForm;
    QStack<GuiForms> m_formStack;
};

#endif // GUIFORMMANAGER_H
