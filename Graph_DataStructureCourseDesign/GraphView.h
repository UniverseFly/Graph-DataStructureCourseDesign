//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHVIEW_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHVIEW_H


#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>
#include <sstream>
#include <QLineEdit>
#include <QMessageBox>
#include "ArcItem.h"
#include "NodeItem.h"
#include "ContainerView.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include <QState>
#include <QStateMachine>
#include <QtCore>
#include <QTextEdit>
#include <deque>

struct GraphView : QGraphicsView {
Q_OBJECT
#ifdef DEBUG

    FRIEND_MAIN

#endif
private:
    GraphModel model;
    QVector<NodeItem *> nodes;
    QMap<QPair<int, int>, ArcItem *> arcs;
private:
    QLineEdit *startLineEdit = new QLineEdit(this);
    QLineEdit *endLineEdit = new QLineEdit(this);
    QLineEdit *searchIndex = new QLineEdit(this);
    QPushButton *result = new QPushButton(this);
    QPushButton *stackInfo = new QPushButton(this);
    ContainerView *container = new ContainerView(this);
    QPushButton *next = new QPushButton(this);
    ContainerView *resultContainer = new ContainerView(this);
public:
    explicit GraphView(QWidget *parent = nullptr) {
        auto scene = new QGraphicsScene(this);
        scene->setSceneRect(-200, -200, 400, 400);
        setScene(scene);
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);

        setCacheMode(CacheBackground);
        setViewportUpdateMode(BoundingRectViewportUpdate);
        setRenderHint(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);

        // 这个必须要有
        scale(qreal(1.2), qreal(1.2));

        setMinimumSize(400, 400);

        resetViewFromModel();

        auto *button = new QPushButton(this);
        button->setText("add node");
        QObject::connect(button, &QPushButton::clicked, this, &GraphView::addNode);


        auto okButton = new QPushButton(this);
        okButton->setText("add arc");
        QObject::connect(okButton, &QPushButton::clicked, this, &GraphView::addArc);

        auto dfsButton = new QPushButton(this);
        dfsButton->setText("dfs");
        QObject::connect(dfsButton, &QPushButton::clicked, this, &GraphView::deepFirstSearchShow);

        startLineEdit->move(0, 0);
        endLineEdit->move(0, 20);
        button->move(0, 60);
        okButton->move(0, 40);
        dfsButton->move(0, 100);

        searchIndex->move(0, 130);

        result->move(0, 180);

        stackInfo->move(0, 220);

        next->setText("next");
        next->move(0, 140);

