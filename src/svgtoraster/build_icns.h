/**
 * \file
 * \brief Заголовочный файл с классами и функциями, необходимыми для преобразования
 * векторного изображения <a href="https://doc.qt.io/qt-6/qicon.html">QIcon</a>
 * в растровый значок формата <a href="https://en.wikipedia.org/wiki/Apple_Icon_Image_format">ICNS</a> с несколькими битмапами различного размера.
 */

#ifndef BUILD_ICNS_H
#define BUILD_ICNS_H

#include <QImage>
#include <QIcon>
#include <QList>
#include <QString>
#include <QDataStream>

/**
 * \brief Индексы кодов типов битмапов, помещаемых в ICNS
 */
enum OSTYPE {
    /** 128 байт, размер 32×32, MacOS v1.0, однобитный монохромный битмап */
    ICON=0,
    /** 256 байт, размер 32×32, MacOS v6.0, однобитный монохромный битмап с однобитной маской */
    ICN,
    /** 48 байт,  размер 16×12, MacOS v6.0, однобитный монохромный битмап с однобитной маской */
    icm,
    /** 96 байт,  размер 16×12, MacOS v7.0, 4-битный битмап */
    icm4,
    /** 192 байт, размер 16×12, MacOS v7.0, 8-битный битмап */
    icm8,
    /** 64 байт,  размер 16×16, MacOS v6.0, однобитный монохромный битмап с однобитной маской */
    ics,
    /** 128 байт, размер 16×16, MacOS v7.0, 4-битный битмап */
    ics4,
    /** 256 байт, размер 16×16, MacOS v7.0, 8-битный битмап */
    ics8,
    /** размер 16×16, MacOS v8.5, 24-битный RGB битмап */
    is32,
    /** 256 байт, размер 16×16, MacOS v8.5, 8-битная маска */
    s8mk,
    /** 512 байт, размер 32×32, MacOS v7.0, 4-битный битмап */
    icl4,
    /** 1024 байт, размер 32×32, MacOS v7.0, 8-битный битмап */
    icl8,
    /** размер 32×32, MacOS v8.5, 24-битный RGB битмап */
    il32,
    /** 1024 байт, размер 32×32, MacOS v8.5, 8-битная маска */
    l8mk,
    /** 576 байт, размер 48×48, MacOS v8.5, однобитный монохромный битмап с однобитной маской */
    ich,
    /** 1152 байт, размер 48×48, MacOS v8.5, 4-битный битмап */
    ich4,
    /** 2304 байт, размер 48×48, MacOS v8.5, 8-битный битмап */
    ich8,
    /** размер 48×48, MacOS v8.5,  24-битный RGB битмап */
    ih32,
    /** размер 2304 48×48, MacOS v8.5, 8-битная маска */
    h8mk,
    /** размер 128×128, MacOS v10.0, 24-битный RGB битмап */
    it32,
    /** 16384 байт, размер 128×128, MacOS v10.0, 8-битная маска */
    t8mk,
    /** размер 16x16, MacOS v10.7, битмап JPEG 2000(X-не исп. в .app) или PNG(X) или 24-битный RGB */
    icp4,
    /** размер 32x32, MacOS v10.7, битмап JPEG 2000(X) или PNG(X) или 24-битный RGB */
    icp5,
    /** размер 64x64, MacOS v10.7, битмап JPEG 2000(X) или PNG(X) */
    icp6,
    /** размер 128x128, MacOS v10.7, битмап JPEG 2000 или PNG */
    ic07,
    /** размер 256x256, MacOS v10.5, битмап JPEG 2000 или PNG */
    ic08,
    /** размер 512x512, MacOS v10.5, битмап JPEG 2000 или PNG */
    ic09,
    /** размер 1024x1024, MacOS v10.7, битмап JPEG 2000 или PNG (512x512@2x "retina" в MacOS 10.8) */
    ic10,
    /** размер 32x32, MacOS v10.8, битмап JPEG 2000 или PNG (16x16@2x "retina") */
    ic11,
    /** размер 64x64, MacOS v10.8, битмап JPEG 2000 или PNG (32x32@2x "retina") */
    ic12,
    /** размер 256x256, MacOS v10.8, битмап JPEG 2000 или PNG (128x128@2x "retina") */
    ic13,
    /** размер 512x512, MacOS v10.8, битмап JPEG 2000 или PNG (256x256@2x "retina") */
    ic14,
    /** размер 16x16, битмап ARGB */
    ic04,
    /** размер 32x32, битмап ARGB */
    ic05,
    /** размер 18x18, битмап ARGB или JPEG 2000 или PNG */
    icsb,
    /** размер 36x36, битмап JPEG 2000 или PNG (18x18@2x "retina") */
    icsB,
    /** размер 24x24, битмап JPEG 2000 или PNG  */
    sb24,
    /** размер 48x48, битмап JPEG 2000 или PNG (24x24@2x "retina")  */
    SB24,
    /** Список заголовков ICNSDATA, включённых в файл ICNS */
    TOC,
    /** Двоичный plist. Формат: https://github.com/gerickson/opencflite/blob/main/CFBinaryPList.c */
    info,
    /** Неправильное значение */
    error = 999
};

/**
 * \brief Класс, переопределяющий четырёхбайтовое число для обеспечения необходимого
 * (первым идёт старший байт, Big-endian) порядка байт при записи в поток.
 */
class BYTES04_BE {

private:

