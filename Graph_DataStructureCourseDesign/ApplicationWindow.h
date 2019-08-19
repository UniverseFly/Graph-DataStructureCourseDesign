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
#include <QLayout>
#include "GraphModel.h"
#include "GraphicsListObject.h"
#include "GraphObject.h"

#define TEST_CASE \
    addVertex();\
    addVertex();\
    addVertex();\
    addVertex();\
    model.addArc(0, 1);\
    graphObject.addArc(0, 1);\
    model.addArc(1, 2);\
    graphObject.addArc(1, 2);

struct ApplicationWindow : QMainWindow {
    Q_OBJECT
private:
    GraphModel model;
    QGraphicsView view;
    // 栈、队列的元素信息
    GraphicsListObject containerInfo;
    // 用来显示搜索结果
    GraphicsListObject searchResult;
    // 用于显示的视图
    GraphObject graphObject;
public:
    explicit ApplicationWindow(QWidget *parent = nullptr) :
            QMainWindow(parent), view(this) {
        containerInfo.setPos(-400, -300);
        searchResult.setPos(-400, -150);

        auto scene = new QGraphicsScene(QRectF{-400, -300, 800, 600}, this);
        scene->addItem(&containerInfo);
        scene->addItem(&searchResult);
        scene->addItem(&graphObject);

        containerInfo.resetToRaw({1, 2, 3});
        searchResult.resetToRaw({9, 9, 9, 9});

        // 设置视图优化
        view.setCacheMode(QGraphicsView::CacheBackground);
        view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        view.setRenderHint(QPainter::Antialiasing);
        view.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        view.scale(qreal(1.2), qreal(1.2));
        view.setScene(scene);

        setCentralWidget(&view);
        initMenu();

        TEST_CASE
    }

private:
    void initMenu() {
        auto structure = menuBar()->addMenu("结构");
        auto algorithms = menuBar()->addMenu("算法");

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
    }

    void dfs_nonRecursive() {
        auto message = new QMessageBox(this);
        message->setText("选择遍历起点");
        message->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        auto startIndexChooser = new QComboBox(message);

        const auto &viewVertices = graphObject.getVertices();
        const int size = viewVertices.size();
        for (QVariant i = 0; i.value<int>() < size; i = i.value<int>() + 1) {
            startIndexChooser->addItem(i.toString());
        }

        message->exec();
        if (message->result() == QMessageBox::Cancel) { return; }

        const auto &result = model.deepFirstSearch_nonRecursive(startIndexChooser->currentText().toInt());
        
        // Animate
        QString text;
        QVariantList rawResult;
        auto sequenceAnimation = new QSequentialAnimationGroup;
        for (int index : result.first) {
            // 添加结果的文字说明
            std::ostringstream oss;
            oss << index << ' ';
            const QString &temp = QString::fromStdString(oss.str());
            text += temp;
            rawResult.push_back(temp);

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
            QVariantList raw;
            for (const auto &x : containerState) {
                std::ostringstream oss;
                oss << x;
                raw.push_back(QString::fromStdString(oss.str()));
            }
            auto state = new QState;
            state->assignProperty(&containerInfo, "raw", raw);
            states.push_back(state);
            machine->addState(state);
        }
        if (!states.isEmpty()) {
            machine->setInitialState(states.first());
        }

        // for (int i = 0; i < states.size() - 1; ++i) {
        //     states[i]->addTransition(next, &QPushButton::clicked, states[i + 1]);
        // }
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
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_APPLICATIONWINDOW_H
