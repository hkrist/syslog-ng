#include "dialog.h"

void Dialog::on_listWidget_clicked(const QModelIndex &index)
{
    QListWidgetItem *clicked_item = ui->listWidget->takeItem(index.row());
    QString requested_id = clicked_item->text();
    /*Id acquired, we dont need the listitem anymore*/
    delete clicked_item;
    QStringList id_matching_event = event_hash->take(requested_id);
    for (QStringList::iterator iter = id_matching_event.begin(); iter != id_matching_event.end(); ++iter)
      {
        //qDebug() << *iter;
        visualization_scene->add_element_to_highlight(*iter);
      }
    visualization_scene->set_animation_timer();
}

void Dialog::event_exch_ready()
{
    if (parser_to_dialog_channel->isEmpty())
      {
         qDebug() << "Error when processing parsed events: channel is empty";
         return;
      }
    QMutex mutex;
    mutex.lock();
    QString id = parser_to_dialog_channel->dequeue();
    QStringList visited_elements;
    while (!parser_to_dialog_channel->isEmpty())
      {
        QString visited_element = parser_to_dialog_channel->dequeue();
        visited_elements.append(visited_element);
      }
    mutex.unlock();
    event_hash->insert(id,visited_elements);
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(id);
    ui->listWidget->addItem(newItem);
}

qreal Dialog::sourcey = 10;
qreal Dialog::filtery = 10;
qreal Dialog::parsery = 10;
qreal Dialog::rewritey = 10;
qreal Dialog::destinationy = 10;

void Dialog::add_processing_element(QString const& type, QString const& name)
{
  /* temporary solution, a Qmap with types and functions or a Qhash would be better */
  if(type == "source")
    {
      VisualElement* item = new VisualElement(name,type);
      visualization_scene->add_element_ptr(item);
      visualization_scene->addItem(item);
      item->setPos(item->mapToParent(sourcex,sourcey));
      sourcey += 100;
    }
  else
  if(type == "filter")
    {
      VisualElement* item = new VisualElement(name,type);
      visualization_scene->add_element_ptr(item);
      visualization_scene->addItem(item);
      item->setPos(item->mapToParent(filterx,filtery));
      filtery += 100;
    }
  else
  if(type == "parser")
    {
      VisualElement* item = new VisualElement(name,type);
      visualization_scene->add_element_ptr(item);
      visualization_scene->addItem(item);
      item->setPos(item->mapToParent(parserx,parsery));
      parsery += 100;
    }
  else
  if(type == "rewrite")
    {
      VisualElement* item = new VisualElement(name,type);
      visualization_scene->add_element_ptr(item);
      visualization_scene->addItem(item);
      item->setPos(item->mapToParent(rewritex,rewritey));
      rewritey += 100;
    }
  else
  if(type == "destination")
    {
      VisualElement* item = new VisualElement(name,type);
      visualization_scene->add_element_ptr(item);
      visualization_scene->addItem(item);
      item->setPos(item->mapToParent(destinationx,destinationy));
      destinationy += 100;
  }
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    visualization_scene = new VisualizationScene(this);
    ui->graphicsView->setScene(visualization_scene);
    visualization_scene->setSceneRect(0,0,1000,500);

    QLinearGradient gradient(0,0,1000,500);
    gradient.setColorAt(0, QColor::fromRgb(141,222,240,130));
    gradient.setColorAt(1, QColor::fromRgb(255,255,255,255));
    QBrush background(gradient);
    visualization_scene->setBackgroundBrush(background);

    event_hash = new QHash<QString,QStringList>();

    parser_to_dialog_channel = new QQueue<QString>();
    logcap_parser = new ParserThread();
    logcap_parser->set_queue(parser_to_dialog_channel);
    logcap_parser->parse_topology();

    /* At this point, the parser_to_dialog_channel contains the topology informations
     * in this format: "processing_element_type,processing_element_name"
     * */
    while(!parser_to_dialog_channel->isEmpty())
      {
        QString line = parser_to_dialog_channel->dequeue();

        QStringList type_and_name = line.split(",");
        QString type = type_and_name.at(0);
        QString name = type_and_name.at(1);
        add_processing_element(type, name);
      }
    connect(logcap_parser,SIGNAL(one_event_parsed()),this,SLOT(event_exch_ready()));
    logcap_parser->start();
}

Dialog::~Dialog()
{
    delete logcap_parser;
    delete event_hash;
    delete parser_to_dialog_channel;
    delete ui;
}
