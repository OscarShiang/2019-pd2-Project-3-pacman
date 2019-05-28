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

}

QPoint Pinky::setTarget() {
    QPoint target = compass->getPlayerPos(), dire = compass->getPlayerDir();
    target += 4 * QPoint(dire.y(), dire.x());

    if (target.x() < 0 || target.x() > 30 || target.y() < 0 || target.y() > 27) {
        target = compass->getPlayerPos();
    }
    return target;
}
