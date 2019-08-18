//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_PLAYGROUND_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_PLAYGROUND_H


#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include "GraphModel.h"

#define MAIN friend int main(int argc, char **argv);

struct Playground : QMainWindow {
Q_OBJECT
#ifdef DEBUG
    MAIN
#endif
private:
    QGraphicsScene *scene = new QGraphicsScene({0, 0, 800, 600}, this);
    QGraphicsView *view = new QGraphicsView(scene, this);
    GraphModel model;
    QVector<QGraphicsLineItem *> lines;
    QVector<QGraphicsEllipseItem *> circles;
public:
    explicit Playground(QWidget *parent = nullptr) : QMainWindow(parent) {
        setCentralWidget(view);
        resetViewFromModel();
        view->show();
    }

    void resetViewFromModel() {
        for (const auto &ptr : lines) { delete ptr; }
        for (const auto &ptr : circles) { delete ptr; }
        lines = {};
        circles = {};

        for (const auto &position : model.getVertexPositions()) {
            circles.push_back(scene->addEllipse(position.x(), position.y(), 20, 20));
        }

        for (const auto &arc : model.getArcs()) {
            const auto &startPosition = model.getVertexPositions()[arc.first];
            const auto &endPosition = model.getVertexPositions()[arc.second];
            lines.push_back(scene->addLine(startPosition.x() + 10, startPosition.y() + 10,
                                           endPosition.x() + 10, endPosition.y() + 10));
        }
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_PLAYGROUND_H
