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
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include <QState>
#include <QStateMachine>
#include <QtCore>
#include <QTextEdit>

struct GraphView : QGraphicsView {
Q_OBJECT
#ifdef DEBUG

    FRIEND_MAIN

#endif
private:
    GraphModel model;
    QList<NodeItem *> nodes;
    QMap<QPair<int, int>, ArcItem *> arcs;
private:
    QLineEdit *startLineEdit = new QLineEdit(this);
    QLineEdit *endLineEdit = new QLineEdit(this);
    QPushButton *result = new QPushButton(this);
    QPushButton *stackInfo = new QPushButton(this);
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

        result->move(0, 180);

        stackInfo->move(0, 220);
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
        const auto &searchResult = model.deepFirstSearch_nonRecursive();
        auto label = new QPushButton("haha", this);
        label->show();

        auto machine = new QStateMachine(this);
        auto group = new QState;

        auto states = new QVector<QState *>;

        QString text;
        for (int index : searchResult) {
            // 添加结果的文字说明
            std::ostringstream oss;
            oss << index << ' ';
            text += QString::fromStdString(oss.str());
            auto position = QGraphicsView::mapFromScene(nodes[index]->pos());
            auto state = new QState(group);
            state->assignProperty(label, "pos", position);
            states->push_back(state);
        }
        // set result label
        result->setText(text);
        qDebug() << text;

        group->setInitialState(states->first());
        machine->addState(group);
        machine->setInitialState(group);

        auto sequenceAnimation = new QSequentialAnimationGroup;

        auto timer = new QTimer;
        timer->setInterval(2000);
        QObject::connect(group, &QState::entered, timer, QOverload<>::of(&QTimer::start));

        for (int i = 0; i < states->size() - 1; ++i) {
            auto animation = new QPropertyAnimation(label, "pos", this);
            animation->setDuration(1500);
            (*states)[i]->addTransition(timer, &QTimer::timeout, (*states)[i + 1])
                    ->addAnimation(animation);
        }


        machine->start();
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
