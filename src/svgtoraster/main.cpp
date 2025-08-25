#include <QIcon>
#include <QSize>
#include "bootstrap.h"

#define quoting(a) prequoting(a)
#define prequoting(a) #a

int main(int argc, char *argv[]) {

    /**
     * Алгоритм:
     * * Создать экземпляр типа QApplication
     */
    QApplication a(argc, argv);

    /**
     * * Выполнить начальные действия: настроить вывод отладочных сообщений,
     * установить локализацию согласно файлу конфигурации. 
     */
    bootstrap::init(argc, argv);

    QImage image = QIcon("./icon.svg").pixmap(QSize(1024, 1024)).toImage();

    image.save("./icon.png");

    return 0;
}
