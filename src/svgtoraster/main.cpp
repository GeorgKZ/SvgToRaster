#include "stdafx.h"
#include "bootstrap.h"
#include "Task.h"

//#define quoting(a) prequoting(a)
//#define prequoting(a) #a

int main(int argc, char *argv[]) {

    /**
     * Алгоритм:
     * <BR>
     */

    /**
     * * Создать экземпляр типа QApplication
     */
    QApplication a(argc, argv);

    /**
     * * Выполнить начальные действия: настроить вывод сообщений,
     * установить локализацию. 
     */
    bootstrap::init();

    // Task parented to the application so that it
    // will be deleted by the application.
    Task *task = new Task(argc, argv, &a);

    // This will cause the application to exit when
    // the task signals finished.    
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}
