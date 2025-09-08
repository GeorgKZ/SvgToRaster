/**
 * \file
 *
 * \brief Файл с определениями функций, необходимыми для преобразования
 * векторного изображения <a href="https://doc.qt.io/qt-6/qicon.html">QIcon</a>
 * в растровый значок формата <ahref="https://ru.wikipedia.org/wiki/ICO_(%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%82_%D1%84%D0%B0%D0%B9%D0%BB%D0%B0)">ICO</a> с несколькими битмапами различного размера.
 */

#include <QFile>
#include "build_ico.h"

/**
 * \file
 * Функции, являющиеся методами класса \ref BYTES02_LE "BYTES02_LE"
 * и дружественные этому классу:
 * <BR>
 */

/**
 * \file
 * * \copybrief BYTES02_LE::BYTES02_LE(quint16)
 */
BYTES02_LE::BYTES02_LE(quint16 value) : m_value(value) {}

/**
 * \file
 * * \copybrief BYTES02_LE::operator=(quint16)
 */
BYTES02_LE &BYTES02_LE::operator=(quint16 value)
{
    m_value = value;
    return *this;
}

/**
 * \file
 * * \copybrief BYTES02_LE::operator+=(quint16)
 */
BYTES02_LE &BYTES02_LE::operator+=(quint16 value)
{
    m_value += value;
    return *this;
}

/**
 * \file
 * * Оператор приведения к типу quint16
 */
BYTES02_LE::operator quint16() const
{
    return m_value;
}

/**
 * \file
 * * Запись структуры \ref BYTES02_LE "BYTES02_LE" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator<<(QDataStream &out, const BYTES02_LE &b)
{
    out <<
      static_cast<quint8>(b.m_value & 0xFF) <<
      static_cast<quint8>((b.m_value >> 8) & 0xFF);
    return out;
}

/**
 * \file
 * * Чтение структуры \ref BYTES02_LE "BYTES02_LE" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator>>(QDataStream &in, BYTES02_LE &b)
{
    quint8 b1, b2;
    in >> b1 >> b2;
    b.m_value =
      ((static_cast<quint32>(b1)) << 0)  |
      ((static_cast<quint32>(b2)) << 8);
    return in;
}

/**
 * \file
 * Функции, являющиеся методами класса \ref BYTES04_LE "BYTES04_LE"
 * и дружественные этому классу:
 * <BR>
 */

/**
 * \file
 * * \copybrief BYTES04_LE::BYTES04_LE(quint32)
 */
BYTES04_LE::BYTES04_LE(quint32 value) : m_value(value) {}

/**
 * \file
 * * \copybrief BYTES04_LE::operator=(quint32)
 */
BYTES04_LE &BYTES04_LE::operator=(quint32 value)
{
    m_value = value;
    return *this;
}

/**
 * \file
 * * \copybrief BYTES04_LE::operator+=(quint32)
 */
BYTES04_LE &BYTES04_LE::operator+=(quint32 value)
{
    m_value += value;
    return *this;
}

/**
 * \file
 * * Оператор приведения к типу quint32
 */
BYTES04_LE::operator quint32() const
{
    return m_value;
}

/**
 * \file
 * * Запись структуры \ref BYTES04_LE "BYTES04_LE" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator<<(QDataStream &out, const BYTES04_LE &b)
{
    out <<
      static_cast<quint8>((b.m_value >> 0)  & 0xFF) <<
      static_cast<quint8>((b.m_value >> 8)  & 0xFF) <<
      static_cast<quint8>((b.m_value >> 16) & 0xFF) <<
      static_cast<quint8>((b.m_value >> 24) & 0xFF);
    return out;
}

/**
 * \file
 * * Чтение структуры \ref BYTES04_LE "BYTES04_LE" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator>>(QDataStream &in, BYTES04_LE &b)
{
    quint8 b1, b2, b3, b4;
    in >> b1 >> b2 >> b3 >> b4;
    b.m_value =
      ((static_cast<quint32>(b1)) << 0)  |
      ((static_cast<quint32>(b2)) << 8)  |
      ((static_cast<quint32>(b3)) << 16) |
      ((static_cast<quint32>(b4)) << 24);
    return in;
}

/**
 * \file
 * Функции, дружественные классу \ref tagBITMAPINFOHEADER "tagBITMAPINFOHEADER":
 * <BR>
 */

/**
 * \file
 * * \copybrief operator<<(QDataStream&, const tagBITMAPINFOHEADER&)
 */
QDataStream &operator<<(QDataStream &out, const tagBITMAPINFOHEADER &ihdr)
{
    out <<
      ihdr.biSize <<
      ihdr.biWidth <<
      ihdr.biHeight <<
      ihdr.biPlanes <<
      ihdr.biBitCount <<
      ihdr.biCompression <<
      ihdr.biSizeImage <<
      ihdr.biXPelsPerMeter <<
      ihdr.biYPelsPerMeter <<
      ihdr.biClrUsed <<
      ihdr.biClrImportant;
    return out;
}

