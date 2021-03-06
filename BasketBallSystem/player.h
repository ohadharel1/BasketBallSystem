#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMainWindow>

enum positions
{
    POINT_GUARD,
    SHOOTING_GUARD,
    SMALL_FORWARD,
    POWER_FORWARD,
    CENTER,

    POSITION_SIZE
};

namespace Ui {
class player;
}

class player : public QWidget
{
    Q_OBJECT

public:
    explicit player(int id, QString firstName, QString surName, QString teamColor, QString m_face = NULL, QWidget *parent = 0);
    ~player();
    player* copyPlayer();

    void init();
    void playerDelete();

    QString firstName() const;
    QString surName() const;
    int id() const;

//    QString imagePath() const;
    void setPosition(const QString&);
    bool getPlayerPosition(positions);

private slots:
    void on_playerPushButton_released();

signals:
    void signalPlayerPress(int id);


private:
    Ui::player *ui;

    int m_id;
    QString m_firstName;
    QString m_surName;
    QString m_teamColor;
    QString m_imagePath;
    QString m_face;
    bool m_playerPositions[POSITION_SIZE];
};

#endif // PLAYER_H
