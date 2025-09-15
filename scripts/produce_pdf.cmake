##############################################################################
# Скрипт создания и установки файла документации pdf
# Аргументы:
# DOC_PATH - директория для собираемых файлов документации
# INST_DOC_PATH - директория для устанавливаемых файлов документации
# INST_DOC_FILE - имя устанавливаемого файла документации
##############################################################################

# Выполнить создание файла документации pdf
execute_process(
  WORKING_DIRECTORY "${DOC_PATH}/latex"
  COMMAND "make"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_VARIABLE  output_error
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "Ошибка выполнения скрипта produce_pdf: ${output_error}")
endif()

# При отсутствии создать директорию установки документации
file(MAKE_DIRECTORY "${INST_DOC_PATH}")

# Скопировать файл документации pdf в директорию установки
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "${DOC_PATH}/latex/refman.pdf" "${INST_DOC_PATH}/${INST_DOC_FILE}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_VARIABLE  output_error
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "Ошибка выполнения скрипта produce_pdf: ${output_error}")
endif()
