/**
 * \file
 * \brief Файл с определениями функций, необходимыми для преобразования
 * векторного изображения <a href="https://doc.qt.io/qt-6/qicon.html">QIcon</a>
 * в растровый значок формата <a href="https://en.wikipedia.org/wiki/Apple_Icon_Image_format">ICNS</a> с несколькими битмапами различного размера.
 */

#include <QFile>
#include <QBuffer>
//#include <QDebug>
#include "build_icns.h"

/**
 * \brief Четырёхбуквенные коды типов битмапов, которые могут быть помещены в ICNS
 */
static const char* OSTYPE_ID[] = {
 "ICON","ICN#","icm#","icm4","icm8","ics#","ics4","ics8","is32","s8mk","icl4","icl8","il32",
 "l8mk","ich#","ich4","ich8","ih32","h8mk","it32","t8mk","icp4","icp5","icp6","ic07","ic08",
 "ic09","ic10","ic11","ic12","ic13","ic14","ic04","ic05","icsb","icsB","sb24","SB24",
 "TOC ", "info"
};

/** \brief Размеры битмапов, которые будут помещены в ICNS */
static const QList<int> icns_sizes = { 64,    128,  256,  256,   16,  512,  512,   32, 1024,   32 };

/** \brief Коды типов  битмапов, которые будут помещены в ICNS */
static const QList<int> icns_codes = { ic12, ic07, ic13, ic08, ic04, ic14, ic09, ic05, ic10, ic11 };

/** \brief Количество битмапов, которые будут помещены в ICNS */
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
     * 1 Сформировать заголовок ICNSHDR.
     */
    ICNSHDR iheader = { "icns", sizeof(ICNSHDR)};

    /**
     * 2 Для каждого размера растра:
     */
    QList<QByteArray> array_list(icns_size_num);
    for (size_t i = 0; i < icns_size_num; ++i)
    {
        /**
         * &nbsp;&nbsp;&nbsp;&nbsp;2.1 Создать изображение необходимого размера из исходного значка.
         */
        QImage img = icon.pixmap(QSize(icns_sizes[i], icns_sizes[i])).toImage();
        img.convertTo(QImage::Format_ARGB32);

        /** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.1 Для форматов ic04, ic05, icsb использовать сжатие ARGB. */
        if (icns_codes[i] == ic04 || icns_codes[i] == ic05 || icns_codes[i] == icsb) {

            /** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.1.1 Разобрать данные изображения по слоям. */
            QByteArray alpha, red, green, blue;
            for (int h = 0; h < img.height(); ++h) {
                for (int w = 0; w < img.width(); ++w) {
                    QRgb rgb = img.pixel(w, h);
                    alpha += (unsigned char)qAlpha(rgb);
                    red += (unsigned char)qRed(rgb);
                    green += (unsigned char)qGreen(rgb);
                    blue += (unsigned char)qBlue(rgb);
                }
            }
            /** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.1.2 Записать заголовок 'ARGB'. */
            array_list[i] += 'A';
            array_list[i] += 'R';
            array_list[i] += 'G';
            array_list[i] += 'B';
            /** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.1.2 Последовательно записать слои, сжатые по алгоритму ARGB. */
            array_list[i] += compressARGB(alpha);
            array_list[i] += compressARGB(red);
            array_list[i] += compressARGB(green);
            array_list[i] += compressARGB(blue);

        /** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.2 В других случаях сформировать в буфере битмап PNG */
        } else {
          QBuffer buffer(&array_list[i]);
          buffer.open(QIODevice::WriteOnly);
          img.save(&buffer, "PNG");
        }

        /**
         * &nbsp;&nbsp;&nbsp;&nbsp;2.2 Учесть размер битмапа и заголовка в общем размере файла
         */
        iheader.m_fsize += sizeof(ICNSDATA) + array_list[i].size();
    }

    /**
     * 3 Создать файл значка.
     */
    QFile icoFile(filePath);
    if (!icoFile.open(QIODevice::WriteOnly)) {
        return -1;
    }
    QDataStream out(&icoFile);

    /**
     * 4 Записать в поток заголовок ICONHDR.
     */
    out << iheader;

    /**
     * 5 Для каждого размера растра записать в поток заголовок ICNSDATA и битмап
     */
    for (size_t i = 0; i < icns_size_num; ++i) {
        ICNSDATA idata = { OSTYPE_ID[icns_codes[i]], sizeof(ICNSDATA) + array_list[i].size() };
        out << idata;
        out.writeRawData(array_list[i].constData(), array_list[i].size());
    }

    /**
     * 6 Закрыть файл значка.
     */
    icoFile.close();
    return 0;
}

/**
 * \file
 * * \copybrief compressARGB(const QByteArray&)
 */
