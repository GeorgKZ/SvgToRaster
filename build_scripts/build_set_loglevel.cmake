cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Установка уровня детализации сообщений сборки
##############################################################################

# Существуют следующие уровни выводимых сообщений:
# FATAL_ERROR - серьёзная ошибка, вызывающая полную остановку сборки;
# set(LOGLEVEL "FATAL_ERROR CACHE INTERNAL "")
# SEND_ERROR - серьёзная ошибка, вызывающая остановку сборки без остановки выполнения скрипта Cmake;
# set(LOGLEVEL "SEND_ERROR CACHE INTERNAL "")
# WARNING - предупреждение;
# set(LOGLEVEL "WARNING CACHE INTERNAL "")
# AUTHOR_WARNING - предупреждение разработчика;
# set(LOGLEVEL "AUTHOR_WARNING CACHE INTERNAL "")
# DEPRECATION - ошибка или предупреждение об использовании устаревшей функции Cmake;
# set(LOGLEVEL "DEPRECATION CACHE INTERNAL "")
# NOTICE - важное сообщение, выводимое на устройство stderr;
# set(LOGLEVEL "NOTICE CACHE INTERNAL "")
# STATUS - краткое важное сообщение (значение по умолчанию);
# set(LOGLEVEL "STATUS CACHE INTERNAL "")
# VERBOSE - подробная информация об особенностях сборки;
# set(LOGLEVEL "VERBOSE CACHE INTERNAL "")
# DEBUG - подробная информация, необходимая только разработчику;
  set(LOGLEVEL "DEBUG" CACHE INTERNAL "")
# TRACE - сообщение о ходе сборки для точного отслеживания процесса.
# set(LOGLEVEL "TRACE CACHE INTERNAL "")
# Выводятся сообщения от указанного уровня (включая его) и выше.

  if( (NOT DEFINED LOGLEVEL) OR ("^^${LOGLEVEL}" STREQUAL "^^") )
      message(STATUS "SetLoglevel: Уровень выводимых сообщений не определён, будет использовано значение по умолчанию STATUS")
  endif()
