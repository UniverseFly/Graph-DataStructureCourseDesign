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
#include <QPushButton>
#include <sstream>
#include <QLineEdit>
#include <QMessageBox>
#include "VertexItem.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include <QState>
#include <QStateMachine>
#include <QTextEdit>
#include <QLineEdit>
#include <QSet>
#include "GraphModel.h"
#include "ArcItem.h"

struct GraphObject : QGraphicsObject {
Q_OBJECT
private:
    QVector<VertexItem *> nodes;
    QSet<QPair<int, int>> arcs;
public:
    static constexpr int vertexRadius = 20;
public:
    explicit GraphObject(QGraphicsItem *parent = nullptr) : QGraphicsObject(parent) {}

    const QVector<VertexItem *> &getVertices() const { return nodes; }

    const QSet<QPair<int, int>> &getArcs() const { return arcs; }

    void addVertex() {
        QVariant text = nodes.size();
        auto node = new VertexItem(vertexRadius, text.toString(), true, this);
        node->setPos(0, 0);
        nodes.push_back(node);
    }

    void addArc(int start, int end) {
        // 不需要起点和终点，通过节点初始化
        auto newArc = new ArcItem({}, {}, this);
        arcs.insert({start, end});

        const auto &source = nodes[start];
        const auto &destination = nodes[end];

        newArc->setVertices(source, destination);
        source->arcs.push_back(newArc);
        destination->arcs.push_back(newArc);
    }

    QRectF boundingRect() const override { return {}; }

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override {}

private:
    int findNodeIndex(VertexItem *nodeToFind) const {
        int index = 0;
        for (const auto &node : nodes) {
            if (node == nodeToFind) { return index; }
            index += 1;
        }
        return -1;
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHVIEW_H
