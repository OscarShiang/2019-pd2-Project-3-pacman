#include "ghost.h"
#include <QDebug>

Ghost::Ghost(Compass *compass_ipt): compass(compass_ipt) {
    setOffset(QPoint(-8, -8));
    tmpDir = QPoint(-1, -1);
    direction = Dir::Right;

    switchTimer = new QTimer();
    connect(switchTimer, SIGNAL(timeout()), this, SLOT(switchAnimate()));
    switchTimer->start(80);

    chaseTimer = new QTimer();
    connect(chaseTimer, SIGNAL(timeout()), this, SLOT(changeMode()));
    chaseTimer->start(20000);

    mode = Mode::Chase;

    for (int i = 0; i < 2; i ++) {
        for (int j = 0; j < 2; j ++) {
            fright[i][j].load(":/pic/ghost/nerf/nerf" + QString::number(i) + QString::number(j + 1) +".png");
            fright[i][j] = fright[i][j].scaledToHeight(32);
        }
    }

    index = 0;

    nerf = false;
}

void Ghost::move() {
    if (y() == 259. && (x() < 0 || x() >= 448)) {
        if (tmpDir == Dir::Left || tmpDir == Dir::Right)
            direction = tmpDir;
        setPos(pos() + direction * 2);

        if (x() < -30)
            setX(478);
        else if (x() > 478)
            setX(-30);
    }
    else if (int(y() - 35) % 16 == 0 && int(x()) % 16 == 0) {
        chase();
        if (tmpDir != QPoint(-1, -1)) {
            if (compass->canMove(pos(), tmpDir)) {
                direction = tmpDir;
                tmpDir = QPoint(-1, -1);
            }
        }
        if (compass->canMove(pos(), direction)) {
            setPos(pos() + direction * 2);
        }
        else {
            setDirection(Dir::Up);
        }
    }
    else
        setPos(pos() + direction * 2);
}

void Ghost::setDirection(QPoint dir) {
    tmpDir = dir;
}

qreal Ghost::distance(QPoint a, QPoint b) {
    QPoint vector = b - a;
    return sqrt(pow(vector.x(), 2) + pow(vector.y(), 2));
}

void Ghost::loadPicture(QString filepath) {
    QString dire = "udlr";
    for (int i = 0; i < 4; i ++) {
        for (int j = 0; j < 2; j ++) {
            pic[i][j].load(filepath + dire[i] + QString::number(j + 1) + ".png");
            pic[i][j] = pic[i][j].scaledToHeight(32);
        }
    }
    setPixmap(pic[0][0]);
}

void Ghost::switchAnimate() {
    int i;
    if (direction == Dir::Up)
        i = 0;
    else if (direction == Dir::Down)
        i = 1;
    else if (direction == Dir::Left)
        i = 2;
    else
        i = 3;

    setPixmap(pic[i][index]);
    index ++;
    if (index >= 2)
        index = 0;
}

QPoint Ghost::getDirection() {
    return direction;
}

QPoint Ghost::getTmpDirection() {
    return tmpDir;
}

void Ghost::chase() {
    if (tmpDir != QPoint(-1, -1))
        return;

    int i = int(y() - 35) / 16;
    int j = int(x()) / 16;
    QPoint choice = QPoint(-1, -1);

    if (mode == Mode::Chase)
        target = setTarget();
    else if (mode == Mode::Scatter)
        target = critical;

    if (compass->canMove(pos(), direction)) {
        QPoint arrow[4] = {Dir::Up, Dir::Down, Dir::Left, Dir::Right};
        qreal length = 999999;

        // test for moving
        for (int a = 0; a < 4; a ++) {
            if (arrow[a] + direction == QPoint(0, 0))
                continue;

            if (compass->canMove(pos() + direction * 16, arrow[a])) {
                if (distance(target, QPoint(i + direction.y() + arrow[a].y(), j + direction.x() + arrow[a].x())) < length) {
                    length = distance(target, QPoint(i + direction.y() + arrow[a].y(), j + direction.x() + arrow[a].x()));
                    choice = arrow[a];
                }
            }
        }
    }
    if (choice != QPoint(-1, -1)) {
        setDirection(choice);
    }
}

void Ghost::setCritical(QPoint pt) {
    critical = pt;
}

void Ghost::changeMode() {
    if (mode == Mode::Chase) {
        chaseTimer->setInterval(10000);
        mode = Mode::Scatter;
//        qDebug() << "start scatter";
    }
    else if (mode == Mode::Scatter) {
        chaseTimer->setInterval(20000);
        mode = Mode::Chase;
//        qDebug() << "start chase";
    }
}