/**
 * \file
 * * \copybrief operator>>(QDataStream&, tagBITMAPINFOHEADER&)
 */
QDataStream &operator>>(QDataStream &in, tagBITMAPINFOHEADER &ihdr)
{
    in >>
      ihdr.biSize >>
      ihdr.biWidth >>
      ihdr.biHeight >>
      ihdr.biPlanes >>
      ihdr.biBitCount >>
      ihdr.biCompression >>
      ihdr.biSizeImage >>
      ihdr.biXPelsPerMeter >>
      ihdr.biYPelsPerMeter >>
      ihdr.biClrUsed >>
      ihdr.biClrImportant;
    return in;
}

/**
 * \file
 * Функции, дружественные классу \ref tagICONDIR "tagICONDIR":
 * <BR>
 */

/**
 * \file
 * * \copybrief operator<<(QDataStream&, const tagICONDIR&)
 */
QDataStream &operator<<(QDataStream &out, const tagICONDIR &icondir) {
    out <<
      icondir.ihReserved <<
      icondir.ihType <<
      icondir.ihCount;
    return out;
}

/**
 * \file
 * * \copybrief operator>>(QDataStream&, tagICONDIR&)
 */
QDataStream &operator>>(QDataStream &in, tagICONDIR &icondir) {
    in >>
      icondir.ihReserved >>
      icondir.ihType >>
      icondir.ihCount;
    return in;
}

/**
 * \file
 * Функции, дружественные классу \ref tagICONDIRENTRY "tagICONDIRENTRY":
 * <BR>
 */

/**
 * \file
 * * \copybrief operator<<(QDataStream&, const tagICONDIRENTRY&)
 */
QDataStream &operator<<(QDataStream &out, const tagICONDIRENTRY &icondirentry) {
    out <<
      icondirentry.iiWidth <<
      icondirentry.iiHeight <<
      icondirentry.iiColors <<
      icondirentry.iiReserved <<
      icondirentry.iiPlanes <<
      icondirentry.iiBpp <<
      icondirentry.iiSize <<
      icondirentry.iiOffset;
    return out;
}

/**
 * \file
 * * \copybrief operator>>(QDataStream&, tagICONDIRENTRY&)
 */
QDataStream &operator>>(QDataStream &in, tagICONDIRENTRY &icondirentry) {
    in >>
      icondirentry.iiWidth >>
      icondirentry.iiHeight >>
      icondirentry.iiColors >>
      icondirentry.iiReserved >>
      icondirentry.iiPlanes >>
      icondirentry.iiBpp >>
      icondirentry.iiSize >>
      icondirentry.iiOffset;
    return in;
}

/**
 * \file
 * Функции, не являющиеся методами:
 * <BR>
 */

/**
 * \file
 * * \copybrief saveIco(const QIcon&, const QString&, const QList<int>&)
 */
