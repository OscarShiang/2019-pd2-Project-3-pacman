#include "game.h"
#include "pacman.h"
#include "dot.h"
#include "pellet.h"
#include <QDebug>

Game::Game() {
    // set up the widow
    setFixedSize(width, height);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create the scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width, height);
    scene->setBackgroundBrush(QPixmap(":/pic/map/map.png").scaledToHeight(564));
    setScene(scene);

    // create the compass item
    compass = new Compass(this);
    connect(compass, SIGNAL(eat(QPoint)), this, SLOT(itemEat(QPoint)));

    mode = Mode::Menu;

    // create the shining timer for power pellets shine
    shine = new QTimer(this);

    // create the lag timer for anytime the game should pause a little
    lag = new QTimer(this);
    connect(lag, SIGNAL(timeout()), this, SLOT(countDown()));

    // create the pacman (the player)
    player = new Pacman(compass);
    scene->addItem(player);
    player->hide();

    // create the ghost
    blinky = new Blinky(compass);
    scene->addItem(blinky);
    blinky->setPos(32, 51);

    pinky = new Pinky(compass);
    scene->addItem(pinky);
    pinky->setPos(416, 51);

    inky = new Inky(compass);
    scene->addItem(inky);
    inky->setPos(16, 499);

    clyde = new Clyde(compass);
    scene->addItem(clyde);
    clyde->setPos(416, 499);

    // create the move timer
    pacmanMove = new QTimer(this);
    // player move
    connect(pacmanMove, SIGNAL(timeout()), player, SLOT(move()));

    ghostMove = new QTimer(this);

    // ghost move
    connect(ghostMove, SIGNAL(timeout()), blinky, SLOT(move()));
    connect(ghostMove, SIGNAL(timeout()), pinky, SLOT(move()));
    connect(ghostMove, SIGNAL(timeout()), inky, SLOT(move()));
    connect(ghostMove, SIGNAL(timeout()), clyde, SLOT(move()));

    connect(blinky, SIGNAL(fail()), this, SLOT(gameFail()));
    connect(pinky, SIGNAL(fail()), this, SLOT(gameFail()));
    connect(inky, SIGNAL(fail()), this, SLOT(gameFail()));
    connect(clyde, SIGNAL(fail()), this, SLOT(gameFail()));

    connect(blinky, SIGNAL(collide()), this, SLOT(ghostKill()));
    connect(pinky, SIGNAL(collide()), this, SLOT(ghostKill()));
    connect(inky, SIGNAL(collide()), this, SLOT(ghostKill()));
    connect(clyde, SIGNAL(collide()), this, SLOT(ghostKill()));

    // create the dashboard
    board = new Dashboard(this);
    scene->addItem(board);
    board->hide();

    title = new QGraphicsPixmapItem(QPixmap(":/pic/item/title.png").scaledToWidth(350, Qt::SmoothTransformation));
    title->setPos(width / 2 - title->boundingRect().width() / 2, 100);
    scene->addItem(title);

    // initialize the buttons
    play = new Button("play", 35);
    quit = new Button("quit", 35);
    scene->addItem(play);
    scene->addItem(quit);
    play->setPos(width / 2 - play->width() / 2, 280);
    quit->setPos(width / 2 - quit->width() / 2, 340);

    connect(play, SIGNAL(clicked()), this, SLOT(gameStart()));
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));

    // initialize the result panel
    result = new QGraphicsTextItem();
    score = new QGraphicsTextItem();
    result->setFont(QFont("Joystix", 50));
    result->setDefaultTextColor(Qt::white);
    scene->addItem(result);
    score->setFont(QFont("Joystix", 25));
    score->setDefaultTextColor(Qt::white);
    scene->addItem(score);

    // buttons
    again = new Button("again", 30);
    back = new Button("back", 30);
    scene->addItem(again);
    scene->addItem(back);

    connect(back, SIGNAL(clicked()), this, SLOT(displayMenu()));
    connect(again, SIGNAL(clicked()), this, SLOT(gameStart()));

    show();
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up)
        player->setDirection(Dir::Up);
    else if (event->key() == Qt::Key_Down)
        player->setDirection(Dir::Down);
    else if (event->key() == Qt::Key_Left)
        player->setDirection(Dir::Left);
    else if (event->key() == Qt::Key_Right)
        player->setDirection(Dir::Right);
    else if (event->key() == Qt::Key_Space) {
        gameClear();
    }
}

