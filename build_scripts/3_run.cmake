  cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Настройка окружения
##############################################################################

  include(build_set_builddir.cmake)

##############################################################################
# Настройка шаблона журналирования
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
  set(ENV{QT_MESSAGE_PATTERN} "${MESSAGE_PATTERN_DEBUG}")

##############################################################################
# Запуск программы
##############################################################################

  file(REMOVE "${BUILDDIR}/log_run.txt")
  file(REMOVE "${BUILDDIR}/log_run_errors.txt")

  # Вывод информации о работе с плюгинами Qt
  # set(ENV{QT_DEBUG_PLUGINS} 1)

  # для Windows
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")

      # Выполнить приложение
      execute_process(
        COMMAND "cmd" "/c"
        ${BUILDDIR}/install/svgtoraster.exe
        --s 256 128 64 48 32 24 16
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.ico
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка ICO успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка ICO завершено с ошибкой ${result}. Выход.")
      endif()

      # Выполнить приложение
      execute_process(
        COMMAND "cmd" "/c"
        ${BUILDDIR}/install/svgtoraster.exe
        --s 16 32 64 128 256 512 1024
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.icns
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка ICNS успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка ICNS завершено с ошибкой ${result}. Выход.")
      endif()

      # Выполнить приложение
      execute_process(
        COMMAND "cmd" "/c"
        ${BUILDDIR}/install/svgtoraster.exe
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.png
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка PNG успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка PNG завершено с ошибкой ${result}. Выход.")
      endif()

  # для MacOS
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")

      # Выполнить приложение
      execute_process(
        COMMAND open -a ${BUILDDIR}/install/svgtoraster.app
        --stdout ${BUILDDIR}/log_run.txt
        --args
        --s 256 128 64 48 32 24 16
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.ico
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка ICO успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка ICO завершено с ошибкой ${result}. Выход.")
      endif()

      # Выполнить приложение
      execute_process(
        COMMAND open -a ${BUILDDIR}/install/svgtoraster.app
        --stdout ${BUILDDIR}/log_run.txt
        --args
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.png
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка PNG успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка PNG завершено с ошибкой ${result}. Выход.")
      endif()

  # для Linux
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")

      # Выполнить приложение
      execute_process(
        COMMAND ${BUILDDIR}/install/bin/svgtoraster
        --s 256 128 64 48 32 24 16
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.ico
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка ICO успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка ICO завершено с ошибкой ${result}. Выход.")
      endif()

      # Выполнить приложение
      execute_process(
        COMMAND ${BUILDDIR}/install/bin/svgtoraster
#        --s 16 32 64 128 256 512 1024
        --s 16
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.icns
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка ICNS успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка ICNS завершено с ошибкой ${result}. Выход.")
      endif()

      # Выполнить приложение
      execute_process(
        COMMAND ${BUILDDIR}/install/bin/svgtoraster
        --i ../Candle/icons/icon.svg
        --o ${BUILDDIR}/icon.png
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE  output_error
        ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE
      )
      if (result EQUAL 0)
          message(STATUS "Run: Создание значка PNG успешно завершено.")
      else()
          message(FATAL_ERROR "Run: Создание значка PNG завершено с ошибкой ${result}. Выход.")
      endif()

  endif()

  if(NOT "^^${output}" STREQUAL "^^")
      file(WRITE "${BUILDDIR}/log_run.txt" "${output}")
  endif()
  if(NOT "^^${output_error}" STREQUAL "^^")
      file(WRITE "${BUILDDIR}/log_run_errors.txt" "${output_error}")
  endif()
