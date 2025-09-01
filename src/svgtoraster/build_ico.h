#include <QString>
#include <QDataStream>


/** Размер в байтах блока данных цветового представления B-G-R */
#define RGB_SIZE 3

/** Размер в байтах блока данных цветового представления B-G-R-A */
#define RGBA_SIZE 4

/** Количество байт, на которое должна быть выравнена длина строки файла BMP */
#define BMP_BYTE_ALIGN 4

/** Количество бит в байте */
#define BITS_IN_BYTE 8

class BYTES02 {

private:

  /**
   * \brief двухбайтовое целое число.
   */
  quint16 m_value;

public:

  BYTES02(quint16 value = 0);

  BYTES02 &operator=(quint16 value);

  operator quint16() const;

  friend QDataStream &operator<<(QDataStream &out, const BYTES02 &b);

  friend QDataStream &operator>>(QDataStream &in, BYTES02 &b);

};

class BYTES04 {

private:

  /**
   * \brief четырёхбайтовое целое число.
   */
  quint32 m_value;

public:

  BYTES04(quint32 value = 0);

  BYTES04 &operator=(quint32 value);

  operator quint32() const;

  friend QDataStream &operator<<(QDataStream &out, const BYTES04 &b);

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
   * \param [in] icondir Структура \ref BITMAPINFOHEADER "", которая записывается в поток.
   * \return ссылка на поток, в который производится запись структуры.
   */
  friend QDataStream &operator<<(QDataStream &out, const tagBITMAPINFOHEADER &ihdr);

  /**
   * \brief Чтение структуры \ref BITMAPINFOHEADER "BITMAPINFOHEADER" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
   *
   * \param [in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
   * \param [out] icondir Структура \ref BITMAPINFOHEADER "BITMAPINFOHEADER", которая читается из потока.
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
   * \brief Несжатый двумерный массив, BitCount не равно нулю.
   */
  BI_RGB = 0,
  /**
   * \brief Сжатие RLE, BitCount равно 8.
   */
  BI_RLE8 = 1,
  /**
   * \brief Сжатие RLE, BitCount равно 4.
   */
  BI_RLE4 = 2,
  /**
   * \brief Несжатый двумерный массив с масками цветовых каналов,
   * BitCount равно 16 или 32.
   */
  BI_BITFIELDS = 3,
  /**
   * \brief Встроенный файл JPEG, BitCount равно нулю.
   */
  BI_JPEG = 4,
  /**
   * \brief Встроенный файл PNG, BitCount равно нулю.
   */
  BI_PNG = 5,
  /**
   * \brief Несжатый двумерный массив с масками цветовых и альфа-канала,
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
 * \param [in] icondir Структура \ref ICONDIRENTRY "ICONDIRENTRY", которая записывается в поток.
 * \return ссылка на поток, в который производится запись структуры.
 */
friend QDataStream &operator<<(QDataStream &out, const tagICONDIRENTRY &icondirentry);

/**
 * \brief Чтение структуры \ref ICONDIRENTRY "ICONDIRENTRY" из <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>
 *
 * \param [in] in Поток <a href="https://doc.qt.io/qt-6/qdatastream.html">QDataStream</a>, из которого производится чтение структуры.
 * \param [out] icondir Структура \ref ICONDIRENTRY "ICONDIRENTRY", которая читается из потока.
 * \return ссылка на поток, из которого производится чтение структуры.
 */
friend QDataStream &operator>>(QDataStream &in, tagICONDIRENTRY &icondirentry);

} ICONDIRENTRY;

/* Отмена установки выравнивания */
#pragma pack(pop)

int saveIco(const QIcon &icon, const QString &filePath, const QList<int> &sizes);