int saveIco(const QIcon &icon, const QString &filePath, const QList<int> &sizes) {

    /**
     * Алгоритм:
     */

    /**
     * * Создать изображения разных размеров из исходного значка.
     */
    QList<QImage> images;
    for (const int &size : sizes)
    {
        QImage img = icon.pixmap(QSize(size, size)).toImage();
        img.convertTo(QImage::Format_ARGB32);
        images.append(img);
    }

    /**
     * * Создать файл значка.
     */
    QFile icoFile(filePath);
    if (!icoFile.open(QIODevice::WriteOnly)) {
        return -1;
    }
    QDataStream out(&icoFile);

    /**
     * * Сформировать заголовок ICONHDR.
     */
    ICONDIR iheader = { };
    iheader.ihType = 1;
    iheader.ihCount = images.size();

    /**
     * * Записать в файл заголовок ICONHDR.
     */
    out << iheader;

    /**
     * * Для каждого размера растра:
     */
    quint32 offset = sizeof(ICONDIR);
    for (const QImage &image : images) {

        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Сформировать заголовок ICONDIR, установив значения полей:
         */
        ICONDIRENTRY iinfo = { };
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ширина изображения в точках, 0 означает ширину 256 точек или более */
        iinfo.iiWidth = static_cast<quint8>(image.width() > 255 ? 0 : image.width());
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Высота изображения в точках, 0 означает высоту 256 точек или более */
        iinfo.iiHeight = static_cast<quint8>(image.height() > 255 ? 0 : image.height());
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Указывает количество цветов в палитре изображения. Для полноцветных значков 0 */
        iinfo.iiColors = 0;    
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Определяет количество плоскостей. Может быть 0 или 1. */
        iinfo.iiPlanes = 1;           
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Определяет количество битов на пиксель. Может быть 0 или больше. */
        iinfo.iiBpp = BITS_IN_BYTE * RGBA_SIZE;
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Указывает размер растра в байтах: размер цветовой матрицы */
        iinfo.iiSize += static_cast<quint32>(image.width() * image.height() * RGBA_SIZE);
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;плюс размер матрицы прозрачности */
        iinfo.iiSize += static_cast<quint32>(image.height() * ((image.width() / BITS_IN_BYTE + (BMP_BYTE_ALIGN-1))/BMP_BYTE_ALIGN*BMP_BYTE_ALIGN));
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;плюс размер заголовка */
        iinfo.iiSize += static_cast<quint32>(sizeof(BITMAPINFOHEADER));
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Указывает абсолютное смещение структуры BITMAPINFOHEADER в файле */
        iinfo.iiOffset = offset + static_cast<quint32>(sizeof(ICONDIRENTRY)) * iheader.ihCount;

        offset += iinfo.iiSize;

        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Записать в файл заголовок ICONDIR.
         */
        out << iinfo;
    }

    /**
     * * Для каждого размера растра:
     */
    for (const QImage &image : images) {
        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Сформировать заголовок BITMAPINFOHEADER, установив значения полей:
         */
        BITMAPINFOHEADER ibih = { };
        ibih.biSize = sizeof(BITMAPINFOHEADER);
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Размер по горизонтали соответствует размеру формируемого растра */
        ibih.biWidth = static_cast<qint32>(image.width());
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Размер по вертикали удвоен (два растра --- растр цветности и растр прозрачности) */
        ibih.biHeight = static_cast<qint32>(image.height()) * 2;
        ibih.biPlanes = 1;
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;На один пиксел приходится четыре восьмибитных канала */
        ibih.biBitCount = BITS_IN_BYTE * RGBA_SIZE;
        ibih.biCompression = BI_RGB;
    
        /** * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Размер в байтах строки растра прозрачности, дополнен до кратности строки 4 байтам; */
        int horsize = ((image.width() / BITS_IN_BYTE + (BMP_BYTE_ALIGN - 1)) / BMP_BYTE_ALIGN * BMP_BYTE_ALIGN);
    
        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Размер состоит из размера растра цветности b-g-r-alpha (автоматически размер строки кратен 4 байтам)
         * и размера битового растра прозрачности.
         */
        ibih.biSizeImage = static_cast<quint32>(image.width() * image.height() * RGBA_SIZE);
        ibih.biSizeImage += static_cast<quint32>(image.height() * horsize);
    
        ibih.biXPelsPerMeter = 0;
        ibih.biYPelsPerMeter = 0;
        ibih.biClrUsed = 0;
        ibih.biClrImportant = 0;
    
        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Записать в файл заголовок BITMAPINFOHEADER;
         */
        out << ibih;
  
        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Заполнить буфер растра цветности масштабированным изображением размером image.width() * image.height() * RGBA_SIZE;
         */
        for (int h = image.height() - 1; h >= 0; --h)
        {
            for (int w = 0; w < image.width(); ++w)
            {
                QRgb rgb = image.pixel(w, h);
                if (qAlpha(rgb) == 0)
                {
                    out << static_cast<quint8>(0);
                    out << static_cast<quint8>(0);
                    out << static_cast<quint8>(0);
                    out << static_cast<quint8>(0);
                } else {
                    out << static_cast<quint8>(qBlue(rgb));
                    out << static_cast<quint8>(qGreen(rgb));
                    out << static_cast<quint8>(qRed(rgb));
                    out << static_cast<quint8>(qAlpha(rgb));
                }
            }
        }
  
        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Для используемого формата растра цветности данные битового растра прозрачности (бит на точку)
         * не используются, но для совместимости они должны быть заполнены по правилу: 
         *
         * * &nbsp;&nbsp;&nbsp;&nbsp;pixel = (screen AND mask_matrix) XOR color_matrix
         *
         * * &nbsp;&nbsp;&nbsp;&nbsp;То есть для прозрачных точек необходимо заменить точку цветового растра на чёрную
         * и поставить точку растра прозрачности, равную единице, а для непрозрачных точек
         * сохранить цветную точку цветового растра и поставить нулевую точку растра прозрачности.
         */
  
        for (int h = image.height() - 1; h >= 0; --h) {
            for (int w = 0; w < horsize; ++w) {
                quint8 byte = 0;
                int bitmask = 1;
                for (int b = 0; b < BITS_IN_BYTE; ++b) {
                    /**
                     * * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Условие, когда точка обозначается прозрачной в растре прозрачности:
                     * или она лежит за пределами цветового растра, или она обозначена прозрачной
                     * в альфа-канатле цветового растра.
                     */
                    if ((b + w * BITS_IN_BYTE >= image.width()) ||
                        (qAlpha(image.pixel(w, h)) == 0)) byte |= bitmask;
                    bitmask <<= 1;
                }
                out << byte;
            }
        }
    }

    /**
     * * Закрыть файл значка.
     */
    icoFile.close();
    return 0;
}
