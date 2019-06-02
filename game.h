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
    void clearDots();
    void pause();
    void resume();

    void wait(qreal msec);

    // panel control
    void menuPanel(bool ipt);
    void playPanel(bool ipt);
    void resultPanel(bool ipt);

    // mode control
    void displayMenu();
    void gameClear();

    Dashboard *board;

public slots:
    void itemEat(QPoint pos);
    void dotsAte();
    void pelletAte();
    void countDown();

    // end game control
    void gameStart();
    void gameFail();

private:
    const int width = 448;
    const int height = 564;

    int mode;
    int remainDots;
    int times;

    QGraphicsScene *scene;
    Item *item[31][28];

    // title
    QGraphicsPixmapItem *title;
    Button *play, *quit;

    // buttons
    Button *test;

    // for game playing
    Character *player, *blinky, *pinky, *inky, *clyde;
    Compass *compass;
    QTimer *pacmanMove, *shine, *ghostMove, *lag;
};

enum Mode {
    Menu, Pause, Play, Result
};

#endif // GAME_H
