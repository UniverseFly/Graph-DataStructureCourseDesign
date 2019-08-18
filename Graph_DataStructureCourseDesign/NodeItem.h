//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_NODEITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_NODEITEM_H


#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include "ArcItem.h"

struct NodeItem : QGraphicsObject {
    Q_OBJECT
private:
public:
    explicit NodeItem() {
        setFlag(ItemIsMovable);
        setFlag(ItemSendsGeometryChanges);
        setCacheMode(DeviceCoordinateCache);
        setZValue(-1);
    }

    QRectF boundingRect() const override {
        return {-10, -10, 20, 20};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::lightGray);
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));

        painter->drawEllipse(-10, -10, 20, 20);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == ItemPositionHasChanged) {
            emit nodePositionChanged(this);
        }

        return QGraphicsItem::itemChange(change, value);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        update();
        QGraphicsItem::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        update();
        QGraphicsItem::mouseReleaseEvent(event);
    }
signals:
    void nodePositionChanged(NodeItem *node);
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_NODEITEM_H