  /**
   * \brief четырёхбайтовое целое число.
   */
  quint32 m_value;

public:

  /**
   * \brief Конструктор класса \ref BYTES04_BE "BYTES04_BE".
   *
   * param [in] value Значение (по умолчанию 0), присваиваемое экземпляру класса.
   */
  BYTES04_BE(quint32 value = 0);

  /**
   * \brief Конструктор класса \ref BYTES04_BE "BYTES04_BE" из строки.
   *
   * param [in] str Значение в виде четырёхсимвольной строки, присваиваемое экземпляру класса.
   */
  BYTES04_BE(const char *str);

  /**
   * \brief Оператор присваивания.
   *
   * param [in] value Значение, присваиваемое экземпляру класса.
   * \return ссылку на экземпляр класса.
   */
  BYTES04_BE &operator=(quint32 value);

  /**
   * \brief Оператор сложения.
   *
   * param [in] value Значение, прибавляемое к значению экземпляру класса.
   * \return ссылку на экземпляр класса.
   */
  BYTES04_BE &operator+=(quint32 value);

  /**
   * \brief Оператор приведения к типу quint32.
   *
   * \return значение экземпляра класса типа quint32.
   */
  operator quint32() const;

  /**
   * \brief Запись структуры \ref BYTES04_BE "BYTES04_BE" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param[in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
   * \param[in] b Структура \ref BYTES04_BE "BYTES04_BE", которая записывается в поток.
   * \return ссылку на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const BYTES04_BE &b);

  /**
   * \brief Чтение структуры \ref BYTES04_BE "BYTES04_BE" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param[in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param[out] b Структура \ref BYTES04_BE "BYTES04_BE", которая читается из потока.
   * \return ссылку на поток, из которого производится чтение структуры.
   */
  friend QDataStream &operator>>(QDataStream &in, BYTES04_BE &b);

};

/* Установка выравнивания в 1 бит для стркутур */
#pragma pack(push, 1)

/**
 * \brief Заголовок файла формата ICNS
 */
typedef struct tagICNSHDR {

  /**
   * \brief "Магическое" значение "icns" (0x69, 0x63, 0x6e, 0x73). 
   */
  BYTES04_BE m_magic;

  /**
   * \brief Размер файла в байтах.
   */
  BYTES04_BE m_fsize;

  /**
   * \brief Запись структуры \ref ICNSHDR "ICNSHDR" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param[in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
   * \param[in] ihdr Структура \ref ICNSHDR "ICNSHDR", которая записывается в поток.
   * \return ссылку на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const tagICNSHDR &ihdr);

  /**
   * \brief Чтение структуры \ref ICNSHDR "ICNSHDR" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param[in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param[out] ihdr Структура \ref ICNSHDR "ICNSHDR", которая читается из потока.
   * \return ссылку на поток, из которого производится чтение структуры.
   */
  friend QDataStream &operator>>(QDataStream &in, tagICNSHDR &ihdr);

} ICNSHDR;

/**
 * \brief Заголовок битмапа в составе ICNS
 */
typedef struct tagICNSDATA {

  /**
   * \brief Тип кодирования битмапа (enum OSTYPE). 
   */
  BYTES04_BE m_ostype;

  /**
   * \brief Размер данных битмапа (включая этот заголовок) в байтах.
   */
  BYTES04_BE m_dsize;

  /**
   * \brief Запись структуры \ref ICNSDATA "ICNSDATA" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param[in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
   * \param[in] ihdr Структура \ref ICNSHDR "ICNSHDR", которая записывается в поток.
   * \return ссылку на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const tagICNSHDR &ihdr);

  /**
   * \brief Чтение структуры \ref ICNSDATA "ICNSDATA" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param[in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param[out] ihdr Структура \ref ICNSHDR "ICNSHDR", которая читается из потока.
   * \return ссылку на поток, из которого производится чтение структуры.
   */
  friend QDataStream &operator>>(QDataStream &in, tagICNSHDR &ihdr);

} ICNSDATA;

/* Отмена установки выравнивания */
#pragma pack(pop)

/**
 * \brief Формирование и запись файла значка формата ICNS
 *
 * \param[in] icon векторный значок <a href="https://doc.qt.io/qt-6/qicon.html">QIcon</a>, на основе которого будет создан растровый значок.
 * \param[in] filePath строка <a href="https://doc.qt.io/qt-6/qstring.html">QString</a>, содержащая путь и имя файла создаваемого растрового значка.
 * \param[in] sizes Список <a href="https://doc.qt.io/qt-6/qstringlist.html">QStringList</a>, содержащий требуемые размеры битмапов значка.
 * \retval 0 если значок успешно создан.
 * \retval -1 в случае ошибки.
 */
int saveIcns(const QIcon &icon, const QString &filePath, const QStringList& sizes);

/**
 * \brief Сжатие последовательности байт по алгоритму ARGB
 *
 * \param[in] data входная последовательность байт.
 * \return последовательность байт, сжатую по алгоритму ARGB.
 */
QByteArray compressARGB(const QByteArray& data);


/**
 * \brief Определение формата битмапа по требуемому размеру и удвоению
 *
 * \param[in] size <a href="https://doc.qt.io/qt-6/qstring.html">строка</a>, содержащая параметр требуемый размер битмапа.
 * \return код, определяющий формат битмапа.
 */
enum OSTYPE getCode(const QString &size);

#endif // BUILD_ICNS_H
