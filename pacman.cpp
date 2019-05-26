#include "pacman.h"
#include <QDebug>

Pacman::Pacman(Compass *compass_ipt) {
    // set the origin to fit the coordinate
    setOffset(QPoint(-7, -7));
    direction = Dir::Right;
    tmpDir = direction;

    // load the picture in
    for (int i = 0; i < 3; i ++) {
        pic[i].load("://pic/pacman/" + QString::number(i + 1) + ".png");
        pic[i] = pic[i].scaledToHeight(30);
    }
    setPixmap(pic[0]);
    setTransformOriginPoint(boundingRect().width() / 2 - 7, boundingRect().height() / 2 - 7);

    // set the timer for moving
    switchTimer = new QTimer();
    connect(switchTimer, SIGNAL(timeout()), this, SLOT(switchAnimate()));
    switchTimer->start(70);
    switchTimer->stop();

    // create the compass object
    compass = compass_ipt;

}

void Pacman::move() {
    // handling bounding
    if (y() == 259. && (x() < 0 || x() >= 448)) {
        if (tmpDir == Dir::Left || tmpDir == Dir::Right)
            direction = tmpDir;
        if (direction == Dir::Left)
            setRotation(180);
        else if (direction == Dir::Right)
            setRotation(0);
        setPos(pos() + direction * 2);

        if (x() < -30)
            setX(478);
        else if (x() > 478)
            setX(-30);
    }
    // handling normal move
    else if (int(y() - 35) % 16 == 0 && int(x()) % 16 == 0) {
        if (tmpDir != direction && compass->canMove(pos(), tmpDir)) {
            direction = tmpDir;
            if (direction == Dir::Up)
                setRotation(270);
            else if (direction == Dir::Down)
                setRotation(90);
            else if (direction == Dir::Left)
                setRotation(180);
            else if (direction == Dir::Right)
                setRotation(0);
        }
        else if (compass->canMove(pos(), direction)) {
            compass->check(pos());
            setPos(pos() + direction * 2);
            switchTimer->start();
        }
        else {
            compass->check(pos());
            switchTimer->stop();
            setPixmap(pic[1]);
        }
    }
    else {
        setPos(pos() + direction * 2);
    }

}

void Pacman::setDirection(QPoint dir) {
    tmpDir = dir;
}

void Pacman::switchAnimate() {
    static int i = 0;
    setPixmap(pic[i]);
    i ++;
    if (i >= 3)
        i = 0;
}
