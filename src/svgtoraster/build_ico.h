/**
 * \file
 *
 * \brief Заголовочный файл с классами и функциями, необходимыми для преобразования
 * векторного изображения <a href="https://doc.qt.io/qt-6/qicon.html">QIcon</a>
 * в растровый значок формата ICO с несколькими битмапами различного размера.
 */

#ifndef BUILD_ICO_H
#define BUILD_ICO_H

#include <QImage>
#include <QIcon>
#include <QList>
#include <QString>
#include <QDataStream>

/** \brief Размер в байтах блока данных цветового представления B-G-R */
#define RGB_SIZE 3

/** \brief Размер в байтах блока данных цветового представления B-G-R-A */
#define RGBA_SIZE 4

/** \brief Количество байт, на которое должна быть выравнена длина строки файла BMP */
#define BMP_BYTE_ALIGN 4

/** \brief Количество бит в байте */
#define BITS_IN_BYTE 8

/**
 * \brief Класс, переопределяющий двухбайтовое число для обеспечения необходимого
 * (первым идёт младший байт) порядка байт при записи в поток.
 */
class BYTES02 {

private:

  /**
   * \brief двухбайтовое целое число.
   */
  quint16 m_value;

public:

  /**
   * \brief Конструктор класса \ref BYTES02 "BYTES02".
   *
   * param [in] value Значение (по умолчанию 0), присваиваемое экземпляру класса.
   */
  BYTES02(quint16 value = 0);

  /**
   * \brief Оператор присваивания.
   *
   * param [in] value Значение, присваиваемое экземпляру класса.
   * \return ссылка на экземпляр класса.
   */
  BYTES02 &operator=(quint16 value);

  /**
   * \brief Оператор сложения.
   *
   * param [in] value Значение, прибавляемое к значению экземпляру класса.
   * \return ссылка на экземпляр класса.
   */
  BYTES02 &operator+=(quint16 value);

  /**
   * \brief Оператор приведения к типу quint16.
   *
   * \return значение экземпляра класса типа quint16.
   */
  operator quint16() const;

  /**
   * \brief Запись структуры \ref BYTES02 "BYTES02" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
   * \param [in] b Структура \ref BYTES02 "BYTES02", которая записывается в поток.
   * \return ссылка на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const BYTES02 &b);

  /**
   * \brief Чтение структуры \ref BYTES02 "BYTES02" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param [out] b Структура \ref BYTES02 "BYTES02", которая читается из потока.
   * \return ссылка на поток, из которого производится чтение структуры.
   */
  friend QDataStream &operator>>(QDataStream &in, BYTES02 &b);

};

/**
 * \brief Класс, переопределяющий четырёхбайтовое число для обеспечения необходимого
 * (первым идёт младший байт) порядка байт при записи в поток.
 */
class BYTES04 {

private:

  /**
   * \brief четырёхбайтовое целое число.
   */
  quint32 m_value;

public:

  /**
   * \brief Конструктор класса \ref BYTES04 "BYTES04".
   *
   * param [in] value Значение (по умолчанию 0), присваиваемое экземпляру класса.
   */
  BYTES04(quint32 value = 0);

  /**
   * \brief Оператор присваивания.
   *
   * param [in] value Значение, присваиваемое экземпляру класса.
   * \return ссылка на экземпляр класса.
   */
  BYTES04 &operator=(quint32 value);

  /**
   * \brief Оператор сложения.
   *
   * param [in] value Значение, прибавляемое к значению экземпляру класса.
   * \return ссылка на экземпляр класса.
   */
  BYTES04 &operator+=(quint32 value);

  /**
   * \brief Оператор приведения к типу quint32.
   *
   * \return значение экземпляра класса типа quint32.
   */
  operator quint32() const;

  /**
   * \brief Запись структуры \ref BYTES04 "BYTES04" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
   * \param [in] b Структура \ref BYTES04 "BYTES04", которая записывается в поток.
   * \return ссылка на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const BYTES04 &b);

  /**
   * \brief Чтение структуры \ref BYTES04 "BYTES04" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param [out] b Структура \ref BYTES04 "BYTES04", которая читается из потока.
   * \return ссылка на поток, из которого производится чтение структуры.
   */
  friend QDataStream &operator>>(QDataStream &in, BYTES04 &b);

};

