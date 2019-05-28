#include "blinky.h"
#include <QString>
#include <cmath>
#include <QDebug>

Blinky::Blinky(Compass *compass_ipt): Ghost (compass_ipt), compass(compass_ipt) {
    loadPicture(":/pic/ghost/blinky/");
    setDirection(Dir::Right);
    setCritical(QPoint(1, 27));
}

QPoint Blinky::setTarget() {
    QPoint player = compass->getPlayerPos();
    compass->setLoc(QPoint(int(y() - 35) / 16, int(x()) / 16), 'b');
    return player;
}
