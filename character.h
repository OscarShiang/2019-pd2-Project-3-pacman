#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <cmath>

class Character: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    virtual void setDirection(QPoint dir) = 0;
    static qreal distance(QPoint a, QPoint b) {
        QPoint vector = b - a;
        return sqrt(pow(vector.x(), 2) + pow(vector.y(), 2));
    }

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