/* Установка выравнивания в 1 бит для стркутур */
#pragma pack(push, 1)

/**
 * \brief Информационный заголовок BMP, версия 3
 */
typedef struct tagBITMAPINFOHEADER {

  /**
   * \brief Размер данной структуры в байтах, указывающий также на версию структуры.
   */
  BYTES04 biSize;

  /**
   * \brief Ширина растра в пикселях. Указывается целым числом со знаком.
   * Нулевое и отрицательные значения не документированы.
   */
  BYTES04 biWidth;

  /**
   * \brief Высота растра в пикселях. Указывается целым числом со знаком.
   * Нулевое и отрицательные значения не документированы.
   */
  BYTES04 biHeight;

  /**
   * \brief В файле BMP допустимо только значение, равное единице. Может использоваться
   * в файлах значков и курсоров.
   */
  BYTES02 biPlanes;

  /**
   * \brief Количество бит на пиксель.
   */
  BYTES02 biBitCount;

  /**
   * \brief Указывает на способ хранения пикселей.
   */
  BYTES04 biCompression;

  /**
   * \brief Размер пиксельных данных в байтах. Может быть равно нулю,
   * если хранение осуществляется двумерным массивом без сжатия.
   */
  BYTES04 biSizeImage;

  /**
   * \brief Количество пикселей на метр по горизонтали или 0.
   */
  BYTES04 biXPelsPerMeter;

  /**
   * \brief Количество пикселей на метр по вертикали или 0.
   */
  BYTES04 biYPelsPerMeter;

  /**
   * \brief Размер таблицы цветов в ячейках.
   */
  BYTES04 biClrUsed;
  /**
   * \brief Количество ячеек от начала таблицы цветов до последней используемой (включая её саму).
   */
  BYTES04 biClrImportant;

  /**
   * \brief Запись структуры \ref BITMAPINFOHEADER "BITMAPINFOHEADER" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
   * \param [in] ihdr Структура \ref BITMAPINFOHEADER "BITMAPINFOHEADER", которая записывается в поток.
   * \return ссылка на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const tagBITMAPINFOHEADER &ihdr);

  /**
   * \brief Чтение структуры \ref BITMAPINFOHEADER "BITMAPINFOHEADER" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param [out] ihdr Структура \ref BITMAPINFOHEADER "BITMAPINFOHEADER", которая читается из потока.
   * \return ссылка на поток, из которого производится чтение структуры.
   */
  friend QDataStream &operator>>(QDataStream &in, tagBITMAPINFOHEADER &ihdr);

} BITMAPINFOHEADER;

/**
 * \brief Способ хранения пикселей, указанный в \ref BITMAPINFOHEADER "информационном заголовке BMP"
 * в поле biCompression
 */
enum biCompression {
  /**
   * Несжатый двумерный массив, BitCount не равно нулю.
   */
  BI_RGB = 0,
  /**
   * Сжатие RLE, BitCount равно 8.
   */
  BI_RLE8 = 1,
  /**
   * Сжатие RLE, BitCount равно 4.
   */
  BI_RLE4 = 2,
  /**
   * Несжатый двумерный массив с масками цветовых каналов,
   * BitCount равно 16 или 32.
   */
  BI_BITFIELDS = 3,
  /**
   * Встроенный файл JPEG, BitCount равно нулю.
   */
  BI_JPEG = 4,
  /**
   * Встроенный файл PNG, BitCount равно нулю.
   */
  BI_PNG = 5,
  /**
   * Несжатый двумерный массив с масками цветовых и альфа-канала,
   * BitCount равно 16 или 32.
   */
  BI_ALPHABITFIELDS = 6
};

/**
 * \brief Заголовок файлов значка и курсора
 */
