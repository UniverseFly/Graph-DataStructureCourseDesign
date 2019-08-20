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
#include <deque>
#include "GraphModel.h"

struct GraphObject : QGraphicsObject {
Q_OBJECT
private:
    QVector<VertexItem *> nodes;
    QMap<QPair<int, int>, ArcItem *> arcs;
public:
    static constexpr int vertexRadius = 20;
public:
    explicit GraphObject(QGraphicsItem *parent = nullptr) : QGraphicsObject(parent) {}

    const QVector<VertexItem *> &getVertices() const { return nodes; }

    QVector<QPair<int, int>> getArcs() const { return arcs.keys().toVector(); }

    void addVertex() {
        QVariant text = nodes.size();
        auto node = new VertexItem(vertexRadius, text.toString(), true, this);
        QObject::connect(node, &VertexItem::nodePositionChanged, this, &GraphObject::updateItem);
        node->setPos(0, 0);
        nodes.push_back(node);
    }

    void addArc(int start, int end) {
        if (start < 0 || start >= nodes.size() || end < 0 || end >= nodes.size()) { return; }
        const auto &startPosition = nodes[start]->pos().toPoint();
        const auto &endPosition = nodes[end]->pos().toPoint();
        QLineF lineF(startPosition, endPosition);
        QPointF offset(vertexRadius * lineF.dx() / lineF.length(), vertexRadius * lineF.dy() / lineF.length());

        auto newArc = new ArcItem(startPosition + QPoint(offset.x(), offset.y()),
                                  endPosition - QPoint(offset.x(), offset.y()), this);
        arcs.insert({start, end}, newArc);
    }

    QRectF boundingRect() const override { return {}; }

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override {}

private:
    void updateItem(VertexItem *nodeToUpdate) {
        const auto &newPosition = QPoint(nodeToUpdate->pos().x(), nodeToUpdate->pos().y());
        int index = findNodeIndex(nodeToUpdate);
        if (index == -1) { return; }

        auto iter = arcs.cbegin();
        while (iter != arcs.cend()) {
            const auto &startPosition = nodes[iter.key().first]->pos().toPoint();
            const auto &endPosition = nodes[iter.key().second]->pos().toPoint();

            QLineF lineF(startPosition, endPosition);
            QPointF offset(vertexRadius * lineF.dx() / lineF.length(), vertexRadius * lineF.dy() / lineF.length());

            auto newStart = startPosition + QPoint(offset.x(), offset.y());
            auto newEnd = endPosition - QPoint(offset.x(), offset.y());
            if (iter.key().first == index || iter.key().second == index) {
                iter.value()->setStart(newStart);
                iter.value()->setEnd(newEnd);
            }
            ++iter;
        }
    }

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
