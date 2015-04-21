#ifndef VISUALELEMENT_H
#define VISUALELEMENT_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtCore>


class VisualElement : public QGraphicsItem
{
public:
    VisualElement();
    VisualElement(QString const& name,QString const& type);
    QRectF boundingRect() const;
    void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);
    ~VisualElement();

    QString get_name() const;
    QString get_type() const;

    void set_name(QString const& name);
    void set_type(QString const& type);

    void set_highlight();
    void unset_highlight();

    bool isHighlighted();

private:
    QString name;
    QString type;

    bool highlighted;
};

#endif
