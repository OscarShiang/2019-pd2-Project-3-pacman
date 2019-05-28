#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include "character.h"
#include "compass.h"
#include "item.h"
#include "dashboard.h"
#include "ghost.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"
#include "button.h"

class Game: public QGraphicsView {
    Q_OBJECT
public:
    Game();

    void keyPressEvent(QKeyEvent *event);
    void putDots();
    void pause();
    void resume();

    void displayMenu();
    void gameClear();

    Dashboard *board;

public slots:
    void itemEat(QPoint pos);
    void dotsAte();
    void pelletAte();

    void gameStart();

private:
    const int width = 448;
    const int height = 564;

    int mode;
    int remainDots;

    QGraphicsScene *scene;
    Item *item[31][28];

    // title
    QGraphicsPixmapItem *title;
    Button *play, *quit;

    // buttons
    Button *test;

    Character *player, *blinky, *pinky, *inky, *clyde;
    Compass *compass;
    QTimer *mv, *shine;
};

enum Mode {
    Menu, Pause, Play
};

#endif // GAME_H
