#ifndef INKY_H
#define INKY_H

#include <QObject>
#include "ghost.h"
#include "compass.h"

class Inky: public Ghost {
    Q_OBJECT
public:
    Inky(Compass *compass_ipt);

    QPoint setTarget();

private:
    Compass *compass;
};

#endif // INKY_H
