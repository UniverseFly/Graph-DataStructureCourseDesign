//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHVIEW_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHVIEW_H


#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include "ArcItem.h"
#include "NodeItem.h"

struct GraphView : QGraphicsView {
    Q_OBJECT
private:
    GraphModel model;
    QList<ArcItem *> arcs;
    QList<NodeItem *> nodes;
public:
    explicit GraphView(QWidget *parent = nullptr) {
        auto scene = new QGraphicsScene(this);
        scene->setSceneRect(-200, -200, 400, 400);
        setScene(scene);

        for (const auto &position : model.getVertexPositions()) {
            auto node = new NodeItem;
            node->setPos(position);
            nodes.push_back(node);
            scene->addItem(node);
        }
        for (const auto &arc : model.getArcs()) {
            const auto &startPosition = model.getVertexPositions()[arc.first];
            const auto &endPosition = model.getVertexPositions()[arc.second];
            auto newArc = new ArcItem(startPosition, endPosition);
            arcs.push_back(newArc);
            scene->addItem(newArc);
        }
    }

    void updateItem(NodeItem *node) {

    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHVIEW_H
