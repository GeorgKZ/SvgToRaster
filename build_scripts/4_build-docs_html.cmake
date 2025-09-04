  cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Функция удаления файла нулевой длины
##############################################################################
  include(build_remove_zerofile.cmake)

##############################################################################
# Настройка параметров сборки
##############################################################################

  # Использовать модуль установки генератора для CMAKE
  include(build_set_generator.cmake)

  # Использовать модуль установки конфигурации
  include(build_set_config.cmake)

  # Использовать модуль установки уровня детализации сообщений сборки
  include(build_set_loglevel.cmake)

##############################################################################
# Настройка окружения
##############################################################################

  # Использовать модуль установки пути к исходным текстам
  include(build_set_source.cmake)

  # Использовать модуль установки директории для сборки и файлов CMAKE
  include(build_set_builddir.cmake)

  # Использовать модуль установки компилятора
  include(build_set_compiler.cmake)

  # Использовать модуль установки пути к требуемой версии Qt
  include(build_set_qt.cmake)

##############################################################################
# Построение документации в формате HTML
##############################################################################

  # Установить конфигурацию для мультиконфигурационного варианта этапа сборки
  if( (DEFINED MULTICONFIG) AND (MULTICONFIG GREATER 0) )
      set(CMAKE_BLD "--config ${CONFIG}")
  endif()

  file(REMOVE "${BUILDDIR}/log_hdoc.txt")
  file(REMOVE "${BUILDDIR}/log_hdoc_errors.txt")

  # Выполнить построение документации в формате HTML
  execute_process(
    COMMAND ${CMAKE_COMMAND}
    --build ${CMAKEDIR}
    ${CMAKE_BLD}
    --target html_doc
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE  output_error
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
  )
  file(WRITE "${BUILDDIR}/log_hdoc.txt" "${output}")
  if(NOT "^^${output_error}" STREQUAL "^^")
      file(WRITE "${BUILDDIR}/log_hdoc_errors.txt" "${output_error}")
  endif()
  if (result EQUAL 0)
      message(STATUS "Построение документации HTML успешно завершено.")
  else()
      message(FATAL_ERROR "Построение документации HTML завершено с ошибкой ${result}. Выход.")
  endif()
