/**
 * \file
 *
 * \brief Файл с определениями функций-членов класса arg_parser
 *
 * <BR>
 */

#include "stdafx.h"
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
 * * \copybrief arg_parser::get_flag(size_t) const
 */
const std::string& arg_parser::get_flag(size_t index) const {
  return args[index].flag;
}

/**
 * \file
 * * \copybrief arg_parser::get_parameters(size_t) const
 */
const std::string& arg_parser::get_parameters(size_t index) const {
  return args[index].str_parameters;
}

/**
 * \file
 * * \copybrief arg_parser::get_parameters_set(size_t) const
 */
const std::vector<std::string>& arg_parser::get_parameters_set(size_t index) const {
  return args[index].parameters;
}

/**
 * \file
 * * \copybrief arg_parser::get_arg_num() const
 */
size_t arg_parser::get_arg_num() const {
  return args.size();
}

/**
 * \file
 * * \copybrief arg_parser::add_new_flag(const char*)
 */
void arg_parser::add_new_flag(const char *flag) {
  argument new_arg;
  new_arg.flag = &flag[2];
  args.push_back(new_arg);
}

/**
 * \file
 * * \copybrief arg_parser::add_new_param(const char*)
 */
void arg_parser::add_new_param(const char *param) {

  if (args.empty()) {
    add_new_flag("");
  }

  argument& curr_arg = args[args.size() - 1];
  if (!curr_arg.str_parameters.empty()) {
    curr_arg.str_parameters += std::string(" ");
  }
  curr_arg.str_parameters += std::string(param);
  curr_arg.parameters.
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus >= 201103L)
                      emplace_back
#else
                      push_back
#endif
                               (std::string(param));
}

/**
 * \file
 * * \copybrief arg_parser::process_cmdline(const char*[], size_t)
 */
int arg_parser::process_cmdline(const char *cmdline[], size_t cmdnum) {

  // Если командная строка не начинается с флага, это ошибка
  if (strlen(cmdline[1]) < 2 || cmdline[1][0] != '-' || cmdline[1][1] != '-') {
    return -1;
  }

  // Добавить флаг
  add_new_flag(cmdline[1]);

  for (size_t argi = 2; argi < cmdnum; argi++) {
    if (strlen(cmdline[argi])>= 2 && cmdline[argi][0] == '-' && cmdline[argi][1] == '-') {
      // Добавить флаг и остаться в режиме чтения параметров
      add_new_flag(cmdline[argi]);
      continue;
    }
    add_new_param(cmdline[argi]);
  }
  return 0;
}
