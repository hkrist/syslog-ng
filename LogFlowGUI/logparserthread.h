#ifndef PARSETHREAD_H
#define PARSETHREAD_H
#include <QtCore>

class ParserThread : public QThread
{
    Q_OBJECT
public:
    explicit ParserThread(QThread *parent = 0);
    bool parse_topology();
    void set_queue(QQueue<QString>*);
    void run(); Q_DECL_OVERRIDE
    void parse_end();
    ~ParserThread();

private:
    bool parse_events_loop();

    QString get_current_ID();
    void exchange_CurrentID(QString const& ID);
    bool parse_by_ID(QString const& ID);
    bool exchange_related_events(QString const& ID);



    /* the file with the syslog topology and message events */
    QFile logcap_file;
    /* contains the information parsed by the logcap_file */
    QQueue<QString> *log_exch;

signals:
    void one_event_parsed();

public slots:
};

#endif
