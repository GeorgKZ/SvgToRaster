  cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Сборка
##############################################################################

  # Установить конфигурацию для мультиконфигурационного варианта этапа сборки
  if( (DEFINED MULTICONFIG) AND (MULTICONFIG GREATER 0) )
      set(CMAKE_BLD "--config ${CONFIG}")
  endif()

  # Выполнить построение исполняемого файла
  execute_process(
    COMMAND ${CMAKE_COMMAND}
    --build ${CMAKEDIR}
    ${CMAKE_BLD}
    --parallel
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE  output_error
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
  )
  file(WRITE "${BUILDDIR}/log_build.txt" "${output}")
  if(NOT "^^${output_error}" STREQUAL "^^")
      file(WRITE "${BUILDDIR}/log_build_errors.txt" "${output_error}")
  endif()
  if (result EQUAL 0)
      message(STATUS "Build: Построение успешно завершено.")
  else()
      message(FATAL_ERROR "Build: Построение завершено с ошибкой ${result}. Выход.")
  endif()
