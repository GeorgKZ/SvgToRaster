//#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include "bootstrap.h"
#include "task.h"

/**
 * \brief Минимально возможное количество аргументов (сама программа, исходный и результирующий файлы)
 */
#define MINARGS 3

int main(int argc, char *argv[]) {

    /**
     * Алгоритм:
     * <BR>
     */

    /**
     * * Создать экземпляр приложения QApplication.
     */
    QApplication a(argc, argv);

    /**
     * * Выполнить начальные действия: настроить вывод сообщений,
     * установить локализацию. 
     */
    bootstrap::init();

    /**
     * * Создать экземпляр задачи, дочернего для экземпляра приложения.
     * Завершение родительского объекта вызовет завершение задачи.
     */
    Task *task = new Task(argc, argv, MINARGS, &a);

    /**
     * * Соединить сигнал завершения, посыляемый задачей, с сигналом
     * завершения приложения.
     */
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    /**
     * * Инициировать начало выполнения задачи.
     */
    QTimer::singleShot(0, task, SLOT(run()));

    /**
     * * Передать управление приложению.
     */
    return a.exec();
}
