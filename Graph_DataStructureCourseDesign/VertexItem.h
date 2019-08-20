//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_VERTEXITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_VERTEXITEM_H


#include <QGraphicsItem>
#include <QPainter>
#include <QList>
#include <QPen>
#include "ArcItem.h"

struct VertexItem : QGraphicsObject {
Q_OBJECT
private:
    QString text;
private:
    QRect bound = {-20, -20, 40, 40};
public:
    explicit VertexItem(int radius, const QString &text, bool movable = true, QGraphicsItem *parent = nullptr) :
            text(text), QGraphicsObject(parent) {
        if (movable) {
            setFlag(ItemIsMovable);
            setFlag(ItemSendsGeometryChanges);
        }
        setCacheMode(DeviceCoordinateCache);
        setZValue(int(-1));
        setScale(radius / 20.0);
    }

    QRectF boundingRect() const override {
        double radius = bound.width() / 2.0;
        radius += 3;
        return {-radius, -radius, 2 * radius, 2 * radius};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::lightGray);
        painter->setPen(QPen(Qt::black, 3.3, Qt::SolidLine));
        QFont font;
        font.setPointSize(15);
        painter->setFont(font);

        painter->drawEllipse(bound);
        painter->drawText(bound, Qt::AlignCenter, text);
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

    void nodePositionChanged(VertexItem *node);
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_VERTEXITEM_H
