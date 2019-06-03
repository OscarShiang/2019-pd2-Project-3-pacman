#include <QApplication>
#include "game.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Game *game = new Game();
    game->displayMenu();
    game->show();

    return a.exec();
}
