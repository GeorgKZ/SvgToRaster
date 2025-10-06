##############################################################################
# Настройка требуемой версии CMake
##############################################################################

  cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Настройка окружения
##############################################################################

  include(build_set_builddir.cmake)

##############################################################################
# Настройка шаблона журналирования для Qt
##############################################################################

  # Вывод всех сообщений с указанием даты, времени, файла, строки
  set(MESSAGE_PATTERN_ALL "[%{time dd.MM.yyyy h:mm:ss.zzz}] %{if-debug}DEBUG%{endif}%{if-info}INFO%{endif}%{if-warning}WARNING%{endif}%{if-critical}CRITICAL%{endif}%{if-fatal}FATAL%{endif} %{file}:%{line} - %{message}")
  # Вывод только ошибок CRITICAL и FATAL
  set(MESSAGE_PATTERN_ERROR "%{if-critical}CRITICAL %{message}%{endif}%{if-fatal}FATAL %{message}%{endif}")
  # Вывод ошибок CRITICAL, FATAL и предупреждений WARNING
  set(MESSAGE_PATTERN_WARNING "%{if-critical}CRITICAL %{message}%{endif}%{if-fatal}FATAL %{message}%{endif}%{if-warning}WARNING %{message}%{endif}")
  # Вывод ошибок CRITICAL, FATAL, предупреждений WARNING и информационных сообщений INFO
  set(MESSAGE_PATTERN_INFO "%{if-critical}CRITICAL %{message}%{endif}%{if-fatal}FATAL %{message}%{endif}%{if-warning}WARNING %{message}%{endif}%{if-info}INFO %{message}%{endif}")
  # Вывод ошибок CRITICAL, FATAL, предупреждений WARNING, информационных сообщений INFO и трассировочных сообщения DEBUG
  set(MESSAGE_PATTERN_DEBUG "%{if-critical}CRITICAL %{message}%{endif}%{if-fatal}FATAL %{message}%{endif}%{if-warning}WARNING %{message}%{endif}%{if-info}INFO %{message}%{endif}%{if-debug}DEBUG %{message}%{endif}")

  # При компиляции можно указать QT_NO_DEBUG_OUTPUT, QT_NO_INFO_OUTPUT, or QT_NO_WARNING_OUTPUT
  set(ENV{QT_MESSAGE_PATTERN} "${MESSAGE_PATTERN_INFO}")

##############################################################################
# Тестовые запуски программы
##############################################################################

  set(TEST_RESULTS "${BUILDDIR}/test_results")
  file(MAKE_DIRECTORY "${TEST_RESULTS}")

  include(../Custom.cmake)

  # Имя проекта, приведённое к нижнему регистру
  string(TOLOWER ${CUSTOM_PROJECT_NAME} PROJECT_NAME_LOWERCASE)

  # Настройки Valgrind
  if(DEFINED DEBUG_VALGRIND) 
      # Настройки Valgrind
      # set(VALGRIND_LEAKS --leak-check=full --show-leak-kinds=all --leak-resolution=high)
      # set(VALGRIND_UNDEFS --undef-value-errors=yes --track-origins=yes)
      set(VALGRIND_MAIN valgrind --show-error-list=yes --keep-debuginfo=yes ${VALGRIND_LEAKS} ${VALGRIND_UNDEFS})
  endif()

  # Вывод информации о работе с плюгинами Qt
  if(DEFINED DEBUG_QT_PLUGINS) 
      set(ENV{QT_DEBUG_PLUGINS} 1)
  endif()

  # для Windows
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
      set(CMD_PREFIX "cmd /c")
  # для MacOS
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
      set(CMD_PREFIX "open -a")
      set(CMD_SUFFIX ".app")
      set(CMD_POSTFIX "--stdout ${BUILDDIR}/log_run.txt --args")
  # для Linux
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
      set(CMD_PREFIX "${VALGRIND_MAIN}")
      set(CMD_PATH "/bin")
  endif()

  # Перебрать все тестовые последовательности
  list(LENGTH TEST_ARGUMENTS_LIST LEN)
  math(EXPR LEN "${LEN} - 1")
  foreach(index1 RANGE 0 ${LEN} 2)
      math(EXPR index2 "${index1} + 1") 
      math(EXPR num "${index1} / 2") 
      # Получить строку комментария для теста
      list(GET TEST_ARGUMENTS_LIST ${index1} CMNT)
      # Получить строку аргументов для теста
      list(GET TEST_ARGUMENTS_LIST ${index2} ARG)

      # Создать скрипт для выполнения команды
      file(WRITE
          "${BUILDDIR}/run_${num}.cmake" 

          "execute_process(\n"
          "  COMMAND ${CMD_PREFIX} ${BUILDDIR}/install${CMD_PATH}/${PROJECT_NAME_LOWERCASE}${CMD_SUFFIX} ${CMD_POSTFIX} ${ARG}\n"
          "  RESULT_VARIABLE result\n"
          "  OUTPUT_VARIABLE output\n"
          "  ERROR_VARIABLE  output_error\n"
          ")\n"
          "file(REMOVE \"${BUILDDIR}/log_run_${num}.txt\")\n"
          "file(REMOVE \"${BUILDDIR}/log_run_${num}_errors.txt\")\n"
          "if(NOT \"^^\${output}\" STREQUAL \"^^\")\n"
          "    file(WRITE \"${BUILDDIR}/log_run_${num}.txt\" \"\${output}\")\n"
          "endif()\n"
          "if(NOT \"^^\${output_error}\" STREQUAL \"^^\")\n"
          "    file(WRITE \"${BUILDDIR}/log_run_${num}_errors.txt\" \"\${output_error}\")\n"
          "endif()\n"
          "if (result EQUAL 0)\n"
          "    message(STATUS \"Run: Выполнение успешно завершено.\")\n"
          "else()\n"
          "    message(STATUS \"Run: Выполнение завершено с ошибкой \${result}.\")\n"
          "endif()\n"
      )
      # Комментарий
      message(STATUS "${CMNT}")
      # Выполнить скрипт с командой
      execute_process(
        COMMAND ${CMAKE_COMMAND} -P "${BUILDDIR}/run_${num}.cmake"
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
      )

      # Удалить скрипт для выполнения команды
      file(REMOVE "${BUILDDIR}/run_${num}.cmake")

      file(REMOVE "${BUILDDIR}/log_runX_${num}_errors.txt")
      if(NOT "^^${output_error}" STREQUAL "^^")
          file(WRITE "${BUILDDIR}/log_runX_${num}_errors.txt" "${output_error}")
      endif()
  endforeach(index1)
