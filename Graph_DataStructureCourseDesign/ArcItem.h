//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H


#include <QGraphicsItem>
#include <QPainter>
#include <cmath>

struct ArcItem : QGraphicsItem {
private:
    QPoint start;
    QPoint end;
public:
    ArcItem(const QPoint &start, const QPoint &end, QGraphicsItem *item = nullptr) :
            start(start), end(end), QGraphicsItem(item) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        QLine line(start, end);

        // Draw the arrows
        double angle = std::atan2(-line.dy(), line.dx());

        int arrowSize = 10;
        QPointF destArrowP1 = end + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                            cos(angle - M_PI / 3) * arrowSize);
        QPointF destArrowP2 = end + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                            cos(angle - M_PI + M_PI / 3) * arrowSize);

        painter->setPen(QPen(Qt::black, 2.3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(line);

        painter->setBrush(Qt::black);
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }

    QRectF boundingRect() const override { return {start, end}; }

    const QPoint &getEnd() const { return end; }

    void setStart(const QPoint &start) {
        prepareGeometryChange();
        ArcItem::start = start;
    }

    void setEnd(const QPoint &end) {
        prepareGeometryChange();
        ArcItem::end = end;
    }

    const QPoint &getStart() const {
        return start;
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H
