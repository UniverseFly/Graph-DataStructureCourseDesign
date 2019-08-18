//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H


#include <QAbstractItemModel>
#include "Graph.h"
#include <string>
#include <QPoint>
#include <QVector>

#define test \
    addVertex("hello", {20, 20});\
    addVertex("world", {50, 25});\
    addVertex("me", {100, 20});\
    addVertex("wyx", {30, 70});\
    addArc(0, 1); \
    addArc(1, 2);

#define edgeWeight 1

struct GraphModel : QObject {
Q_OBJECT
private:
    Graph<std::string> graph;
private:
    // 保存每个顶点的位置信息
    QVector<QPoint> vertexPositions;
    QVector<QPair<int, int>> arcs;
public:
    GraphModel() {
        test
    }

    void addVertex(const std::string &value, const QPoint &point) {
        graph.appendVertex(value);
        vertexPositions.push_back(point);
    }

    void addArc(int startIndex, int endIndex) {
        graph.addArc_withIndex(startIndex, endIndex);
        arcs.push_back({startIndex, endIndex});
    }

    const QVector<QPoint> &getVertexPositions() const { return vertexPositions; }

    const QVector<QPair<int, int>> &getArcs() const { return arcs; }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
