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
     * \param[in] parent указатель на экземпляр родительского класса
     */
    Task(int argc, char **argv, QObject *parent = nullptr);

public slots:

    /**
     * \brief Слот выполнения полезной нагрузки класса
     */
    void run();

private:

    /**
     * \brief Разбор аргументов командной строки
     *
     * \param[in] argc количество аргументов командной строки
     * \param[in] argv массив аргументов-строк командной строки
     * \retval 0 если разбор аргументов командной строки завершился успешно
     * \retval -1 если разбор аргументов командной строки завершился с ошибкой
     */
    int parse_args(int argc, char **argv);

    /**
     * \brief Вывод справки о программе
     */
    void print_help();

    /**
     * \brief Имя исходного файла
     */
    QString m_input_file;

    /**
     * \brief Имя результирующего файла
     */
    QString m_output_file;

    /**
     * \brief Список требуемых размеров битмапов
     */
    QStringList m_bitmap_size;

    /**
     * \brief Код завершения оперрации разбора командной строки
     */
    int m_parse_ok;

    /**
     * \brief Текущая директория
     */
    QDir m_currDir;


signals:

    /**
     * \brief Сигнал завершения выполнения полезной нагрузки класса
     */
    void finished();
};

#endif // TASK_H
