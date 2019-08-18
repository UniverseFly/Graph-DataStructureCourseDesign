//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H


#include <QGraphicsItem>
#include <QPainter>

struct ArcItem : QGraphicsObject {
Q_OBJECT
private:
    QPoint start;
    QPoint end;
public:
    ArcItem(const QPoint &start, const QPoint &end, QGraphicsItem *item = nullptr) :
            start(start), end(end), QGraphicsObject(item) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        QLine line(start, end);

        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(line);
    }

    QRectF boundingRect() const override {
        return {start, end};
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H