void Game::putDots() {
    QList <QPoint> list = compass->dots();
    foreach(QPoint pos, list) {
        item[pos.x()][pos.y()] = new Dot(this);
        item[pos.x()][pos.y()]->setPos(16 * pos.y(), 35 + 16 * pos.x());
        scene->addItem(item[pos.x()][pos.y()]);
        connect(item[pos.x()][pos.y()], SIGNAL(dotEaten()), this, SLOT(dotsAte()));
    }

    list = compass->pellets();
    foreach(QPoint pos, list) {
        item[pos.x()][pos.y()] = new Pellet(this);
        item[pos.x()][pos.y()]->setPos(16 * pos.y(), 35 + 16 * pos.x());
        scene->addItem(item[pos.x()][pos.y()]);
        connect(shine, SIGNAL(timeout()), item[pos.x()][pos.y()], SLOT(shine()));
        connect(item[pos.x()][pos.y()], SIGNAL(pelletEaten()), this, SLOT(pelletAte()));
    }

}

void Game::itemEat(QPoint pos) {
    if (mode == Mode::Play) {
        item[pos.x()][pos.y()]->eaten();
    }
}

void Game::pause() {
    pacmanMove->stop();
    ghostMove->stop();
    shine->stop();

    // turn off the timer on the character
    player->pause();
    blinky->pause();
    inky->pause();
    pinky->pause();
    clyde->pause();
}

void Game::resume() {
    pacmanMove->start();
    ghostMove->start();
    shine->start();

    // turn on all timers on the objects
    player->start();
    blinky->start();
    inky->start();
    pinky->start();
    clyde->start();
}

void Game::dotsAte() {
    board->addScore(10);
    remainDots --;
    qDebug() << remainDots;
    if (!remainDots)
        gameClear();
}

void Game::pelletAte() {
    board->addScore(50);
}

void Game::gameStart() {
    // change mode
    mode = Mode::Play;

    // put the dots and pellets on the map
    compass->initMap();
    putDots();

    // set timer start
    pacmanMove->start(20);
    ghostMove->start(11);
    shine->start(300);

    player->setPos(width / 2 - player->boundingRect().width() / 2 + 7, 403);

    remainDots = 240;

    board->reset();
    resume();

    // hide the items not used in playing mode
    menuPanel(false);
    resultPanel(false);

    // show the items
    playPanel(true);
}

void Game::gameClear() {
    // elements control
    pause();
    playPanel(false);
    resultPanel(true);
    clearDots();

    // set the text
    result->setPlainText("game clear");
    result->setPos(width / 2 - result->boundingRect().width() / 2, 80);

    score->setPlainText("Your score: " + QString::number(board->getScore()) + "\n\nHigh score: " + QString::number(board->getHighScore()));
    score->setPos(50, 230);

    again->setPos(width / 2 - 200, 350);
    back->setPos(width / 2 + 20, 350);
}

void Game::gameFail() {
    mode = Mode::Result;
    pause();
    playPanel(false);
    player->show();
    board->show();

    player->die();
    clearDots();
}

void Game::clearDots() {
    QList <QPoint> list = compass->remainDots();
    foreach(QPoint pos, list) {
        delete item[pos.x()][pos.y()];
    }
}

void Game::menuPanel(bool ipt) {
    if (ipt) {
        title->show();
        play->show();
        quit->show();
    }
    else {
        title->hide();
        play->hide();
        quit->hide();
    }
}

void Game::playPanel(bool ipt) {
    if (ipt) {
        // show the character
        player->show();
        blinky->show();
        inky->show();
        pinky->show();
        clyde->show();

        board->show();
    }
    else {
        player->hide();
        blinky->hide();
        inky->hide();
        pinky->hide();
        clyde->hide();

        board->hide();
    }
}

void Game::wait(qreal msec) {
    if (mode != Mode::Play)
        return;
    mode = Mode::Pause;
    lag->start(int(1000 * msec));
    pause();
    times = 1;
}

void Game::countDown() {
    times --;
    if (!times) {
        mode = Mode::Play;
        lag->stop();
        resume();
    }
}

void Game::ghostKill() {
    wait(0.5);
}

void Game::resultPanel(bool ipt) {
    if (ipt) {
        result->show();
        score->show();
        again->show();
        back->show();
    }
    else {
        result->hide();
        score->hide();
        again->hide();
        back->hide();
    }
}

void Game::displayMenu() {
    menuPanel(true);
    playPanel(false);
    resultPanel(false);
}
