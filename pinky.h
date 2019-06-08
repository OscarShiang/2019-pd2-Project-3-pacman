#ifndef PINKY_H
#define PINKY_H

#include "character.h"
#include "compass.h"
#include "ghost.h"
#include <QTimer>
#include <QObject>

class Pinky: public Ghost {
    Q_OBJECT
public:
    Pinky(Compass *compass_ipt);

    QPoint setTarget();
    void restore();

public slots:
    void sendOut();

private:
    Compass *compass;
};

#endif // PINKY_H
