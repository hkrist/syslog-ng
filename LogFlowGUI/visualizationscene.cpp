#include "visualizationscene.h"

VisualizationScene::VisualizationScene(QObject *parent)
                                      :QGraphicsScene(parent)
{
    animation_timer = new QTimer(this);
    elements_to_highlight = new QQueue<QString>();
    element_ptrs = new QList<VisualElement*>();
    connect(animation_timer, SIGNAL(timeout()),this, SLOT(advance_animation()));
}

VisualizationScene::~VisualizationScene()
{
    delete elements_to_highlight;
    delete element_ptrs;
}

void VisualizationScene::advance_animation()
{
    QString element_to_highlight;
    if (elements_to_highlight->isEmpty())
      {
        animation_timer->stop();
      }
    else
      {
        element_to_highlight = elements_to_highlight->dequeue();
      }

    for (QList<VisualElement*>::iterator iter = element_ptrs->begin(); iter != element_ptrs->end(); iter++)
      {
        if ((*iter)->get_name() == element_to_highlight)
          {
            (*iter)->set_highlight();
          }
        else if((*iter)->isHighlighted())
          {
            (*iter)->unset_highlight();
          }
      }
    update();
}

void VisualizationScene::set_animation_timer()
{
    /* it will call its slot (advance_animation()) right after this function returns */
    animation_timer->singleShot(0, this, SLOT(advance_animation()));
    /* and then every 0.5 secs */
    animation_timer->start(500);
}

void VisualizationScene::add_element_to_highlight(QString element)
{
    elements_to_highlight->enqueue(element);
}

void VisualizationScene::add_element_ptr(VisualElement *element)
{
    element_ptrs->append(element);
}
