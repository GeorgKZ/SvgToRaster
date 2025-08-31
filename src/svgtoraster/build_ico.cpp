#include <QtWidgets/QApplication>
#include <QtGui/QImage>

//#include <QImage>
#include <QFile>
#include <QBuffer>
#include <QDataStream>

#include "build_ico.h"

void saveIco(const QString &filePath) {

    int size[] = {256,48,32,16};
    int size_len = 4;

    // Создаем изображения разных размеров
    QList<QImage> images;
    for (int i = 0; i < size_len; ++i)
    {
        images.append(QImage(size[i], size[i], QImage::Format_ARGB32));
    }

    // Заполняем изображения (например, цветом)
    for (int i = 0; i < images.size(); ++i) {
        images[i].fill(qRgb(255, 0, 0)); // Заполняем красным цветом
    }

    // Создаем ICO файл
    QFile icoFile(filePath);
    if (!icoFile.open(QIODevice::WriteOnly)) {
        qWarning("Cannot open file for writing");
        return;
    }

    // Записываем заголовок ICO ICONDIR
//    QByteArray header;
    QDataStream out(&icoFile); //, QIODevice::WriteOnly);
    out << (quint16)0x0000;        // Значение зарезервировано и должно содержать ноль
    out << (quint16)0x0001;        // Тип файла, принимает значение 1 для ICO и 2 для CUR
    out << (quint16)images.size(); // Количество изображений в файле

    // Записываем информацию о каждом изображении ICONDIRENTRY
    for (const QImage &image : images) {
        out << (quint8)image.width(); // Ширина
        out << (quint8)image.height(); // Высота
        out << (quint8)0x00; // Количество цветов
        out << (quint8)0x00; // Зарезервировано
        out << (quint16)0x0000; // Количество битов на пиксель или 0
        out << (quint32)(image.width() * image.height() * 4); // Размер изображения
        out << (quint32)0x00000000; // Смещение структуры BITMAPINFOHEADER
    }

#if 1
    // Записываем данные изображений
    for (const QImage &image : images) {
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "BMP"); // Сохраняем как BMP для ICO
        buffer.close();
        icoFile.write(imageData);
    }
#endif

    icoFile.close();
}
