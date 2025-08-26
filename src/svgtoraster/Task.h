/**
 * \file
 * \brief Файл с объявлением класса \ref Task "Task"
 */

#ifndef TASK_H
#define TASK_H

#include <QtCore/QtCore>

class Task : public QObject
{
    Q_OBJECT

public:

    Task(const int &argc, char **argv, QObject *parent = nullptr);

    int parse_args(const int &argc, char **argv);

    void print_help();

private:

    QString m_input_file;

    QString m_output_file;

    int m_bitmap_size;

    int m_parse_ok;

public slots:

    void run();

signals:

    void finished();
};

#endif // TASK_H
