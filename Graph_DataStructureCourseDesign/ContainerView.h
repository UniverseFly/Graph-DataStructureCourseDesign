//
// Created by 魏宇翔 on 2019-08-18.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_CONTAINERVIEW_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_CONTAINERVIEW_H


#include <QGraphicsView>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsRectItem>
#include <QSequentialAnimationGroup>
#include <QDebug>
#include "NodeItem.h"

struct ContainerView : QGraphicsView {
Q_OBJECT
Q_PROPERTY(QVariantList raw READ getRaw WRITE setRaw)
#ifdef DEBUG
    FRIEND_MAIN
#endif
public:
    QVariantList raw = {"1", "2", "3"};
    QVector<NodeItem *> nodes;
    QPoint pointToStore = {-55, 0};
    QGraphicsLineItem *topBoard = new QGraphicsLineItem(-80, -25, 80, -25);
    QGraphicsLineItem *bottomBoard = new QGraphicsLineItem(-80, 25, 80, 25);
    int space = 40;
public:
    explicit ContainerView(QWidget *parent = nullptr) : QGraphicsView(parent) {
        auto scene = new QGraphicsScene(this);
        scene->setSceneRect(-80, -40, 160, 80);
        setScene(scene);
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        topBoard->setPen(QPen(Qt::darkBlue, 3));
        bottomBoard->setPen(QPen(Qt::darkBlue, 3));
        scene->addItem(topBoard);
        scene->addItem(bottomBoard);

        setCacheMode(CacheBackground);
        setViewportUpdateMode(BoundingRectViewportUpdate);
        setRenderHint(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);

        // 这个必须要有
        scale(qreal(1.2), qreal(1.2));
        resetViewFromRaw();
    }

public:
    virtual const QPoint &positionToStore() const {
        return pointToStore;
    }


    // 不是虚函数，根据位置来修改。
    void store(const QString &x) {
        auto node = new NodeItem(x, false);
        node->setScale(1.2);
        node->setPos(positionToStore());
        nodes.push_back(node);
        scene()->addItem(node);
        pointToStore += {30, 0};
    }

    void animate() {
        auto parallelAnimation = new QParallelAnimationGroup;
        pointToStore -= {space, 0};
        for (const auto &node : nodes) {
            auto animation = new QPropertyAnimation(node, "pos");
            animation->setEndValue(node->pos() - QPointF{qreal(space), 0});
            animation->setDuration(1000);
            parallelAnimation->addAnimation(animation);
        }
        auto node = new NodeItem("5", false);
        node->setPos(60, 0);
        scene()->addItem(node);
        auto animation = new QPropertyAnimation(node, "pos");
        animation->setEndValue(QPointF(-25, 0));
        animation->setDuration(1000);
        parallelAnimation->addAnimation(animation);
        parallelAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    }

    QVariantList getRaw() const { return raw; }

    void setRaw(QVariantList newRaw) {
        qDebug() << newRaw;
        raw = newRaw;
        resetViewFromRaw();
    }

    void resetViewFromRaw() {
        assert(scene() != nullptr);
        for (const auto &ptr : nodes) { delete ptr; }
        nodes = {};
        pointToStore = {-55, 0};

        for (const auto &x : raw) {
            store(x.value<QString>());
        }
    }

    QPropertyAnimation *animationTo(const QVariantList &newRaw) {
        auto animation = new QPropertyAnimation(this, "raw");
        animation->setEndValue(newRaw);
        return animation;
    }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_CONTAINERVIEW_H
