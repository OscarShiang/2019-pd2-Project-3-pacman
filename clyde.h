#ifndef CLYDE_H
#define CLYDE_H

#include "ghost.h"
#include "compass.h"
#include <QObject>

class Clyde: public Ghost {
    Q_OBJECT
public:
    Clyde(Compass *compass_ipt);
    QPoint setTarget();

private:
    Compass *compass;
};

#endif // CLYDE_H
