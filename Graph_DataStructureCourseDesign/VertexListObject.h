//
// Created by 魏宇翔 on 2019-08-19.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHICSLISTOBJECT_
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHICSLISTOBJECT_


#include <QGraphicsView>
#include <QVector>
#include <QGraphicsObject>
#include <QGraphicsWidget>
#include <sstream>
#include <QDebug>
#include "VertexItem.h"


struct VertexListObject : QGraphicsObject {
Q_OBJECT
Q_PROPERTY(QVariantList raw WRITE resetToRaw)
private:
    QVector<QGraphicsObject *> items;
    QPoint storePoint = {0, 0};
    int space = 5;
private:
    const int radius = 15;
    QString text;
public:
    explicit VertexListObject(const QString &text, const QVariantList &rawValues = {},
                              QGraphicsItem *parent = nullptr) : text(text), QGraphicsObject(parent) {
        setFlag(ItemIsMovable);
    }

    void append(const QVariant &value) {
        auto item = new VertexItem(radius, value.toString(), false, this);
        items.push_back(item);
        item->setPos(storePoint);
        storePoint += {int(item->scale() * item->boundingRect().width() + space), 0};
    }

    void resetToRaw(const QVariantList &rawValues) {
        prepareGeometryChange();
        for (const auto &item : items) { delete item; }
        items = {};
        storePoint = {0, 0};
        for (const auto &value : rawValues) { append(value); }
    }

    const QVector<QGraphicsObject *> &getSubObjects() const { return items; }

    QRectF boundingRect() const override {
        double adjustedRadius = radius + 7.0;
        double deltaHeight = 40.0;
        double width = 110.0;
        if (storePoint.x() + 20.0 > width) { width = storePoint.x() + 20.0; }
        return {-adjustedRadius, -adjustedRadius - deltaHeight,
                width, 2 * adjustedRadius + deltaHeight};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        painter->setPen(QPen(Qt::lightGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(QBrush(QColor(180, 233, 255, 120)));
        painter->drawRect(boundingRect());

        QFont font;
        font.setPointSize(15);
        painter->setPen(Qt::black);
        painter->setFont(font);
        painter->drawText(-radius, -35.0, text);
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHICSLISTOBJECT_
