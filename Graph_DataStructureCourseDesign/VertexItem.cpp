//
// Created by 魏宇翔 on 2019-08-20.
//

#include "ArcItem.h"
#include "VertexItem.h"

QVariant VertexItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        for (const auto &arc : arcs) {
            arc->adjustFromVertices();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}