QByteArray compressARGB(const QByteArray& data) {

  /** Алгоритм: */

  /** 1 Получить размер входных данных в байтах. */
  qsizetype data_size = data.size();

  /** 2 Инициализировать output как байтовый массив. */
  QByteArray output;

  /** 3 Перебрать входные данные по индексу. */
  for (qsizetype index = 0; index < data_size;) {
    QByteArray sequence;

    /** 3.1 Добавлять байты в sequence, пока не встретятся три одинаковых байта */
    qsizetype count;
    for(count = 0; count <= 0x7F && index < data_size; ++count, ++index) {
      if (index + 2 < data_size && data[index] == data[index+1] && data[index] == data[index+2]) {
        break;
      }
      sequence += data[index];
    }

    /** 3.2 Если в sequence есть байты, сохранить как "уникальную последовательность байт". */
    if (!sequence.isEmpty()) {
      output += count - 1;
      output += sequence;
    }

    /** 3.3 Если входные данные закончились, завершить обработку. */
    if (index >= data_size) {
      break;
    }

    /** 3.4 Определить размер последовательности одинаковых байт */
    int repeatedByte = static_cast<int>(data[index]);
    for (count = 0; count <= 0x7F && index < data_size; ++count, ++index) {
      if (static_cast<int>(data[index]) != repeatedByte) {
        break;
      }
    }

    /** 3.5 Если этот размер больше или равен трём, сохранить как "повторение байта" */
    if (count >= 3) {
      output += 0x80 + count - 3;
      output += repeatedByte;
    /** 3.5 Если этот размер меньше трёх, сохранить как "уникальную последовательность байт" */
    } else {
      output += count - 1;
      for (qsizetype i = 0; i < count; ++i) {
        output += repeatedByte;
      }
    }
  }
  return output;
}

/**
 * \TODO Учесть тонкости
 *
* Тонкости:

As of macOS 11, there are certain issues / bugs with the file format:

Setting is32+ics8 or ih32+ich8 will display a proper icon. But setting il32+icl8 ignores
the transparency mask and displays an icon without transparency.

Compressed ARGB data is not interpreted correctly. The last value of the blue channel
(aka. the very last value) is ignored and treated as if it were all zero-bytes. Usually
this is no issue since most icons will have transparency at the bottom right corner anyway.
However, it can become an issue if the last value is a repeating byte (see Compression).
Potentially, up to 130 pixels can lack the blue channel value.

A workaround is to append an additional byte at the end which is interpreted as a control
character without following data. You can compare the difference with these two examples:
        69636E73 00000024 69633034 0000001C 41524742 FFFFFBFF FF00FB00 FF00FB00 FFFFFBFF
        69636E73 00000025 69633034 0000001D 41524742 FFFFFBFF FF00FB00 FF00FB00 FFFFFBFF 00

 macOS 10.15.7 (likely earlier) and later versions have an issue displaying PNG and JPEG 2000
 icons for the keys icp4 (16x16), icp5 (32x32), and icp6 (64x64). The keys work fine in a
 standalone icns file but if used in an application, the icons are displayed completely scrambled.
 Either use the new ARGB format ic04 and ic05 (macOS 11+) or the old 24-bit RGB + alpha mask format.
 Use the latter with the old keys is32+s8mk and il32+l8mk, or with the newer keys icp4+s8mk and
 icp5+l8mk (writing RGB data into PNG fields[2]). If using ARGB image data, make sure to provide
 alternative formats for macOS 10.15 and earlier. This issue is especially tricky to detect if
 you provide both, 16x16 and 16x16@2x icons, because if you connect your Mac to a non-retina
 monitor, the non-retina 16x16 icon will be used and thus the icon will be displayed scrambled.
 The icp6 field does not seem to be used in application icons and can safely be ignored.
 Additionally, if you don't provide the smaller icon sizes at all the bug will also manifest
 when the OS scales down your larger PNG/JPEG 2000 icons, so make sure to render smaller sizes
 and include them.

Element types that deal with ARGB (32-bit) or RGB (24-bit) image formats require different types
of headers before the binary data. It is important to note that this header is part of the image
data and is not the 4-byte big endian icon element type value (e.g. ic04 or ic05).[5]

ARGB Elements
ARGB images must have their binary portion of the image data preceded by the four
byte 'ARGB' header. After that, instead of each pixel with each of its four channels stored
together (e.g. ARGBARGBARGB), an image with three pixels would be stored in individual
channels of pixel data (e.g. AAARRRGGGBBB). In addition, each channel of pixel data needs
to be encoded as mentioned below.

RGB Elements
RGB images have their binary portion of the image data preceded by four zero
byte characters only when the element type is 'it32'. In all other cases, no header is needed.
Channel data is separated as with the ARGB binary data (e.g. RRRGGGBBB instead of RGBRGBRGB).
Each channel must also be encoded as mentioned below.

Mask Elements
 Mask elements are not encoded like ARGB and RGB image color channel data. The data
is the same as that of an ARGB image except only the alpha channel data is provided. So for an
image that has two pixels, ARGBARGB, the mask data is AA.

 */