        container->move(100, 0);
        resultContainer->move(300, 0);
    }

    void addNode() {
        model.addVertex("fuck", {0, 0});
        resetViewFromModel();
    }

    void addArc() {
        bool startSuccess = false, endSuccess = false;
        int start = startLineEdit->text().toInt(&startSuccess);
        int end = endLineEdit->text().toInt(&endSuccess);
        if (!startSuccess || !endSuccess ||
            start < 0 || start >= nodes.count() ||
            end < 0 || end >= nodes.count() ||
            arcs.keys().contains({start, end})) {
            QMessageBox::warning(this, "Invalid vertex index", "Please try again");
            return;
        }

        model.addArc(start, end);
        resetViewFromModel();
    }

    void updateItem(NodeItem *nodeToUpdate) {
        const auto &newPosition = QPoint(nodeToUpdate->pos().x(), nodeToUpdate->pos().y());
        int index = findNodeIndex(nodeToUpdate);
        if (index == -1) { return; }

        // 视图的改变通知模型的改变
        model.setVertexPosition(index, newPosition);

        auto iter = arcs.cbegin();
        while (iter != arcs.cend()) {
            const auto &startPosition = model.getVertexPositions()[iter.key().first];
            const auto &endPosition = model.getVertexPositions()[iter.key().second];

            QLineF lineF(startPosition, endPosition);
            QPointF offset(10 * lineF.dx() / lineF.length(), 10 * lineF.dy() / lineF.length());

            auto newStart = startPosition + QPoint(offset.x(), offset.y());
            auto newEnd = endPosition - QPoint(offset.x(), offset.y());
            if (iter.key().first == index || iter.key().second == index) {
                iter.value()->setStart(newStart);
                iter.value()->setEnd(newEnd);
            }
            ++iter;
        }
    }

    void deepFirstSearchShow() {
        bool ok = false;
        int startIndex = searchIndex->text().toInt(&ok);
        if (!ok || startIndex >= nodes.size()) {
            QMessageBox::warning(this, "", "Index out of range");
            return;
        }

        const auto &searchResult = model.deepFirstSearch_nonRecursive(startIndex);

        QString text;
        QVariantList rawResult;
        auto sequenceAnimation = new QSequentialAnimationGroup;
        for (int index : searchResult.first) {
            // 添加结果的文字说明
            std::ostringstream oss;
            oss << index << ' ';
            const QString &temp = QString::fromStdString(oss.str());
            text += temp;
            rawResult.push_back(temp);
            auto position = QGraphicsView::mapFromScene(nodes[index]->pos());

            auto parallelAnimation = new QParallelAnimationGroup;

            auto scaleAnimation = new QPropertyAnimation(nodes[index], "scale");
            scaleAnimation->setDuration(1500);
            scaleAnimation->setStartValue(1);
            scaleAnimation->setKeyValueAt(0.5, 0);
            scaleAnimation->setEndValue(1);
            scaleAnimation->setEasingCurve(QEasingCurve::OutBounce);

            auto opacityAnimation = new QPropertyAnimation(nodes[index], "opacity");
            opacityAnimation->setDuration(1500);
            opacityAnimation->setStartValue(1);
            opacityAnimation->setKeyValueAt(0.5, 0.3);
            opacityAnimation->setEndValue(1);
            opacityAnimation->setEasingCurve(QEasingCurve::OutQuad);

            parallelAnimation->addAnimation(scaleAnimation);
            parallelAnimation->addAnimation(opacityAnimation);

            sequenceAnimation->addAnimation(parallelAnimation);
        }
        resultContainer->setRaw(rawResult);

        auto group = new QSequentialAnimationGroup;

        for (const auto &x : resultContainer->nodes) {
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
        for (const auto &containerInfo : searchResult.second) {
            QVariantList raw;
            for (const auto &x : containerInfo) {
                std::ostringstream oss;
                oss << x;
                raw.push_back(QString::fromStdString(oss.str()));
            }
            auto state = new QState;
            state->assignProperty(container, "raw", raw);
            states.push_back(state);
            machine->addState(state);
        }
        if (!states.isEmpty()) {
            machine->setInitialState(states.first());
        }

        for (int i = 0; i < states.size() - 1; ++i) {
            states[i]->addTransition(next, &QPushButton::clicked, states[i + 1]);
        }
        machine->start();

        // set result label
        result->setText(text);

        sequenceAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    }

private:
    void resetViewFromModel() {
        assert(scene() != nullptr);

        for (const auto &node : nodes) { delete node; }
        for (const auto &arc : arcs.values()) { delete arc; }
        nodes = {};
        arcs = {};

        int index = 0;
        for (const auto &position : model.getVertexPositions()) {
            std::ostringstream oss;
            oss << index;
            QString text = QString::fromStdString(oss.str());

            auto node = new NodeItem(text);
            QObject::connect(node, &NodeItem::nodePositionChanged, this, &GraphView::updateItem);

            node->setPos(position);
            nodes.push_back(node);
            scene()->addItem(node);

            ++index;
        }
        for (const auto &arc : model.getArcs()) {
            const auto &startPosition = model.getVertexPositions()[arc.first];
            const auto &endPosition = model.getVertexPositions()[arc.second];

            QLineF lineF(startPosition, endPosition);
            QPointF offset(10 * lineF.dx() / lineF.length(), 10 * lineF.dy() / lineF.length());

            auto newArc = new ArcItem(startPosition + QPoint(offset.x(), offset.y()),
                                      endPosition - QPoint(offset.x(), offset.y()));
            arcs.insert({arc.first, arc.second}, newArc);
            scene()->addItem(newArc);
        }
    }

private:
    int findNodeIndex(NodeItem *nodeToFind) const {
        int index = 0;
        for (const auto &node : nodes) {
            if (node == nodeToFind) { return index; }
            index += 1;
        }
        return -1;
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHVIEW_H
