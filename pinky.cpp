#include "pinky.h"
#include "blinky.h"
#include <QString>
#include <cmath>
#include <QDebug>

Pinky::Pinky(Compass *compass_ipt): Ghost(compass_ipt), compass(compass_ipt) {
    // set the origin point of the character
    setDirection(Dir::Left);
    loadPicture(":/pic/ghost/pinky/");
    setCritical(QPoint(1, 1));
    setKind('p');
}

QPoint Pinky::setTarget() {
    QPoint target = compass->getPlayerPos(), dire = compass->getPlayerDir();
    target += 4 * QPoint(dire.y(), dire.x());

    return target;
}
