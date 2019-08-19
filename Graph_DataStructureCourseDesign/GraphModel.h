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

    QPair<QVector<int>, QVector<std::deque<int>>> deepFirstSearch_nonRecursive(int index) {
        const auto &result = deepFirstSearch_nonRecursive_withIndex(graph, index);
        const auto &indexOrder = QVector<int>::fromStdVector(result.indexOrder);
        const auto &containerInfo = QVector<std::deque<int>>::fromStdVector(result.containerCondition);

        for (const auto &x : containerInfo) {
            std::vector<int> vec{x.cbegin(), x.cend()};
            qDebug() << QVector<int>::fromStdVector(vec);
        }

        return {indexOrder, containerInfo};
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
