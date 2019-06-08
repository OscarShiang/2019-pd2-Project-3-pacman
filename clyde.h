#ifndef CLYDE_H
#define CLYDE_H

#include "ghost.h"
#include "compass.h"
#include <QObject>
#include <QTimer>

class Clyde: public Ghost {
    Q_OBJECT
public:
    Clyde(Compass *compass_ipt);
    QPoint setTarget();

    void restore();

public slots:
    void sendOut();

private:
    Compass *compass;
};

#endif // CLYDE_H
