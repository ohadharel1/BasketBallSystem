#include "player.h"
#include "ui_player.h"
#include <QDebug>
#include <QTime>


player::player(int id, QString firstName, QString surName, QString teamColor, QString face, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::player),
    m_id(id),
    m_firstName(firstName),
    m_surName(surName),
    m_teamColor(teamColor),
    m_face(face)
{
    ui->setupUi(this);
    init();
    qDebug() << "id: " << m_id << "; firstName: " << m_firstName << "; lastName: " << m_surName << "; teamColor: " << m_teamColor;
}

player::~player()
{
    delete ui;
}

player *player::copyPlayer()
{
    player *p = new player(m_id, m_firstName, m_surName, m_teamColor, m_face);
    return p;
}

void player::init()
{
    for(int i = 0; i < POSITION_SIZE; ++i)
    {
        m_playerPositions[i] = false;
    }
    if(m_face == NULL)
    {
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());
        switch(qrand() % 8 + 0)
        {
            case 0:
                m_face = "face1";
                break;
            case 1:
                m_face = "face2";
                break;
            case 2:
                m_face = "face3";
                break;
            case 3:
                m_face = "face4";
                break;
            case 4:
                m_face = "face5";
                break;
            case 5:
                m_face = "face6";
                break;
            case 6:
                m_face = "face7";
                break;
            case 7:
                m_face = "face8";
                break;
            default:
                qDebug() << "face not good";
        }
    }
    m_imagePath = "background-image: url(:/Resources/Resources/" + m_teamColor + "/" + m_face + ".png);";
    qDebug() << m_imagePath;
    ui->playerPushButton->setStyleSheet(m_imagePath);
    this->setFixedHeight(200);
    this->setFixedWidth(200);
}

void player::playerDelete()
{
    this->~player();
}

QString player::firstName() const
{
    return m_firstName;
}

QString player::surName() const
{
    return m_surName;
}

int player::id() const
{
    return m_id;
}

void player::setPosition(const QString &pos)
{
    if(pos == "Point gaurd")
    {
        m_playerPositions[POINT_GUARD] = true;
    }
    else if(pos == "Shooting guard")
    {
        m_playerPositions[SHOOTING_GUARD] = true;
    }
    else if(pos == "Small forward")
    {
        m_playerPositions[SMALL_FORWARD] = true;
    }
    else if(pos == "Power forward")
    {
        m_playerPositions[POWER_FORWARD] = true;
    }
    else if(pos == "Center")
    {
        m_playerPositions[CENTER] = true;
    }
    else
    {
        qDebug() << "bad argument for possition: " << pos;
    }

}

bool player::getPlayerPosition(positions pos)
{
    return m_playerPositions[pos];
}

//QString player::imagePath() const
//{
//    return m_imagePath;
//}


