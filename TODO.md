# Планируемые доработки

### Сделать

-[ ] Сохранение файла ICNS со множественными битмапами


Можно использовать следующий подход:
```
/**
 * \brief Заголовок файла формата  ICNS
 */
class ICNSHDR {
  /**
   * \brief "Магическое" значение "icns" (0x69, 0x63, 0x6e, 0x73). 
   */
  BYTES04 m_magic;
  /**
   * \brief Размер файла в байтах.
   */
  BYTES04 m_flength;
);
/**
 * \brief Заголовок битмапа
 */
class ICNSDATA {
  /**
   * \brief Тип кодирования битмапа (enum OSTYPE). 
   */
  BYTES04 m_ostype;
  /**
   * \brief Размер данных битмапа в байтах.
   */
  BYTES04 m_flength;
);
enum OSTYPE {
ICON=0, //	128 	32×32 	1.0 	1-bit mono icon
ICN#, //	256 	32×32 	6.0 	1-bit mono icon with 1-bit mask
icm#, // 	48 	16×12 	6.0 	1 bit mono icon with 1-bit mask
icm4, // 	96 	16×12 	7.0 	4 bit icon
icm8, //	192 	16×12 	7.0 	8 bit icon
ics#, //	64 	16×16 	6.0 	1-bit mono icon with 1-bit mask
ics4, //	128 	16×16 	7.0 	4-bit icon
ics8, //	256 	16×16 	7.0 	8 bit icon
is32, //	varies1 (768) 	16×16 	8.5 	24-bit RGB icon
s8mk, //	256 	16×16 	8.5 	8-bit mask
icl4, //	512 	32×32 	7.0 	4-bit icon
icl8, //	1024 	32×32 	7.0 	8-bit icon
il32, //	varies1 (3072) 	32×32 	8.5 	24-bit RGB icon
l8mk, //	1024 	32×32 	8.5 	8-bit mask
ich#, //	576 	48×48 	8.5 	1-bit mono icon with 1-bit mask
ich4, //	1152 	48×48 	8.5 	4-bit icon
ich8, //	2304 	48×48 	8.5 	8-bit icon
ih32, //	varies1 (6912) 	48×48 	8.5 	24-bit RGB icon
h8mk, //	2304 	48×48 	8.5 	8-bit mask
it32, //	varies1 (49152 + 4)2 	128×128 	10.0 	24-bit RGB icon
t8mk, //	16384 	128×128 	10.0 	8-bit mask
icp4, //	16x16 	10.7 	JPEG 2000† or PNG† format or 24-bit RGB icon[2]
icp5, //	32x32 	10.7 	JPEG 2000† or PNG† format or 24-bit RGB icon[2]
icp6, //	48x48 	10.7 	JPEG 2000† or PNG† format
ic07, //	128x128 	10.7 	JPEG 2000 or PNG format
ic08, //	256x256 	10.5 	JPEG 2000 or PNG format
ic09, //	512x512 	10.5 	JPEG 2000 or PNG format
ic10, //	1024x1024 	10.7 	JPEG 2000 or PNG format (512x512@2x "retina" in 10.8)
ic11, // размер 32x32 	10.8 	JPEG 2000 or PNG format (16x16@2x "retina")
ic12, // размер 64x64 	10.8 	JPEG 2000 or PNG format (32x32@2x "retina")
ic13, // размер 256x256 	10.8 	JPEG 2000 or PNG format (128x128@2x "retina")
ic14, // размер 512x512 	10.8 	JPEG 2000 or PNG format (256x256@2x "retina")
ic04, //	varies1 (1024) 	16x16 		ARGB or JPEG 2000† or PNG† format
ic05, //	varies1 (4096) 	32x32 		ARGB or JPEG 2000† or PNG† format (16x16@2x "retina")
icsb, //	varies1 (1296) 	18x18 		ARGB or JPEG 2000† or PNG† format
icsB, // размер 36x36 		JPEG 2000 or PNG format (18x18@2x "retina")
sb24, // размер 24x24 		JPEG 2000 or PNG format
SB24  // размер 48x48 		JPEG 2000 or PNG format (24x24@2x "retina") 
};
const char OSTYPE_ID[][4] = {
 "ICON","ICN#","icm#","icm4","icm8","ics#","ics4","ics8","is32","s8mk","icl4","icl8","il32",
 "l8mk","ich#","ich4","ich8","ih32","h8mk","it32","t8mk","icp4","icp5","icp6","ic07","ic08",
 "ic09","ic10","ic11","ic12","ic13","ic14","ic04","ic05","icsb","icsB","sb24","SB24"
};
/* Сохранение изображения указанного формата в буфер */
QImage image;
// ...
QByteArray ba;
QBuffer buffer(&ba);
buffer.open(QIODevice::WriteOnly);
image.save(&buffer, "PNG");
```
для формирования отдельных блоков.