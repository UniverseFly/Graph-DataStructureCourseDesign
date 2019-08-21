//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H


#include <QGraphicsItem>
#include <QPainter>
#include <cmath>

struct VertexItem;

struct ArcItem : QGraphicsItem {
    friend struct GraphItem;
    friend struct VertexItem;
private:
    QPoint start;
    QPoint end;
    VertexItem *source = nullptr;
    VertexItem *destination = nullptr;
public:
    ArcItem(const QPoint &start, const QPoint &end, QGraphicsItem *item = nullptr) :
            start(start), end(end), QGraphicsItem(item) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    QRectF boundingRect() const override {
        qreal extra = 5.0;
        return QRectF{start, end}.normalized().adjusted(-extra, -extra, extra, extra);;
    }

    const QPoint &getEnd() const { return end; }

    void setStart(const QPoint &start) {
        ArcItem::start = start;
    }

    void setEnd(const QPoint &end) {
        ArcItem::end = end;
    }

    const QPoint &getStart() const {
        return start;
    }

private:
    void setVertices(VertexItem *source, VertexItem *destination);

    void adjustFromVertices();
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_ARCITEM_H
