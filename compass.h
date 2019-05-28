#ifndef COMPASS_H
#define COMPASS_H

#include <QObject>
#include <QGraphicsItem>

class Compass: public QObject {
    Q_OBJECT
public:
    Compass(QObject *parent = 0);

    void initMap();
    void check(QPointF pos, QPoint dir);

    bool canMove(QPointF pos, QPoint direction);
    QList <QPoint> dots();
    QList <QPoint> pellets();

    void setLoc(QPoint pos, char charcter);
    QPoint getPlayerPos();
    QPoint getPlayerDir();
    QPoint getBlinkyPos();

signals:
    void eat(QPoint pos);

private:
    QList <QList <int> > map;
    QPoint player, blinky;
    QPoint dir_player;
};


#endif // COMPASS_H
