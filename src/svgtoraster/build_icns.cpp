/**
 * \file
 *
 * \brief Файл с определениями функций, необходимыми для преобразования
 * векторного изображения <a href="https://doc.qt.io/qt-6/qicon.html">QIcon</a>
 * в растровый значок формата <a href="https://en.wikipedia.org/wiki/Apple_Icon_Image_format">ICNS</a> с несколькими битмапами различного размера.
 */

#include <QFile>
#include <QBuffer>
#include "build_icns.h"

#include <QDebug>

/**
 * \brief Четырёхбуквенные коды типов битмапов, помещаемых в ICNS
 */
static const char* OSTYPE_ID[] = {
 "ICON","ICN#","icm#","icm4","icm8","ics#","ics4","ics8","is32","s8mk","icl4","icl8","il32",
 "l8mk","ich#","ich4","ich8","ih32","h8mk","it32","t8mk","icp4","icp5","icp6","ic07","ic08",
 "ic09","ic10","ic11","ic12","ic13","ic14","ic04","ic05","icsb","icsB","sb24","SB24",
 "TOC ", "info"
};

static const QList<int> icns_sizes = { 64,    128,  256,  256,  512,  512, 1024,   32 };
static const QList<int> icns_codes = { ic12, ic07, ic13, ic08, ic14, ic09, ic10, ic11 };
static const size_t icns_size_num = icns_sizes.size();


/**
 * \file
 * Функции, являющиеся методами класса \ref BYTES04_BE "BYTES04_BE"
 * и дружественные этому классу:
 * <BR>
 */

/**
 * \file
 * * \copybrief BYTES04_BE::BYTES04_BE(quint32)
 */
BYTES04_BE::BYTES04_BE(quint32 value) : m_value(value) {}

BYTES04_BE::BYTES04_BE(const char *str)
{
    m_value = 0;
    if (str[0] != '\0') m_value |= ((static_cast<quint32>(str[0])) << 24); else return;
    if (str[1] != '\0') m_value |= ((static_cast<quint32>(str[1])) << 16); else return;
    if (str[2] != '\0') m_value |= ((static_cast<quint32>(str[2])) <<  8); else return;
    if (str[3] != '\0') m_value |= ((static_cast<quint32>(str[3])) <<  0);
}

/**
 * \file
 * * \copybrief BYTES04_BE::operator=(quint32)
 */
BYTES04_BE &BYTES04_BE::operator=(quint32 value)
{
    m_value = value;
    return *this;
}

/**
 * \file
 * * \copybrief BYTES04_BE::operator+=(quint32)
 */
BYTES04_BE &BYTES04_BE::operator+=(quint32 value)
{
    m_value += value;
    return *this;
}

/**
 * \file
 * * Оператор приведения к типу quint32
 */
BYTES04_BE::operator quint32() const
{
    return m_value;
}

/**
 * \file
 * * Запись структуры \ref BYTES04_BE "BYTES04_BE" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator<<(QDataStream &out, const BYTES04_BE &b)
{
    out <<
      static_cast<quint8>((b.m_value >> 24) & 0xFF) <<
      static_cast<quint8>((b.m_value >> 16) & 0xFF) <<
      static_cast<quint8>((b.m_value >> 8)  & 0xFF) <<
      static_cast<quint8>((b.m_value >> 0)  & 0xFF);
    return out;
}

/**
 * \file
 * * Чтение структуры \ref BYTES04_BE "BYTES04_BE" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator>>(QDataStream &in, BYTES04_BE &b)
{
    quint8 b1, b2, b3, b4;
    in >> b4 >> b3 >> b2 >> b1;
    b.m_value =
      ((static_cast<quint32>(b1)) << 0)  |
      ((static_cast<quint32>(b2)) << 8)  |
      ((static_cast<quint32>(b3)) << 16) |
      ((static_cast<quint32>(b4)) << 24);
    return in;
}

/**
 * \file
 * Функции, дружественные классу \ref tagICNSHDR "tagICNSHDR":
 * <BR>
 */

/**
 * \file
 * * Запись структуры \ref ICNSHDR "ICNSHDR" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator<<(QDataStream &out, const tagICNSHDR &ihdr)
{
    out <<
      ihdr.m_magic <<
      ihdr.m_fsize;
    return out;
}

/**
 * \file
 * * Чтение структуры \ref ICNSHDR "ICNSHDR" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator>>(QDataStream &in, tagICNSHDR &ihdr)
{
    in >>
      ihdr.m_magic >>
      ihdr.m_fsize;
    return in;
}

/**
 * \file
 * Функции, дружественные классу \ref tagICNSDATA "tagICNSDATA":
 * <BR>
 */

/**
 * \file
 * * Запись структуры \ref ICNSDATA "ICNSDATA" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator<<(QDataStream &out, const tagICNSDATA &idata) {
    out <<
      idata.m_ostype <<
      idata.m_dsize;
    return out;
}

/**
 * \file
 * * Чтение структуры \ref ICNSDATA "ICNSDATA" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 */
QDataStream &operator>>(QDataStream &in, tagICNSDATA &idata) {
    in >>
      idata.m_ostype >>
      idata.m_dsize;
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
int saveIcns(const QIcon &icon, const QString &filePath) {

    /**
     * Алгоритм:
     */

    /**
     * * Сформировать заголовок ICNSHDR.
     */
    ICNSHDR iheader = { "icns", sizeof(ICNSHDR)};

    /**
     * * Для каждого размера растра:
     */
    QList<QByteArray> array_list(icns_size_num);
    for (size_t i = 0; i < icns_size_num; ++i)
    {
        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Создать изображение необходимого размера из исходного значка.
         */
        QImage img = icon.pixmap(QSize(icns_sizes[i], icns_sizes[i])).toImage();
        img.convertTo(QImage::Format_ARGB32);

        //TODO Для ic04, ic5 использовать сжатие ARGB

        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Сформировать в буфере битмап PNG
         */
        QBuffer buffer(&array_list[i]);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG");

        /**
         * * &nbsp;&nbsp;&nbsp;&nbsp;Учесть размер битмапа и заголовка в общем размере файла
         */
        iheader.m_fsize += sizeof(ICNSDATA) + array_list[i].size();
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
     * * Записать в поток заголовок ICONHDR.
     */
    out << iheader;

    /**
     * * Для каждого размера растра записать в поток заголовок ICNSDATA и битмап
     */
    for (size_t i = 0; i < icns_size_num; ++i) {
        ICNSDATA idata = { OSTYPE_ID[icns_codes[i]], sizeof(ICNSDATA) + array_list[i].size() };
        out << idata;
        out.writeRawData(array_list[i].constData(), array_list[i].size());
    }

    /**
     * * Закрыть файл значка.
     */
    icoFile.close();
    return 0;
}
