cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Конфигурирование
##############################################################################

  # Подготовить параметр "генератор"
  if( (DEFINED GENERATOR) AND (NOT "^^${GENERATOR}" STREQUAL "^^") )
      set(GENERATOR_STRING "-G ${GENERATOR}")
  else()
      message(STATUS "BuildClean: Генератор не указан, будет использовано значение по умолчанию")
  endif()

  # Подготовить параметр "уровень детализации"
  if( (DEFINED LOGLEVEL) AND (NOT "^^${LOGLEVEL}" STREQUAL "^^") )
      set(LOGLEVEL_STRING "--log-level=${LOGLEVEL}")
  else()
      message(STATUS "BuildClean: Уровень детализации сообщений не указан, будет использовано значение по умолчанию")
  endif()

  # Установить конфигурацию для не мультиконфигурационного варианта этапа конфигурирования
  if( (NOT DEFINED MULTICONFIG) OR (MULTICONFIG EQUAL 0) )
      set(CMAKE_CNF "-D CMAKE_BUILD_TYPE:STRING=${CONFIG}")
  endif()

  # Выполнить конфигурирование
  execute_process(
    COMMAND ${CMAKE_COMMAND}
    ${COMPILER}
    ${LOGLEVEL_STRING}
    ${CMAKE_CNF}
    ${GENERATOR_STRING}
    -S ${SOURCEDIR}
    -B ${CMAKEDIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE  output_error
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
  )
  file(WRITE "${BUILDDIR}/log_config.txt" "${output}")
  if(NOT "^^${output_error}" STREQUAL "^^")
      file(WRITE "${BUILDDIR}/log_config_errors.txt" "${output_error}")
  endif()
  if (result EQUAL 0)
      message(STATUS "Config: Конфигурирование успешно завершено.")
  else()
      message(FATAL_ERROR "Config: Конфигурирование завершено с ошибкой ${result}. Выход.")
  endif()
