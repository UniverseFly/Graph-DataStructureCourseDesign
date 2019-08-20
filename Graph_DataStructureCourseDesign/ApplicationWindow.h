//
// Created by 魏宇翔 on 2019-08-19.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_APPLICATIONWINDOW_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_APPLICATIONWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include <QtCore>
#include <QMenuBar>
#include <QAction>
#include <QGraphicsView>
#include <QCombobox>
#include <QVariantList>
#include <QLayout>
#include "GraphModel.h"
#include "VertexListObject.h"
#include "GraphObject.h"
#include "AdjointListGraphObject.h"

#define TEST_CASE \
    addVertex();\
    addVertex();\
    addVertex();\
    addVertex();\
    addVertex();\
    model.addArc(0, 1);\
    graphObject.addArc(0, 1);\
    model.addArc(1, 2);\
    graphObject.addArc(1, 2);\
    model.addArc(0, 3);\
    graphObject.addArc(0, 3);\
    model.addArc(1, 2);\
    graphObject.addArc(1, 2);\
    model.addArc(1, 4);\
    graphObject.addArc(1, 4);\


struct ApplicationWindow : QGraphicsView {
Q_OBJECT
private:
    GraphModel model;
    // 栈、队列的元素信息
    VertexListObject containerInfo;
    // 用来显示搜索结果
    VertexListObject searchResult;
    // 用于显示的视图
    GraphObject graphObject;
    // 邻接表的显示
    AdjointListGraphObject adjointListGraph;
public:
    explicit ApplicationWindow(QWidget *parent = nullptr) :
            QGraphicsView(parent) {
        containerInfo.setPos(-500, 0);
        searchResult.setPos(-500, 150);
        adjointListGraph.setPos(-500, 300);

        auto scene = new QGraphicsScene(QRect(-600, -600, 1200, 1200), this);
        scene->addItem(&containerInfo);
        scene->addItem(&searchResult);
        scene->addItem(&graphObject);
        scene->addItem(&adjointListGraph);

        containerInfo.resetToRaw({1, 2, 3});
        searchResult.resetToRaw({9, 9, 9, 9});

        // 设置视图优化
        setCacheMode(QGraphicsView::CacheBackground);
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setRenderHint(QPainter::Antialiasing);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        scale(qreal(1.2), qreal(1.2));
        setScene(scene);

        initMenu();

        TEST_CASE
    }

private:
    void initMenu() {
        auto menuBar = new QMenuBar(this);
        auto structure = menuBar->addMenu("结构");
        auto algorithms = menuBar->addMenu("算法");

        auto addVertex = new QAction("添加顶点", this);
        auto addArc = new QAction("添加弧", this);
        auto dfs = new QMenu("深度优先遍历", this);
        auto bfs = new QAction("广度优先遍历", this);

        auto dfs_recursive = new QAction("递归", this);
        auto dfs_nonRecursive = new QAction("非递归", this);

        structure->addActions({addVertex, addArc});

        algorithms->addMenu(dfs);
        algorithms->addAction(bfs);

        dfs->addActions({dfs_nonRecursive, dfs_recursive});

        QObject::connect(addVertex, &QAction::triggered, this, &ApplicationWindow::addVertex);
        QObject::connect(addArc, &QAction::triggered, this, &ApplicationWindow::addArc);
        QObject::connect(dfs_nonRecursive, &QAction::triggered, this, &ApplicationWindow::dfs_nonRecursive);
        QObject::connect(dfs_recursive, &QAction::triggered, this, &ApplicationWindow::dfs_recursive);
        QObject::connect(bfs, &QAction::triggered, this, &ApplicationWindow::bfs);

    }

    void bfs() {
        auto userChoice = askForTraverseStartIndex();
        if (userChoice.first == QMessageBox::Cancel) { return; }

        animate(model.bfs(userChoice.second));
    }

    void dfs_recursive() {
        auto userChoice = askForTraverseStartIndex();
        if (userChoice.first == QMessageBox::Cancel) { return; }

        animate(model.dfs_recursive(userChoice.second));
    }

    void dfs_nonRecursive() {
        auto userChoice = askForTraverseStartIndex();
        if (userChoice.first == QMessageBox::Cancel) { return; }

        animate(model.dfs_nonRecursive(userChoice.second));
    }

    // $0: 确认还是取消，$1: 选择的起点。
    QPair<int, int> askForTraverseStartIndex() {
        auto message = new QMessageBox(this);
        auto startIndexChooser = new QComboBox(message);
        message->setText("选择遍历起点");
        message->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        const int size = graphObject.getVertices().size();
        for (QVariant i = 0; i.value<int>() < size; i = i.value<int>() + 1) {
            startIndexChooser->addItem(i.toString());
        }
        message->exec();

        return {message->result(), startIndexChooser->currentText().toInt()};
    }

