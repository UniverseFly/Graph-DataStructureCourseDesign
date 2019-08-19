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
#include <deque>
#include <QDebug>
#include <QVariantList>

struct GraphModel : QObject {
Q_OBJECT
private:
    Graph<int> graph;
public:
    void addVertex() {
        graph.appendVertex(0);
    }

    void addArc(int startIndex, int endIndex) {
        graph.addArc_withIndex(startIndex, endIndex);
    }

    QPair<QVector<int>, QVector<QVariantList>> deepFirstSearch_nonRecursive(int index) {
        const auto &result = deepFirstSearch_nonRecursive_withIndex(graph, index);
        const auto &indexOrder = QVector<int>::fromStdVector(result.indexOrder);
        const auto &containerInfo = result.containerCondition;

        QVector<QVariantList> converted;
        for (const auto &deque : containerInfo) {
            QVariantList list;
            for (const auto &x : deque) { list.push_back(x); }
            converted.push_back(std::move(list));
        }

        return {indexOrder, converted};
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