typedef struct tagICONDIR {

  /**
   * \brief Значение зарезервировано и должно содержать ноль. 
   */
  BYTES02 ihReserved;

  /**
   * \brief Тип файла, принимает значение 1 для значка и 2 для курсора.
   */
  BYTES02 ihType;

  /**
   * \brief Количество изображений в файле, не меньше единицы.
   */
  BYTES02 ihCount;

  /**
   * \brief Запись структуры \ref ICONDIR "ICONDIR" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
   * \param [in] icondir Структура \ref ICONDIR "ICONDIR", которая записывается в поток.
   * \return ссылка на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const tagICONDIR &icondir);

  /**
   * \brief Чтение структуры \ref ICONDIR "ICONDIR" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param [out] icondir Структура \ref ICONDIR "ICONDIR", которая читается из потока.
   * \return ссылка на поток, из которого производится чтение структуры.
   */
  friend QDataStream &operator>>(QDataStream &in, tagICONDIR &icondir);

} ICONDIR;

/**
 * \brief Каталог информации об изображениях
 */
typedef struct tagICONDIRENTRY {

  /**
   * \brief Ширина изображения в пикселях, 0 означает ширину 256 или более пикселей.
   */
  quint8 iiWidth;

  /**
   * \brief Высота изображения в пикселях, 0 означает высоту 256 или более пикселей.
   */
  quint8 iiHeight;

  /**
   * \brief Указывает количество цветов в палитре изображения.
   * Для полноцветных (RGB, RGBA без палитры) файлов значка должно иметь значение 0.
   */
  quint8 iiColors;

  /**
   * \brief Значение зарезервировано и должно содержать ноль. 
   */
  quint8 iiReserved;

  /**
   * \brief В файле значка определяет количество цветовых плоскостей, может иметь
   * значение равно 0 или 1. В файле курсора определяет горизонтальную координату
   * «горячей точки» в пикселях относительно левого края изображения.
   */
  BYTES02 iiPlanes;

  /**
   * \brief В файле значка определяет количество битов на пиксель, может иметь значение 0 или больше.
   * В файле курсора определяет горизонтальную координату «горячей точки» в пикселях
   * относительно левого края изображения.
   */
  BYTES02 iiBpp;

  /**
   * \brief Указывает размер растра в байтах.
   */
  BYTES04 iiSize;

  /**
   * \brief Указывает абсолютное смещение структуры BITMAPINFOHEADER или данных PNG в файле.
   */
  BYTES04 iiOffset;

/**
 * \brief Запись структуры \ref ICONDIRENTRY "ICONDIRENTRY" в <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 *
 * \param [in] out Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, в который производится запись структуры.
 * \param [in] icondirentry Структура \ref ICONDIRENTRY "ICONDIRENTRY", которая записывается в поток.
 * \return ссылка на поток, в который производится запись структуры.
 */
friend QDataStream &operator<<(QDataStream &out, const tagICONDIRENTRY &icondirentry);

/**
 * \brief Чтение структуры \ref ICONDIRENTRY "ICONDIRENTRY" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 *
 * \param [in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
 * \param [out] icondirentry Структура \ref ICONDIRENTRY "ICONDIRENTRY", которая читается из потока.
 * \return ссылка на поток, из которого производится чтение структуры.
 */
friend QDataStream &operator>>(QDataStream &in, tagICONDIRENTRY &icondirentry);

} ICONDIRENTRY;

/* Отмена установки выравнивания */
#pragma pack(pop)

/**
 * \brief Формирование и запись файла значка формата ICO
 *
 * \param [in] icon векторный значок <a href="https://doc.qt.io/qt-6/qicon.html">QIcon</a>, на основе которого будет создан растровый значок.
 * \param [in] filePath строка <a href="https://doc.qt.io/qt-6/qstring.html">QString</a>, содержащая путь и имя файла создаваемого растрового значка.
 * \param [in] sizes Список <a href="https://doc.qt.io/qt-6/qlist.html">QList</a>, содержащий требуемые размеры битмапов значка.
 * \retval 0 если значок успешно создан.
 * \retval -1 в случае ошибки.
 */
int saveIco(const QIcon &icon, const QString &filePath, const QList<int> &sizes);

#endif // BUILD_ICO_H
