#include "button.h"
#include <QFont>

Button::Button(QString text_ipt, int font_size_ipt): font_size(font_size_ipt) {
    text = new QGraphicsTextItem(text_ipt, this);
    text->setFont(QFont(font_family, font_size));
    icon = new QGraphicsPixmapItem(QPixmap(":/pic/pacman/2.png").scaledToHeight(text->boundingRect().height() - 5), this);

    text->setDefaultTextColor(Qt::white);
    text->setPos(icon->boundingRect().width() - 2, 0);

    icon->setY(text->boundingRect().height() / 2 - icon->boundingRect().height() / 2);
    icon->hide();
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    icon->show();
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    icon->hide();
}

qreal Button::width() {
    return text->boundingRect().width() + icon->boundingRect().width() + 10;
}

qreal Button::height() {
    return text->boundingRect().height();
}
