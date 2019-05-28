#ifndef GHOST_H
#define GHOST_H

#include "character.h"
#include <QObject>
#include <QTimer>
#include "compass.h"

class Ghost: public Character {
    Q_OBJECT
public:
    Ghost(Compass *compass);

    void move();
    void setDirection(QPoint dir);
    void chase();

    virtual QPoint setTarget() = 0;

    void loadPicture(QString filepath);

    void setCritical(QPoint pt);

    QPoint getDirection();
    QPoint getTmpDirection();

    static qreal distance(QPoint a, QPoint b);

public slots:
    void switchAnimate();
    void changeMode();

private:
    QPoint direction;
    QPoint tmpDir;
    QPoint target;
    QPoint critical;
    QPixmap pic[4][2];
    QPixmap fright[2][2];
    QTimer *switchTimer, *chaseTimer;
    Compass *compass;
    bool nerf;
    int index;
    int mode;

    enum Mode {
        Scatter,Frighten, Chase
    };
};

#endif // GHOST_H
