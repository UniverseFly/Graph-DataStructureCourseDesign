//
// Created by 魏宇翔 on 2019-08-19.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_APPLICATIONWINDOW_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_APPLICATIONWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include <QtCore>
#include <QMenuBar>
#include <QApplication>
#include <QAction>
#include <QGraphicsView>
#include <QCombobox>
#include <QVariantList>
#include <QLayout>
#include "GraphModel.h"
#include "VertexListItem.h"
#include "GraphItem.h"
#include "AdjointListItem.h"

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
    model.addArc(1, 4);\
    graphObject.addArc(1, 4);\
    adjointListGraph.resetFromRaw(model.getAdjointList());


struct ApplicationWindow : QGraphicsView {
Q_OBJECT
private:
    GraphModel model;
    // 栈、队列的元素信息
    VertexListItem containerInfo;
    // 用来显示搜索结果
    VertexListItem searchResult;
    // 用于显示的视图
    GraphItem graphObject;
    // 邻接表的显示
    AdjointListItem adjointListGraph;
    // 判断是否在动画中，从而切断一些用户操作
    bool isAnimating = false;
public:
    explicit ApplicationWindow(QWidget *parent = nullptr) :
            containerInfo("栈、队列情况"), searchResult("遍历结果"), QGraphicsView(parent) {
        containerInfo.setPos(-500, -280);
        searchResult.setPos(-500, -150);
        adjointListGraph.setPos(-480, 0);

        auto scene = new QGraphicsScene(QRect(-1500, -600, 3000, 1200), this);
        scene->addItem(&containerInfo);
        scene->addItem(&searchResult);
        scene->addItem(&graphObject);
        scene->addItem(&adjointListGraph);

        containerInfo.resetToRaw({"空"});
        searchResult.resetToRaw({"空"});

        // 设置视图优化
        setCacheMode(QGraphicsView::CacheBackground);
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setRenderHint(QPainter::Antialiasing);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setScene(scene);

        initMenu();

        resize(1200, 900);
        TEST_CASE
    }

private:
    void initMenu() {
        auto menuBar = new QMenuBar(this);

        // 一级菜单
        auto structure = menuBar->addMenu("结构");
        auto algorithms = menuBar->addMenu("算法");
        auto display = menuBar->addMenu("显示");
        auto help = menuBar->addMenu("帮助");

        // 二级菜单与actions
        auto addVertex = new QAction("添加顶点", this);
        auto addArc = new QAction("添加弧", this);
        auto clearGraph = new QAction("清空顶点");
        auto dfs = new QMenu("深度优先遍历", this);
        auto bfs = new QAction("广度优先遍历", this);
        auto showInstruction = new QAction("使用说明", this);
        auto zoomIn = new QAction("放大", this);
        auto zoomOut = new QAction("缩小", this);

        // 三级actions
        auto dfs_recursive = new QAction("递归", this);
        auto dfs_nonRecursive = new QAction("非递归", this);

        structure->addActions({addVertex, addArc, clearGraph});

        algorithms->addMenu(dfs);
        algorithms->addAction(bfs);

        display->addActions({zoomIn, zoomOut});

        help->addAction(showInstruction);

        dfs->addActions({dfs_nonRecursive, dfs_recursive});

        addVertex->setShortcut(Qt::CTRL + Qt::Key_V);
        addArc->setShortcut(Qt::CTRL + Qt::Key_A);
        clearGraph->setShortcut(Qt::CTRL + Qt::Key_R);
        dfs_nonRecursive->setShortcut(Qt::CTRL + Qt::Key_D);
        dfs_recursive->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_D);
        bfs->setShortcut(Qt::CTRL + Qt::Key_B);
        zoomIn->setShortcut(QKeySequence::ZoomIn);
        zoomOut->setShortcut(QKeySequence::ZoomOut);

