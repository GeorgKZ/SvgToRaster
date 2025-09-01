#include <QImage>
#include <QIcon>
#include <QFile>
#include <QDebug>
#include <QDataStream>

#include "build_ico.h"

BYTES02::BYTES02(quint16 value) : m_value(value) {}

BYTES02 &BYTES02::operator=(quint16 value)
{
    m_value = value;
    return *this;
}

BYTES02::operator quint16() const
{
    return m_value;
}

QDataStream &operator<<(QDataStream &out, const BYTES02 &b)
{
    out <<
      static_cast<quint8>(b.m_value & 0xFF) <<
      static_cast<quint8>((b.m_value >> 8) & 0xFF);
    return out;
}

QDataStream &operator>>(QDataStream &in, BYTES02 &b)
{
    quint8 b1, b2;
    in >> b1 >> b2;
    b.m_value = b1 | (((quint16)b2) << 8);
    return in;
}

BYTES04::BYTES04(quint32 value) : m_value(value) {}

BYTES04 &BYTES04::operator=(quint32 value)
{
    m_value = value;
    return *this;
}

BYTES04::operator quint32() const
{
    return m_value;
}


QDataStream &operator<<(QDataStream &out, const BYTES04 &b)
{
    out <<
      static_cast<quint8>((b.m_value >> 0)  & 0xFF) <<
      static_cast<quint8>((b.m_value >> 8)  & 0xFF) <<
      static_cast<quint8>((b.m_value >> 16) & 0xFF) <<
      static_cast<quint8>((b.m_value >> 24) & 0xFF);
    return out;
}

QDataStream &operator>>(QDataStream &in, BYTES04 &b)
{
    quint8 b1, b2, b3, b4;
    in >> b1 >> b2 >> b3 >> b4;
    b.m_value = (quint32)b1 |
      (((quint32)b2) << 8) |
      (((quint32)b3) << 16) <<
      (((quint32)b4) << 24);
    return in;
}




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
 * * \copybrief operator<<(QDataStream&, tagICONDIRENTRY&)
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
     * * Создать файл формата ICO.
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
         * *	Сформировать заголовок ICONDIR, установив значения полей:
         */
        ICONDIRENTRY iinfo = { };
        /** *		Ширина изображения в точках, 0 означает ширину 256 точек или более */
        iinfo.iiWidth = static_cast<quint8>(image.width() > 255 ? 0 : image.width());
        /** *		Высота изображения в точках, 0 означает высоту 256 точек или более */
        iinfo.iiHeight = static_cast<quint8>(image.height() > 255 ? 0 : image.height());
        /** *		Указывает количество цветов в палитре изображения. Для полноцветных значков 0 */
        iinfo.iiColors = 0;    
        /** *		Определяет количество плоскостей. Может быть 0 или 1. */
        iinfo.iiPlanes = 1;           
        /** *		Определяет количество битов на пиксель. Может быть 0 или больше. */
        iinfo.iiBpp = BITS_IN_BYTE * RGBA_SIZE;
        /** *		Указывает размер растра в байтах: размер цветовой матрицы */
        /** плюс размер матрицы прозрачности плюс размер заголовка */
        iinfo.iiSize = static_cast<quint32>(image.width() * image.height() * RGBA_SIZE) +
          static_cast<quint32>(image.height() * ((image.width() / BITS_IN_BYTE + (BMP_BYTE_ALIGN-1))/BMP_BYTE_ALIGN*BMP_BYTE_ALIGN)) +
          static_cast<quint32>(sizeof(BITMAPINFOHEADER));
        /** *		Указывает абсолютное смещение структуры BITMAPINFOHEADER в файле */
        iinfo.iiOffset = offset + static_cast<quint32>(sizeof(ICONDIRENTRY)) * iheader.ihCount;

        /**
         * *	Записать в файл заголовок ICONDIR.
         */
        out << iinfo;
    }

    /**
     * * Для каждого размера растра:
     */
    for (const QImage &image : images) {
        /**
         * *	Сформировать заголовок BITMAPINFOHEADER, установив значения полей:
         */
        BITMAPINFOHEADER ibih = { };
        ibih.biSize = sizeof(BITMAPINFOHEADER);
        /** * Размер по горизонтали соответствует размеру формируемого растра */
        ibih.biWidth = static_cast<qint32>(image.width());
        /** * Размер по вертикали удвоен (два растра --- растр цветности и растр прозрачности) */
        ibih.biHeight = static_cast<qint32>(image.height()) * 2;
        ibih.biPlanes = 1;
        /** * На один пиксел приходится четыре восьмибитных канала */
        ibih.biBitCount = BITS_IN_BYTE * RGBA_SIZE;
        ibih.biCompression = BI_RGB;
    
        /** *    Размер в байтах строки растра прозрачности, дополнен до кратности строки 4 байтам */
        int horsize = ((image.width() / BITS_IN_BYTE + (BMP_BYTE_ALIGN - 1)) / BMP_BYTE_ALIGN * BMP_BYTE_ALIGN);
    
        /**
         * *    Размер состоит из размера растра цветности b-g-r-alpha (автоматически размер строки кратен 4 байтам)
         * и размера битового растра прозрачности.
         */
        quint32 biSizeImage = static_cast<quint32>(image.width() * image.height() * RGBA_SIZE);
        biSizeImage += static_cast<quint32>(image.height() * horsize);
        ibih.biSizeImage = biSizeImage;
    
        ibih.biXPelsPerMeter = 0;
        ibih.biYPelsPerMeter = 0;
        ibih.biClrUsed = 0;
        ibih.biClrImportant = 0;
    
        /**
         * *    Записать в файл заголовок BITMAPINFOHEADER;
         */
        out << ibih;
  
        /**
         * *    Заполнить буфер растра цветности масштабированным изображением размером image.width() * image.height() * RGBA_SIZE;
         */
        for (int h = image.height() - 1; h >= 0; --h)
        {
            for (int w = 0; w < image.width(); ++w)
            {
                QRgb rgb = image.pixel(w, h);
                out << (quint8)qBlue(rgb);
                out << (quint8)qGreen(rgb);
                out << (quint8)qRed(rgb);
                out << (quint8)qAlpha(rgb);
            }
        }
  
        /**
         * Для используемого формата растра цветности данные битового растра прозрачности (бит на точку)
         * не используются, но для совместимости они должны быть заполнены по правилу: 
         *
         * pixel = (screen AND mask_matrix) XOR color_matrix
         *
         * То есть для прозрачных точек необходимо заменить точку цветового растра на чёрную
         * и поставить точку растра прозрачности, равную единице, а для непрозрачных точек
         * сохранить цветную точку цветового растра и поставить нулевую точку растра прозрачности.
         */
  
        for (int ky = image.height() - 1; ky >= 0; --ky) {
            for (int kx = 0; kx < horsize; ++kx) {
                quint8 byte = 0;
                int bitmask = 1;
                for (int kz = 0; kz < BITS_IN_BYTE; ++kz) {
                    /**
                     * Условие, когда точка обозначается прозрачной в растре прозрачности:
                     * или она лежит за пределами цветового растра, или она обозначена прозрачной
                     * в альфа-канатле цветового растра.
                     */
                    if ((kz + kx * BITS_IN_BYTE >= image.width()) ||
                        (qAlpha(image.pixel(kx, ky)) == 0)) byte |= bitmask;
                    bitmask <<= 1;
                }
                out << byte;
            }
        }
    }

    icoFile.close();
    return 0;
}
