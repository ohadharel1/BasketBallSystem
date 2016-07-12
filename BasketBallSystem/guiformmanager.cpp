#include "guiformmanager.h"
#include "mainwindow.h"

GuiFormManager *GuiFormManager::m_instance = NULL;

GuiFormManager::GuiFormManager(QObject *parent) :
    QObject(parent),
    m_currentForm(GUI_FORM_ENTRANCE),
    m_formStack()
{

}

GuiFormManager::~GuiFormManager()
{
    delete m_instance;
}

void GuiFormManager::exitForm()
{
    switch(m_formStack.pop())
    {
        case GUI_FORM_ENTRANCE:
            emit signalEntrance(false);
            break;
        case GUI_FORM_PLAYER_SELECTION:
            emit signalPlayerSelection(false);
            break;
        case GUI_FORM_TEAM_SELECTION:
            emit signalTeamSelection(false);
            break;
        case GUI_FORM_GAME_MANAGMENT:
            emit signalGameManagment(false);
            break;
        default:
            break;
    }
}

void GuiFormManager::enterForm(GuiForms form)
{
    switch(form)
    {
        case GUI_FORM_ENTRANCE:
            emit signalEntrance(true);
            break;
        case GUI_FORM_PLAYER_SELECTION:
            emit signalPlayerSelection(true);
            break;
        case GUI_FORM_TEAM_SELECTION:
            emit signalTeamSelection(true);
            break;
        case GUI_FORM_GAME_MANAGMENT:
            emit signalGameManagment(true);
            break;
        default:
            break;
    }
}

GuiFormManager *GuiFormManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new GuiFormManager();
        m_instance->m_formStack.push(GUI_FORM_ENTRANCE);
    }
    return m_instance;
}

void GuiFormManager::changeForm(GuiForms newForm)
{
    if(newForm != m_formStack.top())
    {
        this->exitForm();
        m_formStack.push(newForm);
        emit signalChangeForm(newForm);
        this->enterForm(newForm);
    }
}

GuiForms GuiFormManager::getCurrentForm()
{
    return m_formStack.top();
}

void GuiFormManager::goHome()
{
    m_formStack.clear();
    this->changeForm(GUI_FORM_TEAM_SELECTION);
}

void GuiFormManager::goBack()
{
    this->exitForm();
    emit signalChangeForm(m_formStack.top());
    this->enterForm(m_formStack.top());
}