        QObject::connect(addVertex, &QAction::triggered, this, &ApplicationWindow::addVertex);
        QObject::connect(addArc, &QAction::triggered, this, &ApplicationWindow::addArc);
        QObject::connect(clearGraph, &QAction::triggered, this, &ApplicationWindow::clearGraph);
        QObject::connect(dfs_nonRecursive, &QAction::triggered, this, &ApplicationWindow::dfs_nonRecursive);
        QObject::connect(dfs_recursive, &QAction::triggered, this, &ApplicationWindow::dfs_recursive);
        QObject::connect(bfs, &QAction::triggered, this, &ApplicationWindow::bfs);
        QObject::connect(showInstruction, &QAction::triggered, this, &ApplicationWindow::showInstruction);
        QObject::connect(zoomIn, &QAction::triggered, this, &ApplicationWindow::zoomIn);
        QObject::connect(zoomOut, &QAction::triggered, this, &ApplicationWindow::zoomOut);
    }

    void zoomIn() {
        scaleView(qreal(1.2));
    }

    void zoomOut() {
        scaleView(1 / qreal(1.2));
    };

    void showInstruction() {
        QMessageBox::information(this, "帮助", "<h4>界面上的顶点、框框均可拖动</h4>\n"
                                             "<h4>可以通过Ctrl/Command + 滚轮实现缩放</h4>\n"
                                             "<h4>已经预置了部分顶点和弧，请通过拖动分离</h4>");
    }

    void clearGraph() {
        if (isAnimating) {
            QMessageBox::warning(this, "操作失败", "请等待动画结束后操作");
            return;
        }
        model.reset();
        graphObject.reset();
        adjointListGraph.resetFromRaw({});
    }

    void bfs() {
        auto userChoice = askForTraverseStartIndex();
        if (!userChoice.first) { return; }

        animate(model.bfs(userChoice.second));
    }

    void dfs_recursive() {
        auto userChoice = askForTraverseStartIndex();
        if (!userChoice.first) { return; }

        animate(model.dfs_recursive(userChoice.second));
    }

    void dfs_nonRecursive() {
        auto userChoice = askForTraverseStartIndex();
        if (!userChoice.first) { return; }

        animate(model.dfs_nonRecursive(userChoice.second));
    }

    // $0: 确认还是取消，$1: 选择的起点。
    QPair<bool, int> askForTraverseStartIndex() {
        auto message = new QMessageBox(this);
        auto startIndexChooser = new QComboBox(message);

        message->setText("选择遍历起点");
        message->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        auto layout = new QVBoxLayout;

        layout->addWidget(startIndexChooser);
        message->layout()->addItem(layout);

        const int size = graphObject.getVertices().size();
        for (QVariant i = 0; i.value<int>() < size; i = i.value<int>() + 1) {
            startIndexChooser->addItem(i.toString());
        }
        message->exec();

        bool flagResult = true;
        if (message->result() == QMessageBox::Cancel) {
            return {false, -1};
        }

        int valueResult = startIndexChooser->currentText().toInt(&flagResult);
        if (!flagResult) {
            QMessageBox::warning(this, "Invalid index", "无效的起始顶点");
            return {false, -1};
        }

        return {flagResult, valueResult};
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
        auto machine = new QStateMachine(this);
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

        isAnimating = true;
        QObject::connect(group, &QPropertyAnimation::finished, this, &ApplicationWindow::userInteractionEnabled);
    }

    void userInteractionEnabled() { isAnimating = false; }

    void addVertex() {
        model.addVertex();
        graphObject.addVertex();
        adjointListGraph.resetFromRaw(model.getAdjointList());
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
        adjointListGraph.resetFromRaw(model.getAdjointList());
    }

protected:
    void wheelEvent(QWheelEvent *event) override {
        if (QApplication::keyboardModifiers() == Qt::CTRL) {
            scaleView(pow((double) 2, -event->delta() / 240.0));
        } else {
            QGraphicsView::wheelEvent(event);
        }
    }

private:
    void scaleView(qreal scaleFactor) {
        qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;

        scale(scaleFactor, scaleFactor);
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_APPLICATIONWINDOW_H
