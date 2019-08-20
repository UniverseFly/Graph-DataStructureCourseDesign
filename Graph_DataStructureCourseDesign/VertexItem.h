//
// Created by 魏宇翔 on 2019-08-17.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_VERTEXITEM_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_VERTEXITEM_H


#include <QGraphicsItem>
#include <QPainter>
#include <QList>
#include <QVector>
#include <QPen>

struct GraphObject;
struct ArcItem;

struct VertexItem : QGraphicsObject {
Q_OBJECT
    friend struct ArcItem;
    friend struct GraphObject;
private:
    QString text;
    GraphObject *graph = nullptr;
    QVector<ArcItem *> arcs;
    QRect bound = {-20, -20, 40, 40};
    int radius;
public:
    explicit VertexItem(int radius, const QString &text, bool movable = true, QGraphicsItem *parent = nullptr) :
            text(text), QGraphicsObject(parent), radius(radius) {
        if (movable) {
            setFlag(ItemIsMovable);
            setFlag(ItemSendsGeometryChanges);
        }
        setCacheMode(DeviceCoordinateCache);
        setZValue(-1);
        setScale(radius / 20.0);
    }

    QRectF boundingRect() const override {
        double adjustedRadius = radius + 6.0;
        return {-adjustedRadius, -adjustedRadius, 2 * adjustedRadius, 2 * adjustedRadius};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(QColor(235, 235, 235));
        painter->setPen(QPen(Qt::black, 3.3, Qt::SolidLine));
        QFont font;
        font.setPointSize(15);
        painter->setFont(font);

        painter->drawEllipse(bound);
        painter->drawText(bound, Qt::AlignCenter, text);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_VERTEXITEM_H
