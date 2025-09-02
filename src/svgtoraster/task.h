/**
 * \file
 * \brief Заголовочный файл с объявлением класса \ref Task "Task"
 */

#ifndef TASK_H
#define TASK_H

#include <QtCore/QtCore>

/**
 * \brief Объявление класса \ref Task "Task"
 *
 * Этот класс предназначен для выполнения "полезной нагрузки" приложения
 * в окружении Qt с очередью задач.
 */
class Task : public QObject
{
    Q_OBJECT

public:

    Task(int argc, char **argv, int minargs, QObject *parent = nullptr);

public slots:

    void run();

private:

    int parse_args(int argc, char **argv, int minargs);

    /**
     * \brief Вывод справки о программе
     */
    void print_help();

    QString m_input_file;

    QString m_output_file;

    QList<int> m_bitmap_size;

    int m_parse_ok;


signals:

    void finished();
};

#endif // TASK_H
