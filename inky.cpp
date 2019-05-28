#include "inky.h"
#include <QDebug>

Inky::Inky(Compass *compass_ipt): Ghost (compass_ipt), compass(compass_ipt) {
    loadPicture(":/pic/ghost/inky/");
    setDirection(Dir::Right);
    setCritical(QPoint(27, 27));
    setKind('i');
}

QPoint Inky::setTarget() {
    QPoint blinky = compass->getBlinkyPos();
    QPoint pacman = compass->getPlayerPos();
    QPoint dire = compass->getPlayerDir();

    QPoint target = blinky + 2 * (pacman + 2 * QPoint(dire.y(), dire.x()) - blinky);
    if (target.x() < 0 || target.x() > 30 || target.y() < 0 || target.y() > 27) {
        target = compass->getPlayerPos();
    }
    return target;
}
