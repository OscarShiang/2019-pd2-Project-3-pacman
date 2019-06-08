#ifndef BLINKY_H
#define BLINKY_H

#include "character.h"
#include "ghost.h"
#include "compass.h"
#include <QTimer>
#include <QObject>

class Blinky: public Ghost {
    Q_OBJECT
public:
    Blinky(Compass *compass_ipt);
    QPoint setTarget();
    void sendOut();

private:
    Compass *compass;
};

#endif // BLINKY_H
