#ifndef VISUALIZATIONSCENE_H
#define VISUALIZATIONSCENE_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>
#include "visualelement.h"

class VisualizationScene : public QGraphicsScene
{
    Q_OBJECT
public:
    VisualizationScene(QObject *parent = 0);
    ~VisualizationScene();
    void add_element_to_highlight(QString);
    void set_animation_timer();
    void add_element_ptr(VisualElement*);
private:
    //used when an event is selected from the event list
    QQueue<QString> *elements_to_highlight;
    QList<VisualElement*> *element_ptrs;
    QTimer *animation_timer;
signals:
public slots:
    void advance_animation();

};

#endif // VISUALIZATIONSCENE_H
