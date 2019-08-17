#include <iostream>
#include <vector>
#include "Graph.h"
#include "GraphAlgorithms.h"

#include <QApplication>
#include <QListView>
#include "GraphModel.h"
#include <QDebug>

// void test() {
//     Graph<std::string> g;
//     std::vector<int> v;
//     g.appendVertex("zero");
//     g.appendVertex("one");
//     g.appendVertex("two");
//     g.appendVertex("three");
//     g.appendVertex("four");
//     g.appendVertex("five");
//     g.appendVertex("six");
//     g.addArc("zero", "one");
//     g.addArc("zero", "two");
//     g.addArc("one", "three");
//     g.addArc("one", "five");
//     g.addArc("two", "five");
//     g.addArc("two", "six");
//     g.addArc("one", "six");
//     g.deleteArc("one", "six");
//     g.deleteVertex("four");
//     // g.deleteArc("one", "five");
//     // g.deleteArc("three", "four");
//     // g.addArc("three", "four");
//     // g.deleteVertex("zero");
//     auto r1 = deepFirstSearch_recursive(g, std::string("zero"));
//     auto r2 = breadthFirstSearch(g, std::string("zero"));
//     auto r3 = deepFirstSearch_nonRecursive(g, std::string("zero"));
//     // for (const auto &list : r2.containerCondition) {
//     //     for (const auto &x : list) {
//     //         std::cout << x << ' ';
//     //     }
//     //     std::cout << std::endl;
//     // }
// }

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("数据结构-图");

    GraphModel model;
    QListView view;

    view.setModel(&model);
    view.show();

    qDebug() << model.index(0, 0).data();

    return QApplication::exec();
}
