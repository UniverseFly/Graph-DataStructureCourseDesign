//
// Created by 魏宇翔 on 2019-08-20.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTSINGLELISTITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTSINGLELISTITEM_H


#include <QGraphicsObject>
#include <QPainter>
#include <QSet>
#include "VertexItem.h"
#include "ArcItem.h"
#include "AdjointNodeItem.h"

struct AdjointSingleListItem : QGraphicsObject {
Q_OBJECT
private:
    QPoint pointToStore = {100, 0};
    const int space = 40;
    VertexItem *vertex = nullptr;
    QVector<AdjointNodeItem *> adjNodes;
    QVector<ArcItem *> arrows;
public:
    explicit AdjointSingleListItem(QGraphicsItem *parent = nullptr) : QGraphicsObject(parent) {}

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override {}

    QRectF boundingRect() const override {
        return {-30.0, -30.0, pointToStore.x() + 30.0, 60};
    }

    // 提供顶点信息以及邻接边的集合来绘制图形
    void resetFromRaw(const QVariant &vertexText, const QSet<int> &nodeTexts) {
        delete vertex;
        for (const auto &ptr : adjNodes) { delete ptr; }
        for (const auto &ptr : arrows) { delete ptr; }
        adjNodes = {};
        arrows = {};

        vertex = new VertexItem(20, vertexText.toString(), false, this);
        vertex->setPos(0, 0);

        // 画顶点开始的箭头，如果没有就不画
        if (!nodeTexts.isEmpty()) {
            arrows.push_back(new ArcItem(vertex->pos().toPoint() + QPoint{20, 0},
                             QPoint{70, 0}, this));
        }

        pointToStore = {110, 0};
        for (auto iter = nodeTexts.begin(); iter != nodeTexts.end();) {
            auto node = new AdjointNodeItem(QVariant(*iter).toString(), this);
            node->setPos(pointToStore);
            pointToStore += {int(node->scale() * node->boundingRect().width() + space), 0};
            adjNodes.push_back(node);

            // 邻接顶点之间的箭头
            ++iter;
            if (iter != nodeTexts.end()) {
                arrows.push_back(new ArcItem(node->pos().toPoint() + QPoint{25, 0},
                                             pointToStore - QPoint{40, 0}, this));
            }
        }
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_ADJOINTSINGLELISTITEM_H
