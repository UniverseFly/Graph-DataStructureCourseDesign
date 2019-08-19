//
// Created by 魏宇翔 on 2019-08-19.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHICSLISTOBJECT_
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHICSLISTOBJECT_


#include <QGraphicsView>
#include <QVector>
#include <QGraphicsObject>
#include <sstream>
#include <QDebug>
#include "NodeItem.h"

struct GraphicsListObject : QGraphicsObject {
#ifdef DEBUG

    FRIEND_MAIN

#endif
    Q_PROPERTY(QVariantList raw
                       WRITE
                       resetToRaw)
private:
    QVector<QGraphicsObject *> items;
    QPoint storePoint = {0, 0};
    int space = 5;
public:
    explicit GraphicsListObject(const QVariantList &rawValues = {}, QGraphicsItem *parent = nullptr) :
            QGraphicsObject(parent) {
        resetToRaw(rawValues);
    }

    void append(const QVariant &value) {
        auto item = new NodeItem(value.toString(), false, this);
        items.push_back(item);
        item->setPos(storePoint);
        storePoint += {int(item->boundingRect().width() + space), 0};
    }

    void resetToRaw(const QVariantList &rawValues) {
        for (const auto &item : items) { delete item; }
        items = {};
        storePoint = {0, 0};
        for (const auto &value : rawValues) { append(value); }
    }

    const QVector<QGraphicsObject *> &getSubObjects() const { return items; }

    QRectF boundingRect() const override { return {}; }

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override {}
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHICSLISTOBJECT_
