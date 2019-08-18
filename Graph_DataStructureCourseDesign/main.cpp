#define FRIEND_MAIN friend int main(int argc, char **argv);
#define DEBUG

#include <iostream>

#include <vector>
#include "Graph.h"
#include "GraphAlgorithms.h"

#include <QApplication>
#include <QListView>
#include "GraphModel.h"
#include "GraphView.h"

#include "Playground.h"
#include "NodeItem.h"

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

void testCase() {
    Graph<std::string> g;
    std::vector<int> v;
    g.appendVertex("zero");
    g.appendVertex("one");
    g.appendVertex("two");
    g.appendVertex("three");
    g.appendVertex("four");
    g.appendVertex("five");
    g.appendVertex("six");
    g.addArc("zero", "one");
    g.addArc("zero", "two");
    g.addArc("one", "three");
    g.addArc("one", "five");
    g.addArc("two", "five");
    g.addArc("two", "six");
    g.addArc("one", "six");
    g.deleteArc("one", "six");
    g.deleteVertex("four");
    // g.deleteArc("one", "five");
    // g.deleteArc("three", "four");
    // g.addArc("three", "four");
    // g.deleteVertex("zero");
    auto r1 = deepFirstSearch_recursive(g, std::string("zero"));
    auto r2 = breadthFirstSearch(g, std::string("zero"));
    auto r3 = deepFirstSearch_nonRecursive(g, std::string("zero"));
    // for (const auto &list : r2.containerCondition) {
    //     for (const auto &x : list) {
    //         std::cout << x << ' ';
    //     }
    //     std::cout << std::endl;
    // }
}


int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("数据结构-图");

    // NodeItem node;
    // QGraphicsScene scene;
    // scene.addItem(&node);
    // QGraphicsView view(&scene);
    GraphView view;
    view.show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &view, &GraphView::resetViewFromModel);
    timer.start(5000);

    return QApplication::exec();
}
