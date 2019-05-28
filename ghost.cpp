#include "ghost.h"
#include <QDebug>

Ghost::Ghost(Compass *compass_ipt): compass(compass_ipt) {
    setOffset(QPoint(-8, -8));
    tmpDir = QPoint(-1, -1);
    direction = Dir::Right;

    connect(compass, SIGNAL(powerUp()), this, SLOT(nerfInterval()));

    switchTimer = new QTimer();
    connect(switchTimer, SIGNAL(timeout()), this, SLOT(switchAnimate()));
    switchTimer->start(80);

    chaseTimer = new QTimer();
    connect(chaseTimer, SIGNAL(timeout()), this, SLOT(changeMode()));
    chaseTimer->start(20000);

    nerfTimer = new QTimer();
    connect(nerfTimer, SIGNAL(timeout()), this, SLOT(timeLeft()));

    mode = Mode::Chase;

    step_size = 1;

    for (int i = 0; i < 2; i ++) {
        for (int j = 0; j < 2; j ++) {
            fright[i][j].load(":/pic/ghost/nerf/nerf" + QString::number(i) + QString::number(j + 1) +".png");
            fright[i][j] = fright[i][j].scaledToHeight(32);
        }
    }

    index_j = 0;

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
        compass->setLoc(QPoint(int(y() - 35) / 16, int(x()) / 16), kind);
        if (tmpDir != QPoint(-1, -1)) {
            if (compass->canMove(pos(), tmpDir)) {
                direction = tmpDir;
                tmpDir = QPoint(-1, -1);
            }
        }
        if (compass->canMove(pos(), direction)) {
            setPos(pos() + direction * step_size);
        }
        else {
            QPoint arrow[4] = {Dir::Up, Dir::Down, Dir::Left, Dir::Right};
            for (int i = 0; i < 4; i ++) {
                if (compass->canMove(pos(), direction)) {
                    setDirection(arrow[i]);
                    break;
                }
            }
        }
    }
    else
        setPos(pos() + direction * step_size);
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
    if (mode == Mode::Scatter || mode == Mode::Chase) {
        if (direction == Dir::Up)
            index_i = 0;
        else if (direction == Dir::Down)
            index_i = 1;
        else if (direction == Dir::Left)
            index_i = 2;
        else
            index_i = 3;

        setPixmap(pic[index_i][index_j]);
    }
    else if (mode == Mode::Frighten) {
        int index_i = 1;
        setPixmap(fright[index_i][index_j]);
        if (remainNerf <= 5) {
            index_i ++;
            index_i %= 2;
        }
    }
    index_j ++;
    if (index_j >= 2)
        index_j = 0;
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
    else if (mode == Mode::Frighten)
        target = compass->getPlayerPos();

    if (compass->canMove(pos(), direction)) {
        QPoint arrow[4] = {Dir::Up, Dir::Down, Dir::Left, Dir::Right};
        qreal length;

        if (mode == Mode::Chase || mode == Mode::Scatter)
            length = 9999999;
        else
            length = -1;

        // test for moving
        for (int a = 0; a < 4; a ++) {
            if (arrow[a] + direction == QPoint(0, 0))
                continue;

            if (mode == Mode::Chase || mode == Mode::Scatter) {
                if (compass->canMove(pos() + direction * 16, arrow[a])) {
                    if (distance(target, QPoint(i + direction.y() + arrow[a].y(), j + direction.x() + arrow[a].x())) < length) {
                        length = distance(target, QPoint(i + direction.y() + arrow[a].y(), j + direction.x() + arrow[a].x()));
                        choice = arrow[a];
                    }
                }
            }
            else if (mode == Mode::Frighten) {
                if (compass->canMove(pos() + direction * 16, arrow[a])) {
                    if (distance(target, QPoint(i + direction.y() + arrow[a].y(), j + direction.x() + arrow[a].x())) > length) {
                        length = distance(target, QPoint(i + direction.y() + arrow[a].y(), j + direction.x() + arrow[a].x()));
                        choice = arrow[a];
                    }
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

void Ghost::nerfInterval() {
    chaseTimer->stop();
    prevMode = mode;
    mode = Mode::Frighten;
    remainNerf = 6;
    nerfTimer->start(1000);
}

void Ghost::timeLeft() {
    remainNerf --;
    if (!remainNerf) {
        nerfTimer->stop();
        chaseTimer->start();
        mode = prevMode;
    }
}

void Ghost::die() {

}

void Ghost::setKind(char ipt) {
    kind = ipt;
}
