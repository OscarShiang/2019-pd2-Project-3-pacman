#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Character: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    virtual void setDirection(QPoint dir) = 0;

public slots:
    virtual void move() = 0;

private:

};

namespace Dir {
    const QPoint Up = QPoint(0, -1);
    const QPoint Down = QPoint(0, 1);
    const QPoint Left = QPoint(-1, 0);
    const QPoint Right = QPoint(1, 0);
}

#endif // CHARACTER_H
