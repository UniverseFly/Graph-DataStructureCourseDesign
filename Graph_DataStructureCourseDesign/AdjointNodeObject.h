//
// Created by 魏宇翔 on 2019-08-20.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTNODEOBJECT_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTNODEOBJECT_H


#include <QGraphicsObject>
#include <QPainter>

struct AdjointNodeObject : QGraphicsObject {
Q_OBJECT
private:
    QString text;
public:
    explicit AdjointNodeObject(const QVariant &text, QGraphicsItem *parent = nullptr) :
            text(text.toString()), QGraphicsObject(parent) {}

    QRectF boundingRect() const override {
        return {-40, -17, 80, 34};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        painter->setPen(QPen(Qt::black, 2.3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(QColor(245, 245, 245));
        painter->drawRect(boundingRect());
        painter->drawLine(10, 17, 10, -17);
        painter->drawText(QRect(-40, -17, 50, 34), Qt::AlignCenter, text);
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTNODEOBJECT_H
