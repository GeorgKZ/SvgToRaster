/**
 * \file
 * \brief Заголовочный файл с объявлением класса \ref arg_parser "arg_parser",
 * предназначенного для разбора командной строки 
 */

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <QString>
#include <QList>

/**
 * \brief Класс, реализующий простой разбор командной строки, состоящей из конструкций
 * ("аргументов") типа флаг/параметры "--flag [param ...]"
 * \details Флаг предваряют два символа минус ('-'), а не один, так как параметром флага
 * может являться отрицательное число, также начинающееся с символа минус.
 */
class arg_parser {

  private:

  /**
   * \brief Класс, реализующий вспомогательную структуру из \ref arg_parser::argument::flag "флага" (ключевого слова)
   * и <a href="https://doc.qt.io/qt-6/qlist.html">списка</a> \ref arg_parser::argument::parameters "параметров"
   */
  class argument {

    public:

    /** \brief Флаг (ключевое слово) для параметров в виде <a href="https://doc.qt.io/qt-6/qstring.html">строки</a> */
    QString flag;

    /** \brief <a href="https://doc.qt.io/qt-6/qstringlist.html">Список</a> параметров--<a href="https://doc.qt.io/qt-6/qstring.html">строк</a> */
    QStringList parameters;
  };

  /** 
   * \brief <a href="https://doc.qt.io/qt-6/qlist.html">Список</a> разобранных \ref arg_parser::argument "аргументов"
   */
  QList<argument> args;

  /** 
   * \brief Добавить новый \ref arg_parser::argument "аргумент", установив значение \ref arg_parser::argument::flag "флага"
   *
   * \param[in] flag <a href="https://doc.qt.io/qt-6/qstring.html">строка</a>, содержащая флаг (ключевое слово)
   */
  void add_new_flag(const QString &flag);

  /** 
   * \brief Дополнить \ref arg_parser::argument::parameters "параметр" последнего \ref arg_parser::argument "аргумента"
   *
   * \param[in] param <a href="https://doc.qt.io/qt-6/qstring.html">строка</a>, содержащая параметр
   */
  void add_new_param(const QString &param);

  public:

  /** 
   * \brief Конструктор по умолчанию
   */
  arg_parser();

  /** 
   * \brief Обработать командную строку
   *
   * \param[in] cmdline массив строк аргументов командной строки
   * \param[in] cmdnum количество аргументов командной строки
   * \retval -1 если первый аргумент командной строки не является флагом (ключевым словом),
   * то есть не начинается с символов '--';
   * \retval 0 в случае успешного завершения.
   */
  int process_cmdline(const char *cmdline[], int cmdnum);

  /** 
   * \brief Получить \ref arg_parser::argument::flag "флаг" указанного \ref arg_parser::argument "аргумента"
   *
   * \param[in] index номер (индекс) \ref argument "аргумента"
   * \return значение \ref arg_parser::argument::flag "флага" указанного \ref argument "аргумента" в виде <a href="https://doc.qt.io/qt-6/qstring.html">строки</a>.
   */
  const QString& get_flag(qsizetype index) const;

  /** 
   * \brief Получить \ref arg_parser::argument::parameters "параметры" указанного \ref arg_parser::argument "аргумента" в виде <a href="https://doc.qt.io/qt-6/qstring.html">строки</a>.
   *
   * \param[in] index номер (индекс) \ref argument "аргумента"
   * \return \ref arg_parser::argument::parameters "параметры" указанного \ref arg_parser::argument "аргумента" через пробел, объединённые в <a href="https://doc.qt.io/qt-6/qstring.html">строку</a>.
   */
  QString get_parameters(qsizetype index) const;

  /** 
   * \brief Получить набор \ref arg_parser::argument::parameters "параметров" указанного \ref arg_parser::argument "аргумента" в виде <a href="https://doc.qt.io/qt-6/qstringlist.html">списка строк</a>
   *
   * \param[in] index номер (индекс) \ref argument "аргумента"
   * \return arg_parser::argument::parameters "параметры" указанного \ref arg_parser::argument "аргумента" в виде <a href="https://doc.qt.io/qt-6/qstringlist.html">списка строк</a>.
   */
  const QStringList& get_parameters_set(qsizetype index) const;

  /** 
   * \brief Получить количество \ref arg_parser::argument "аргументов"
   *
   * \return количество \ref arg_parser::argument "аргументов"
   */
  qsizetype get_arg_num() const;
};

#endif // ARG_PARSER_H
