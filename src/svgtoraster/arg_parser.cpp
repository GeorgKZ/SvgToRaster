/**
 * \file
 * \brief Файл с определениями функций-членов класса \ref arg_parser "arg_parser"
 */

#include "arg_parser.h"

/**
 * \file
 * Функции, являющиеся методами класса \ref arg_parser "arg_parser":
 * <BR>
 */

/**
 * \file
 * * \copybrief arg_parser::arg_parser()
 */
arg_parser::arg_parser()
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus >= 201103L)
  = default;
#else
  {}
#endif

/**
 * \file
 * * \copybrief arg_parser::get_flag(qsizetype) const
 */
const QString& arg_parser::get_flag(qsizetype index) const {
  return args.at(index).flag;
}

/**
 * \file
 * * \copybrief arg_parser::get_parameters(qsizetype) const
 */
QString arg_parser::get_parameters(qsizetype index) const {
  const QStringList& a = args.at(index).parameters;
  QString str_parameters;
  for (auto it = a.begin(); it != a.end(); ++it) {
    if (it != a.begin()) {
      str_parameters += " ";
    }
    str_parameters += *it;
  }
  return str_parameters;
}

/**
 * \file
 * * \copybrief arg_parser::get_parameters_set(qsizetype) const
 */
const QStringList& arg_parser::get_parameters_set(qsizetype index) const {
  return args.at(index).parameters;
}

/**
 * \file
 * * \copybrief arg_parser::get_arg_num() const
 */
qsizetype arg_parser::get_arg_num() const {
  return args.size();
}

/**
 * \file
 * * \copybrief arg_parser::add_new_flag(const QString&)
 */
void arg_parser::add_new_flag(const QString &flag) {
  argument new_arg;
  new_arg.flag = flag;
  if (flag.startsWith("--")) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    new_arg.flag.slice(2);
#else
    new_arg.flag = new_arg.flag.sliced(2);
#endif
  }
  args += new_arg;
}

/**
 * \file
 * * \copybrief arg_parser::add_new_param(const QString&)
 */
void arg_parser::add_new_param(const QString &param) {

  if (args.isEmpty()) {
    add_new_flag("");
  }

  args.last().parameters += param;
}

/**
 * \file
 * * \copybrief arg_parser::process_cmdline(const char*[], int)
 */
int arg_parser::process_cmdline(const char *cmdline[], int cmdnum) {

  /** Алгоритм: */

  /**
   * 1 Разделить аргументы по пробелам, так как в одном строковом аргументе могут оказаться
   * несколько аргументов, разделённых пробелами.
   */
  QStringList processed_cmdline;

  for (int argi = 0; argi < cmdnum; argi++) {
    QString curr_str;
    for (size_t i = 0; i < strlen(cmdline[argi]); ++i) {
      if (cmdline[argi][i] != ' ') {
        curr_str += cmdline[argi][i];
      } else {
        if (!curr_str.isEmpty()) {
          processed_cmdline += curr_str;
          curr_str.clear();
        }
      }
    }
    if (!curr_str.isEmpty()) {
      processed_cmdline += curr_str;
    }
  }
  qsizetype processed_cmdnum = processed_cmdline.size();

  /**
   * 2 Проверить: если в командной строке нет аргументов (или есть только нулевой
   * агрумент - сама программа), то это ошибка.
   */
  if (processed_cmdnum < 2) {
    return -2;
  }

  /** 3 Проверить: если командная строка не начинается с флага, то это ошибка. */
  if (!processed_cmdline[1].startsWith("--")) {
    add_new_flag(processed_cmdline[1]);
    return -1;
  }

  /** 4 Проверить: если командная строка начинается с пустого флага, то это ошибка. */
  if (processed_cmdline[1].size() == 2) {
    add_new_flag(processed_cmdline[1]);
    return -3;
  }

  /** 5 Добавить первый флаг в список обнаруженных флагов. */
  add_new_flag(processed_cmdline[1]);

  /** 6 Перебрать оставшиеся аргументы. */
  for (qsizetype argi = 2; argi < processed_cmdnum; argi++) {
    if (processed_cmdline[argi].startsWith("--")) {

      /**
       * &nbsp;&nbsp;&nbsp;&nbsp;6.1 Если аргументом окажется флаг, добавить его в список
       * обнаруженных флагов и продолжить перебирать аргументы.
       */
      add_new_flag(processed_cmdline[argi]);
      continue;
    }
    /** &nbsp;&nbsp;&nbsp;&nbsp;6.2 Иначе добавить аргумент как параметр текущего флага. */
    add_new_param(processed_cmdline[argi]);
  }
  return 0;
}
