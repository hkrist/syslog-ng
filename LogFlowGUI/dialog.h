#ifndef DIALOG_H
#define DIALOG_H

#include <QtCore>
#include <QtGui>
#include "logparserthread.h"
#include "ui_dialog.h"
#include "visualelement.h"
#include "visualizationscene.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    void add_processing_element(QString const& type, QString const& name);

    static const qreal sourcex = 20;
    static const qreal filterx = 220;
    static const qreal parserx = 420;
    static const qreal rewritex = 620;
    static const qreal destinationx = 820;
    static qreal sourcey;
    static qreal filtery;
    static qreal parsery;
    static qreal rewritey;
    static qreal destinationy;

    ParserThread *logcap_parser;
    QQueue<QString> *parser_to_dialog_channel;
    QHash<QString,QStringList> *event_hash;

    Ui::Dialog *ui;
    VisualizationScene *visualization_scene;
public slots:
    void event_exch_ready();

signals:

private slots:
    void on_listWidget_clicked(const QModelIndex &index);
};

#endif // DIALOG_H
