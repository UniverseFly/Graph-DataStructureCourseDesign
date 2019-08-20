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
    using Result = QPair<QVector<int>, QVector<QVariantList>>;

    void addVertex() {
        graph.appendVertex(0);
    }

    void addArc(int startIndex, int endIndex) {
        graph.addArc_withIndex(startIndex, endIndex);
    }

    Result dfs_nonRecursive(int index) const {
        return traverseAlgorithm(deepFirstSearch_nonRecursive_withIndex, index);
    }

    Result dfs_recursive(int index) const {
        return traverseAlgorithm(deepFirstSearch_recursive_withIndex, index);
    }

    Result bfs(int index) const {
        return traverseAlgorithm(breadthFirstSearch_withIndex, index);
    }

    QVector<QPair<QVariant, QSet<int>>> getAdjointList() const {
        QVector<QPair<QVariant, QSet<int>>> result;
        for (int index = 0; index < graph.verticesCount(); ++index) {
            const auto &vertex = graph.vertexAt(index);

            QSet<int> adjNodes;
            for (const auto &node : vertex.getAdjList()) { adjNodes.insert(node); }

            QPair<QVariant, QSet<int>> pair{index, adjNodes};
            result.push_back(std::move(pair));
        }
        return result;
    }

    void reset() {
        graph = {};
    }

private:
    // 接受函数指针，代表某种遍历算法。对所有不同的搜索算法进通用的行类型转换适配。
    Result traverseAlgorithm(SearchResult algorithm(const Graph<int> &, int), int startIndex) const {
        const auto &result = algorithm(graph, startIndex);
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
