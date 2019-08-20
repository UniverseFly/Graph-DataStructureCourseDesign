//
// Created by 魏宇翔 on 2019-08-20.
//

#include "ArcItem.h"
#include "VertexItem.h"
#include <cassert>

void ArcItem::setVertices(VertexItem *source, VertexItem *destination) {
    this->source = source;
    this->destination = destination;
}

void ArcItem::adjustFromVertices() {
    assert(source != nullptr && destination != nullptr);

    const auto &startPosition = source->pos().toPoint();
    const auto &endPosition = destination->pos().toPoint();

    QLineF lineF(startPosition, endPosition);
    QPointF offset(source->radius * lineF.dx() / lineF.length(), source->radius * lineF.dy() / lineF.length());

    auto newStart = startPosition + QPoint(offset.x(), offset.y());
    auto newEnd = endPosition - QPoint(offset.x(), offset.y());

    // important to add this method!!!
    prepareGeometryChange();
    setStart(newStart);
    setEnd(newEnd);
}

void ArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QLineF line(start, end);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

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
