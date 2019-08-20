//
// Created by 魏宇翔 on 2019-08-20.
//

#include "VertexItem.h"
#include "GraphObject.h"


QVariant VertexItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        graph->updateItem(this);
    }

    return QGraphicsItem::itemChange(change, value);
}

void VertexItem::setGraph(GraphObject *graph) {
    this->graph = graph;
}
