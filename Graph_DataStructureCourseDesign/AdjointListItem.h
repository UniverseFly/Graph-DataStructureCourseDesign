//
// Created by 魏宇翔 on 2019-08-20.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTLISTITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTLISTITEM_H


#include <QGraphicsObject>
#include <QPainter>
#include <QPair>
#include <QVector>
#include "AdjointSingleListItem.h"

struct AdjointListItem : QGraphicsObject {
private:
    QVector<AdjointSingleListItem *> rows;
    const int space = 20;
    QPoint pointToStore = {0, 0};
public:
    explicit AdjointListItem(QGraphicsItem *parent = nullptr) : QGraphicsObject(parent) {
        setFlag(ItemIsMovable);
    }

    void resetFromRaw(const QVector<QPair<QVariant, QSet<int>>> &rawValue) {
        for (const auto &ptr : rows) { delete ptr; }
        rows = {};

        prepareGeometryChange();
        pointToStore = {0, 0};
        for (const auto &pair : rawValue) {
            auto row = new AdjointSingleListItem(this);
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


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTLISTITEM_H
