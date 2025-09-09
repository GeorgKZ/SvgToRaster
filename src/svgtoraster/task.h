/**
 * \file
 * \brief Заголовочный файл с объявлением класса \ref Task "Task"
 */

#ifndef TASK_H
#define TASK_H

#include <QtCore>

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

    /**
     * \brief Конструктор класса \ref Task "Task"
     *
     * \param[in] argc количество аргументов командной строки
     * \param[in] argv массив аргументов-строк командной строки
     * \param[in] minargs минимально допустимое количество аргументов командной строки
     * \param[in] parent указатель на экземпляр родительского класса
     */
    Task(int argc, char **argv, int minargs, QObject *parent = nullptr);

public slots:

    /**
     * \brief Выполнение полезной нагрузки класса
     */
    void run();

private:

    /**
     * \brief Разбор аргументов командной строки
     *
     * \param[in] argc количество аргументов командной строки
     * \param[in] argv массив аргументов-строк командной строки
     * \param[in] minargs минимально допустимое количество аргументов командной строки
     * \retval 0 если разбор аргументов командной строки завершился успешно
     * \retval -1 если разбор аргументов командной строки завершился с ошибкой
     */
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
