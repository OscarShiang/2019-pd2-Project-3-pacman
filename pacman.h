#ifndef PACMAN_H
#define PACMAN_H

#include <QPixmap>
#include <QTimer>
#include "character.h"
#include "compass.h"

class Pacman: public Character {
    Q_OBJECT
public:
    Pacman(Compass *compass_ipt);

    void setDirection(QPoint dir);

public slots:
    void move();
    void switchAnimate();

private:
    QPoint direction;
    QPoint tmpDir;
    QTimer *switchTimer;
    QPixmap pic[3];
    Compass *compass;
};

#endif // PACMAN_H
