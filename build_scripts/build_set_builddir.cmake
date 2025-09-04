cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Установка директории для сборки и файлов CMAKE
##############################################################################

  # для MacOS
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
      set(DISKNAME "RAM_Disk")
      set(ROOTDIR "/Volumes/${DISKNAME}")
      # Создать RAM-диск

      if(NOT EXISTS "${ROOTDIR}")

          execute_process(COMMAND
            hdiutil attach -nomount ram://10000000
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE  output_error
          )
          # Проверить код завершения
          if(NOT result STREQUAL "0")
              message(FATAL_ERROR "Программа hdiutil завершилась с ошибкой: ${result}\n${output_error}")
          endif()

          string(REGEX REPLACE "[ \r\n\t]+" "" output1 "${output}")

          execute_process(COMMAND
            diskutil eraseVolume HFS+ ${DISKNAME} ${output1}
            RESULT_VARIABLE result2
            OUTPUT_VARIABLE output2
            ERROR_VARIABLE  output2_error
          )
          # Проверить код завершения
          if(NOT result2 STREQUAL "0")
              message(FATAL_ERROR "Программа diskutil завершилась с ошибкой: ${result2}\n${output2_error}")
          endif()
      endif()
      # Установить директорию сборки
      set(BUILDDIR "${ROOTDIR}/build_svgtoraster" CACHE INTERNAL "")
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
      # Установить директорию сборки
      set(BUILDDIR "T:/build_svgtoraster" CACHE INTERNAL "")
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
      # mkdir /cache
      # chmod 777 /cache
      # mount -t tmpfs -o size=1024M tmpfs /cache

      # Установить директорию сборки
      set(BUILDDIR "/var/tmp/build_svgtoraster" CACHE INTERNAL "")
  endif()

  # Установить директорию сборки для файлов CMAKE
  set(CMAKEDIR "${BUILDDIR}/cmake" CACHE INTERNAL "")

  # Сохранить директории в переменных окружения
  set(ENV{BUILDDIR} "${BUILDDIR}")
  set(ENV{CMAKEDIR} "${CMAKEDIR}")
