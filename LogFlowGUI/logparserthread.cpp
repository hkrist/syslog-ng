#include "logparserthread.h"

ParserThread::ParserThread(QThread *parent)
                             : QThread(parent)
{
    logcap_file.setFileName("/var/log/logcap.log");
    logcap_file.open(QIODevice::ReadWrite);
}

ParserThread::~ParserThread()
{
    if (isRunning())
      {
        this->terminate();
        this->wait();
      }
}


void ParserThread::set_queue(QQueue<QString>* qq)
{
    log_exch = qq;
}


bool ParserThread::exchange_related_events(QString const& ID)
{
  QString writeback_content;
  logcap_file.seek(0);
  while (!(logcap_file.atEnd()))
    {
      QString line = logcap_file.readLine();
      if (line.isEmpty())
        {
          qDebug() << "Error parsing logcap file: empty line found";
          return false;
        }
      QStringList id_and_name = line.split(":");
      if (id_and_name.size() != 2)
          return false;
      QString input_id = id_and_name.at(0);
      QString name = id_and_name.at(1);
      name.remove(name.size()-1, 1);        // cutting off '/n'
      if (input_id == ID && name != log_exch->back())
        {
          QMutex mutex;
          mutex.lock();
          log_exch->enqueue(name);
          mutex.unlock();
        }
      else if (input_id != ID)
        {
          writeback_content += line;
        }
      else
        {
            /* ignore duplicated element logs (sources,destinations) */
        }
    }
  if (!logcap_file.resize(0))
    {
      qDebug() << "Error parsing logcap file: couldn't clear content";
      return false;
    }
  logcap_file.seek(0);
  logcap_file.write(writeback_content.toStdString().c_str());
  return true;
}

void ParserThread::exchange_CurrentID(QString const& ID)
{
  QMutex mutex;
  mutex.lock();
  log_exch->enqueue(ID);
  mutex.unlock();
}

bool ParserThread::parse_by_ID(QString const& ID)
{
  exchange_CurrentID(ID);
  for (int i = 0; i < 5; i++)  /* 5*200ms cycle, in case the message travels through syslog-ng slowly*/
    {
      if (!exchange_related_events(ID))
        return false;
      this->msleep(200);
    }
  return true;
}

/* the first log's id in the logcap_file gets to be the currently parsed ID*/
QString ParserThread::get_current_ID()
{
    logcap_file.seek(0);
    QString line = logcap_file.readLine();
    if (line[0] != '1')
      {
        return "";
      }
    QStringList id_and_name = line.split(":");
    return id_and_name.at(0);
}

bool ParserThread::parse_events_loop()
{
    while (true)
    {
      QString CurrentID = get_current_ID();
      if (!CurrentID.isEmpty())
       {
          if (!parse_by_ID(CurrentID))
            {
              return false;
            }
          emit one_event_parsed();
          this->msleep(100);
       }
      else
       {
         this->sleep(1);
       }
    }
}

bool ParserThread::parse_topology()
{
    QString writeback_content;
    while (!logcap_file.atEnd())
      {
        QString line = logcap_file.readLine();
        if (*line.begin() != 'P')
          {
            writeback_content += line;
            continue;
          }
        /* Removing the "ProcessingElement" tag and the /n from the end */
        line.remove(0,18);
        line.remove(line.size()-1,1);
        log_exch->enqueue(line);
      }
    if (!logcap_file.resize(0))
      {
        qDebug() << "Error parsing topology, couldn't clear file content";
        return false;
      }
    logcap_file.seek(0);
    logcap_file.write(writeback_content.toStdString().c_str());
    return true;
}

void ParserThread::run()
{
    /* The lines parsed by this thread are in this format: "ID:processingelementname".
     * the ID is the Unix time of the actual LogMessage's generation in Syslog-ng, this
     * is a poor temporary solution for differentiate LogMessages. The CurrentId string
     * contains the id of the LogMessage which is being parsed by the file
     */

    if (!parse_events_loop())
        qDebug() << "Error in parser thread";
    return;
}
