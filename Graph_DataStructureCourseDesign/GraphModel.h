//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H


#include <QAbstractItemModel>
#include "Graph.h"
#include <string>

struct GraphModel : QAbstractItemModel {
Q_OBJECT
private:
    Graph<std::string> graph;
public:
    GraphModel() {
        graph.appendVertex("hello");
        graph.appendVertex("world");
        graph.appendVertex("me");
        graph.appendVertex("wyx");
    }

    int rowCount(const QModelIndex &parent) const override {
        return graph.verticesCount();
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid() || index.row() >= graph.verticesCount()) { return {}; }

        if (role != Qt::DisplayRole)
            return {};

        return QString::fromStdString(graph.vertexAt(index.row()).getValue());
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role != Qt::DisplayRole) { return {}; }

        return "Fuck";
    }

    int columnCount(const QModelIndex &parent) const override {
        return 1;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent) const override {
        if (row < 0 || row >= graph.verticesCount() || parent.isValid()) { return {}; }
        return createIndex(row, column);
    }

    QModelIndex index(int row, int column) const { return index(row, column, {}); }

    QModelIndex parent(const QModelIndex &index) const override { return {}; }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHMODEL_H
