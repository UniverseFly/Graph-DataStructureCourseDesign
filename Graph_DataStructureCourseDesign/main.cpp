#define FRIEND_MAIN friend int main(int argc, char **argv);
#define DEBUG

#include <iostream>

#include <vector>
#include "Graph.h"
#include "GraphAlgorithms.h"

#include <QApplication>
#include <QListView>
#include "ApplicationWindow.h"
#include "GraphModel.h"
#include "VertexListObject.h"

#include "GraphObject.h"
#include "AdjointNodeObject.h"
#include "AdjointListObject.h"

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QComboBox>
#include <QListWidget>

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

    ApplicationWindow window;
    window.show();

    VertexListObject itemListView({1, 2, 9, 10, 25});
    AdjointListObject graphObject;
    graphObject.resetFromRaw(1, {2, 3, 4});
    graphObject.resetFromRaw(1, {});
    graphObject.resetFromRaw(1, {2, 3, 4});
    QGraphicsScene scene;
    scene.addItem(&graphObject);
    QGraphicsView view;
    view.scale(1.2, 1.2);
    view.setScene(&scene);
    view.show();

    return QApplication::exec();
}
