cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Установка генератора для CMAKE
##############################################################################

  # Генератор по умолчанию (для CMake это Makefile)
# set(MULTICONFIG 0 CACHE INTERNAL "")

  # Генератор Makefile - создаёт стандартные файлы makefile
# set(GENERATOR "Unix Makefiles")
# set(MULTICONFIG 0 CACHE INTERNAL "")

  # Генератор Ninja - создаёт конфигурационные файлы build.ninja
  set(GENERATOR "Ninja" CACHE INTERNAL "")
  set(MULTICONFIG 0 CACHE INTERNAL "")

  # Генератор Ninja мультиконфигурационный - создаёт конфигурационные файлы build-<Config>.ninja
# set(GENERATOR "Ninja Multi-Config" CACHE INTERNAL "")
# set(MULTICONFIG 1 CACHE INTERNAL "")

  if( (NOT DEFINED GENERATOR) OR ("^^${GENERATOR}" STREQUAL "^^") )
      message(STATUS "SetGenerator: Генератор не определён, будет использовано значение по умолчанию Unix Makefiles")
  endif()