    // 根据返回结果进行动画渲染。
    void animate(const QPair<QVector<int>, QVector<QVariantList>> &result) {
        const auto &viewVertices = graphObject.getVertices();
        QVariantList rawResult;
        auto sequenceAnimation = new QSequentialAnimationGroup;
        for (int index : result.first) {
            std::ostringstream oss;
            oss << index;
            rawResult.push_back(QString::fromStdString(oss.str()));

            auto parallelAnimation = new QParallelAnimationGroup;

            auto scaleAnimation = new QPropertyAnimation(viewVertices[index], "scale");
            scaleAnimation->setDuration(1500);
            scaleAnimation->setStartValue(1);
            scaleAnimation->setKeyValueAt(0.5, 0);
            scaleAnimation->setEndValue(1);
            scaleAnimation->setEasingCurve(QEasingCurve::OutBounce);

            auto opacityAnimation = new QPropertyAnimation(viewVertices[index], "opacity");
            opacityAnimation->setDuration(1500);
            opacityAnimation->setStartValue(1);
            opacityAnimation->setKeyValueAt(0.5, 0.3);
            opacityAnimation->setEndValue(1);
            opacityAnimation->setEasingCurve(QEasingCurve::OutQuad);

            parallelAnimation->addAnimation(scaleAnimation);
            parallelAnimation->addAnimation(opacityAnimation);

            sequenceAnimation->addAnimation(parallelAnimation);
        }
        searchResult.resetToRaw(rawResult);

        auto group = new QSequentialAnimationGroup;

        for (const auto &x : searchResult.getSubObjects()) {
            x->setOpacity(0);
            auto animation = new QPropertyAnimation(x, "opacity");
            animation->setStartValue(0);
            animation->setEndValue(1);
            animation->setDuration(1500);
            group->addAnimation(animation);
        }
        group->start(QPropertyAnimation::DeleteWhenStopped);

        QVector<QState *> states;
        auto machine = new QStateMachine;
        for (const auto &containerState : result.second) {
            auto state = new QState(machine);
            state->assignProperty(&containerInfo, "raw", containerState);
            states.push_back(state);
        }
        if (!states.isEmpty()) {
            machine->setInitialState(states.first());
        } else {
            auto empty = new QState(machine);
            empty->assignProperty(&containerInfo, "raw", {});
            machine->setInitialState(empty);
        }

        auto timer = new QTimer(this);
        timer->setInterval(1500);
        timer->start();

        for (int i = 0; i < states.size() - 1; ++i) {
            states[i]->addTransition(timer, &QTimer::timeout, states[i + 1]);
        }
        machine->start();

        sequenceAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    }

    void addVertex() {
        model.addVertex();
        graphObject.addVertex();
    }

    void addArc() {
        auto message = new QMessageBox(this);
        message->setText("选择起点和终点");
        message->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        auto layout = new QVBoxLayout;
        auto startIndexChooser = new QComboBox(message);
        auto endIndexChooser = new QComboBox(message);
        layout->addWidget(startIndexChooser);
        layout->addWidget(endIndexChooser);

        message->layout()->addItem(layout);

        const int size = graphObject.getVertices().size();
        for (QVariant i = 0; i.value<int>() < size; i = i.value<int>() + 1) {
            startIndexChooser->addItem(i.toString());
            endIndexChooser->addItem(i.toString());
        }

        startIndexChooser->setStatusTip("选择起始顶点");
        endIndexChooser->setStatusTip("选择目的顶点");

        message->exec();
        if (message->result() == QMessageBox::Cancel) { return; }

        int start = startIndexChooser->currentText().toInt();
        int end = endIndexChooser->currentText().toInt();

        if (start == end) {
            QMessageBox::warning(this, "错误", "起点与终点不能相同");
            return;
        }
        if (graphObject.getArcs().contains({start, end})) {
            QMessageBox::warning(this, "错误", "这条弧已经存在，不能重复添加");
        }

        model.addArc(start, end);
        graphObject.addArc(start, end);
    }

// protected:
//     void wheelEvent(QWheelEvent *event) override {
//         scaleView(pow((double) 2, -event->delta() / 240.0));
//     }
//
// private:
//     void scaleView(qreal scaleFactor) {
//         qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
//         if (factor < 0.07 || factor > 100)
//             return;
//
//         scale(scaleFactor, scaleFactor);
//     }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_APPLICATIONWINDOW_H
