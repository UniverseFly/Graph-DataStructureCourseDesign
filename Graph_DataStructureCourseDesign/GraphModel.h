//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H


#include <QAbstractItemModel>
#include "Graph.h"
#include "GraphAlgorithms.h"
#include <string>
#include <QPoint>
#include <QVector>

#define TEST_CASE \
    addVertex("hello", {20, 20});\
    addVertex("world", {50, 25});\
    addVertex("me", {100, 20});\
    addVertex("wyx", {30, 70});\
    addArc(0, 1); \
    addArc(1, 2);

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
        TEST_CASE
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

    void setVertexPosition(int index, const QPoint &position) {
        vertexPositions[index] = position;
    }

    QVector<int> deepFirstSearch_nonRecursive() {
        return QVector<int>::fromStdVector(deepFirstSearch_recursive_withIndex(graph, 0).indexOrder);
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
