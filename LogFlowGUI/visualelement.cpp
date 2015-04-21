#include "visualelement.h"


VisualElement::VisualElement()
{
}

VisualElement::VisualElement(QString const& name, QString const& type)
{
    set_name(name);
    set_type(type);
    highlighted = false;
}

VisualElement::~VisualElement()
{

}

void VisualElement::set_name(QString const& name)
{
    this->name = name;
}

void VisualElement::set_type(QString const& type)
{
    this->type = type;
}

void VisualElement::set_highlight()
{
    highlighted = true;
}

void VisualElement::unset_highlight()
{
    highlighted = false;
}

bool VisualElement::isHighlighted()
{
    return highlighted;
}

QString VisualElement::get_name() const
{
    return name;
}

QString VisualElement::get_type() const
{
    return type;
}

void VisualElement::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::SolidPattern);

    if (highlighted)
      {
        brush.setColor(Qt::white);
      }
    else
      {
        brush.setColor(Qt::gray);
      }

    painter->drawRect(rec);
    painter->fillRect(rec, brush);
    painter->drawText(rec,name);
}

QRectF VisualElement::boundingRect() const
{
    return QRectF(0,0,100,50);
}
