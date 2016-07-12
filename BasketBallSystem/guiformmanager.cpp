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
    emit signalOnExitForm(this->getCurrentForm());
    m_formStack.push(newForm);
    emit signalChangeForm(newForm);
    emit signalOnEnterForm(newForm);
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
    emit signalOnExitForm(m_formStack.pop());
    emit signalChangeForm(m_formStack.top());
    emit signalOnEnterForm(m_formStack.top());
}


