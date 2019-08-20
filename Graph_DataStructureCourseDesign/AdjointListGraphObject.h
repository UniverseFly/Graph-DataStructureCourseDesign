//
// Created by 魏宇翔 on 2019-08-20.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTLISTGRAPHOBJECT_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTLISTGRAPHOBJECT_H


#include <QGraphicsObject>
#include <QPainter>
#include <QPair>
#include <QVector>
#include "AdjointListObject.h"

struct AdjointListGraphObject : QGraphicsObject {
private:
    QVector<AdjointListObject *> rows;
    const int space = 20;
    QPoint pointToStore = {0, 0};
public:
    explicit AdjointListGraphObject(QGraphicsItem *parent = nullptr) : QGraphicsObject(parent) {
        setFlag(ItemIsMovable);
        setCacheMode(DeviceCoordinateCache);
    }

    void resetFromRaw(const QVector<QPair<QVariant, QSet<int>>> &rawValue) {
        for (const auto &ptr : rows) { delete ptr; }
        rows = {};

        pointToStore = {0, 0};
        for (const auto &pair : rawValue) {
            auto row = new AdjointListObject(this);
            row->resetFromRaw(pair.first, pair.second);
            row->setPos(pointToStore);
            rows.push_back(row);

            pointToStore += {0, int(space + row->boundingRect().height())};
        }
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        painter->setPen(QPen(Qt::lightGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(QBrush(QColor(180, 233, 255, 120)));
        painter->drawRect(boundingRect());

        QFont font;
        font.setPointSize(15);
        painter->setPen(Qt::black);
        painter->setFont(font);
        painter->drawText(-20.0, -44.0, "邻接表视图");
    }

    QRectF boundingRect() const override {
        double width = 80.0;
        for (const auto &row : rows) {
            qreal rowWidth = row->boundingRect().width();
            if (rowWidth > width) {
                width = rowWidth;
            }
        }
        return {-40.0, -70.0, width + 40.0, rows.count() * (60.0 + space) + 50.0};
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTLISTGRAPHOBJECT_H
