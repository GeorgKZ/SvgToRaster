/**
 * \file
 *
 * \brief Заголовочный файл с объявлением класса \ref arg_parser "arg_parser",
 * предназначенного для разбора командной строки 
 */

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <QtCore/QString>
#include <QtCore/QList>

/**
 * \brief Класс, реализующий простой разбор командной строки, состоящей из конструкций
 * ("аргументов") типа "--flag [param ...]"
 */
class arg_parser {

  private:

  /**
   * \brief Класс, реализующий вспомогательную структуру из двух строк ---
   * флага (ключевого слова) и параметров
   */
  class argument {

    public:

    /** \brief Ключевое слово для параметров в виде <a href="https://doc.qt.io/qt-6/qstring.html">строки</a> */
    QString flag;

    /** \brief <a href="https://doc.qt.io/qt-6/qlist.html">Список</a> параметры по отдельности */
    QStringList parameters;

    /** \brief Параметры в виде одной <a href="https://doc.qt.io/qt-6/qstring.html">строки</a> */
    QString str_parameters;
  };

  /** 
   * \brief <a href="https://doc.qt.io/qt-6/qlist.html">Список</a> разобранных аргументов
   */
  QList<argument> args;

  /** 
   * \brief Добавить новый аргумент, установив значение флага
   *
   * \param [in] flag строка-флаг (ключевое слово)
   */
  void add_new_flag(const char *flag);

  /** 
   * \brief Дополнить параметр последнего аргумента
   *
   * \param [in] param строка-параметр
   */
  void add_new_param(const char *param);

  public:

  /** 
   * \brief Конструктор по умолчанию
   */
  arg_parser();

  /** 
   * \brief Обработать командную строку
   *
   * \param [in] cmdline массив строк аргументов командной строки
   * \param [in] cmdnum количество аргументов командной строки
   * \retval -1 если первый аргумент командной строки не является флагом (ключевым словом),
   * то есть не начинается с символов '--';
   * \retval 0 в случае успешного завершения.
   */
  int process_cmdline(const char *cmdline[], int cmdnum);

  /** 
   * \brief Получить флаг указанного аргумента
   *
   * \param [in] index номер аргумента
   * \return значение флага в виде <a href="https://doc.qt.io/qt-6/qstring.html">строки</a>.
   */
  const QString& get_flag(int index) const;

  /** 
   * \brief Получить параметры указанного аргумента в виде <a href="https://doc.qt.io/qt-6/qstring.html">строки</a>.
   *
   * \param [in] index номер аргумента
   * \return параметры указанного аргумента в виде <a href="https://doc.qt.io/qt-6/qstring.html">строки</a>.
   */
  const QString& get_parameters(int index) const;

  /** 
   * \brief Получить набор параметров указанного аргумента в виде <a href="https://doc.qt.io/qt-6/qstringlist.html">списка строк</a>
   *
   * \param [in] index номер аргумента
   * \return строку, содержащую параметры указанного аргумента в виде <a href="https://doc.qt.io/qt-6/qstringlist.html">списка строк</a>.
   */
  const QStringList& get_parameters_set(int index) const;

  /** 
   * \brief Получить количество аргументов
   *
   * \return количество аргументов
   */
  int get_arg_num() const;
};

#endif // ARG_PARSER_H
