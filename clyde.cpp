#include "clyde.h"

Clyde::Clyde(Compass *compass_ipt): Ghost (compass_ipt), compass(compass_ipt) {
    loadPicture(":/pic/ghost/clyde/");
    setCritical(QPoint(27, 1));
    setDirection(Dir::Left);
    setKind('c');
}

QPoint Clyde::setTarget() {
    QPoint target;
    QPoint pos(int(y() - 35) / 16, int(x()) / 16);
    QPoint player = compass->getPlayerPos();

    qreal dis = distance(pos, player);
    if (dis > 10)
        target = player;
    else
        // set to scatter point
        target = QPoint(27, 1);
    return target;
}
