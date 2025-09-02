##############################################################################
# Минимально необходимая для нормальной работы версия cmake, Qt
##############################################################################

cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Установки, которые необходимо выполнить до директивы project(...)
##############################################################################

# Директория для промежуточных и собранных файлов
if(DEFINED BUILDDIR) 
    message(DEBUG "Директория сборки согласно переменной BUILDDIR=${BUILDDIR}")
    cmake_path(SET BUILD_PATH NORMALIZE "${BUILDDIR}")
elseif(DEFINED ENV{BUILDDIR}) 
    message(DEBUG "Директория сборки согласно окружению BUILDDIR=$ENV{BUILDDIR}")
    cmake_path(SET BUILD_PATH NORMALIZE "$ENV{BUILDDIR}")
else()
    set(BUILD_PATH "${CMAKE_BINARY_DIR}/build")
endif()

# Директория для установочного набора
set(CMAKE_INSTALL_PREFIX "${BUILD_PATH}/install" CACHE PATH "Install path prefix, prepended onto install directories.")

##############################################################################
# Информация о проекте
##############################################################################

include(Custom.cmake)

# Имя, версия, описание проекта
project(${CUSTOM_PROJECT_NAME}
    VERSION ${CUSTOM_VERSION}
    DESCRIPTION ${CUSTOM_DESCRIPTION}
    HOMEPAGE_URL ${CUSTOM_HOMEPAGE_URL}
    LANGUAGES ${CUSTOM_LANGUAGES}
)

# Имя проекта, приведённое к нижнему регистру
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)

# Поставщик установочного пакета
set(PROJECT_VENDOR "${CUSTOM_PROJECT_VENDOR}")

# Контактные данные поставщика установочного пакета
set(PROJECT_MAINTAINER "${CUSTOM_PROJECT_MAINTAINER}")

# Путь к файлу значка ICO проекта
set(ICON_FILE "${CUSTOM_ICON_FILE}")

##############################################################################
# Относительные пути для пакетов MacOS/OS X
##############################################################################

# Имя пакета для MacOS/OS X
set(BUNDLE_NAME "${PROJECT_NAME_LOWERCASE}.app")

# Путь к директории содержимого пакета для MacOS/OS X
set(BUNDLE_CONTENT_PATH "${BUNDLE_NAME}/Contents")

# Путь к директории исполняемых файлов пакета для MacOS/OS X
set(BUNDLE_BINARY_PATH "${BUNDLE_CONTENT_PATH}/MacOS")

# Путь к директории фреймворков пакета для MacOS/OS X
set(BUNDLE_FRAMEWORK_PATH "${BUNDLE_CONTENT_PATH}/Frameworks")

# Путь к директории ресурсов пакета для MacOS/OS X
set(BUNDLE_RESOURCE_PATH "${BUNDLE_CONTENT_PATH}/Resources")

# Путь к директории плюгинов Qt для MacOS/OS X
set(BUNDLE_PLUGIN_PATH "${BUNDLE_BINARY_PATH}")

##############################################################################
# Установки, которые необходимо выполнить до поиска Qt
##############################################################################

# При сборке для работы в среде Windows
if((CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "MSYS"))

    # Путь, куда устанавливать динамические библиотеки
    set(CMAKE_INSTALL_LIBDIR "./"
      CACHE PATH "Object code libraries (lib)"
    )
    # Путь, куда устанавливать исполняемые файлы
    set(CMAKE_INSTALL_BINDIR "./"
      CACHE PATH "User executables (bin)"
    )

# При сборке для работы в среде Linux
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    # Путь, куда устанавливать динамические библиотеки
    set(CMAKE_INSTALL_LIBDIR "lib/${PROJECT_NAME_LOWERCASE}/lib"
      CACHE PATH "Object code libraries (lib)"
    )
    # Путь, куда устанавливать исполняемые файлы
    set(CMAKE_INSTALL_BINDIR "bin"
      CACHE PATH "User executables (bin)"
    )

# При сборке для работы в среде MacOS/OS X
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    # Путь, куда устанавливать динамические библиотеки
    set(CMAKE_INSTALL_LIBDIR "${BUNDLE_FRAMEWORK_PATH}"
      CACHE PATH "Object code libraries (lib)"
    )
    # Путь, куда устанавливать исполняемые файлы
    set(CMAKE_INSTALL_BINDIR "${BUNDLE_BINARY_PATH}"
      CACHE PATH "User executables (bin)"
    )

else()
    message(WARNING "Неизвестная среда сборки " ${CMAKE_SYSTEM_NAME})
endif()

##############################################################################
# Настройка местоположения Qt
##############################################################################

# Определить местоположение Qt
if(DEFINED ENV{QT_ROOT})
    message(DEBUG "Директория Qt согласно окружению QT_ROOT=$ENV{QT_ROOT}")

    # Эти переменные должны указывать на директорию, в которой расположен файл Qt6Config.cmake,
    # например /opt/qt691/lib/cmake/Qt6/
    set(QT_DIR  "$ENV{QT_ROOT}/lib/cmake/Qt6/" CACHE PATH "The directory containing a CMake configuration file for QT.")
    set(Qt6_DIR "$ENV{QT_ROOT}/lib/cmake/Qt6/" CACHE PATH "The directory containing a CMake configuration file for Qt6.")
    # Эта переменная должна указывать на установочную директорию Qt
    set(CMAKE_PREFIX_PATH "$ENV{QT_ROOT}")
    find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
    find_package(Qt6 COMPONENTS Core REQUIRED)
    # Получить путь к библиотеке libQt6Core, например /opt/qt691/lib/libQt6Core.so.6.9.1
    get_target_property(QtCore_location Qt6::Core LOCATION)
    message(DEBUG "Qt6 libQt6Core location: ${QtCore_location}")
else()
    find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
    find_package(Qt6 COMPONENTS Core REQUIRED)
    # Получить путь к библиотеке libQt6Core, например /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.4.2
    get_target_property(QtCore_location Qt6::Core LOCATION)
    message(DEBUG "Qt6 libQt6Core location: ${QtCore_location}")
    # Получить из этого пути директорию, в которой расположен файл Qt6Config.cmake,
    # например /usr/lib/x86_64-linux-gnu/cmake/Qt6, и присвоить полченное значение
    # переменным QT_DIR, Qt6_DIR
    cmake_path(GET QtCore_location ROOT_DIRECTORY QtCore_path)
    set(QT_DIR  "$ENV{QtCore_path}/lib/cmake/Qt6/" CACHE PATH "The directory containing a CMake configuration file for QT.")
    set(Qt6_DIR "$ENV{QtCore_path}/lib/cmake/Qt6/" CACHE PATH "The directory containing a CMake configuration file for Qt6.")
endif()

# Директория, в которой расположены плюгины Qt
set(QT_PLUGIN_PATH "${QT_DIR}/../../../plugins")

##############################################################################
# Директории, создаваемые для выполнения сборки
##############################################################################

# Директория для результирующих бинарных файлов
set(BIN_PATH "${BUILD_PATH}/bin")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_PATH})

# Директория для промежуточных файлов
set(MISC_PATH "${BUILD_PATH}/misc")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${MISC_PATH})

# Директория для заготовок значков Mac OS/OSX
set(ICNS_PATH "${MISC_PATH}/icns.iconset")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${ICNS_PATH})

# Директория для файлов, для которых будут искаться зависимости
set(FIND_DEPS_PATH "${MISC_PATH}/find_deps")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${FIND_DEPS_PATH})

# Директория найденных зависимостей
set(FOUND_DEPS_PATH "${MISC_PATH}/found_deps")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${FOUND_DEPS_PATH})

# Директория для файлов с изображениями значка
set(ICONS_PATH "${BUILD_PATH}/icons")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${ICONS_PATH})

# Директория для документации приложения
set(DOC_PATH "${BUILD_PATH}/docs")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${DOC_PATH})

##############################################################################
# Прочие директории и пути к файлам, помещаемые в переменные
##############################################################################

# Путь к директории с изображениями для Doxygen
#!!! set(DOXY_IMAGES "${CMAKE_SOURCE_DIR}/screenshots;${CMAKE_SOURCE_DIR}/doc/images;${CMAKE_SOURCE_DIR}/wiki/images")

# Путь к директории с переводами
set(TRANSLATE_PATH "${BUILD_PATH}/translations")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${TRANSLATE_PATH})

##############################################################################
# Пути к файлам lupdate, lrelease
##############################################################################

if ((CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux") OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")

    # Исполняемый файл lupdate
    if(EXISTS "/usr/lib/qt6/bin/lupdate")
        set(LUPDATE_CMD "/usr/lib/qt6/bin/lupdate")
    elseif(EXISTS "${QT_DIR}/../../../bin/lupdate")
        set(LUPDATE_CMD "${QT_DIR}/../../../bin/lupdate")
    else()
        set(LUPDATE_CMD "lupdate")
    endif()

    # Исполняемый файл lrelease
    if(EXISTS "/usr/lib/qt6/bin/lrelease")
        set(LRELEASE_CMD "/usr/lib/qt6/bin/lrelease")
    elseif(EXISTS "${QT_DIR}/../../../bin/lrelease")
        set(LRELEASE_CMD "${QT_DIR}/../../../bin/lrelease")
    else()
        set(LUPDATE_CMD "lrelease")
    endif()

elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")

    # Исполняемый файл lupdate
    if(EXISTS "${QT_DIR}/../../../bin/lupdate.exe")
        set(LUPDATE_CMD "${QT_DIR}/../../../bin/lupdate.exe")
    else()
        set(LUPDATE_CMD "lupdate.exe")
    endif()

    # Исполняемый файл lrelease
    if(EXISTS "${QT_DIR}/../../../bin/lrelease.exe")
        set(LRELEASE_CMD "${QT_DIR}/../../../bin/lrelease.exe")
    else()
        set(LRELEASE_CMD "lrelease.exe")
    endif()

endif()

##############################################################################
# Определение битности целевого исполняемого файла
##############################################################################

if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(TARGET_BIT_WIDTH "64")
elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(TARGET_BIT_WIDTH "32")
else()
    message(WARNING "Неизвестная битность целевого исполняемого файла - ${CMAKE_SIZEOF_VOID_P} байт")
endif()

##############################################################################
# Определение архитектуры целевого исполняемого файла
##############################################################################

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(TARGET_ARCH "${CMAKE_OSX_ARCHITECTURES}")
    if ("^^${TRAGET_ARCH}" STREQUAL "^^")
        set(TARGET_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
else()
    set(TARGET_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()

##############################################################################
# Настройка в зависимости от среды сборки
##############################################################################

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")

# Команда перенаправления потока в нуль
    set(CMD_TO_NUL >/dev/null 2>&1)
# Командный файл для запуска pdflatex с параметром
    set(CMD_RUN_LATEX "${MISC_PATH}/runlatex.bash")
# Подготовить командный файл для запуска pdflatex с параметром
    file(WRITE ${CMD_RUN_LATEX} "#!/bin/bash\n\npdflatex --shell-escape $1")
    execute_process(COMMAND chmod +x ${CMD_RUN_LATEX})

elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")

# Команда перенаправления потока в нуль
    set(CMD_TO_NUL >NUL 2>&1)

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")

# Команда перенаправления потока в нуль
    set(CMD_TO_NUL >/dev/null 2>&1)
# Командный файл для запуска pdflatex с параметром
    set(CMD_RUN_LATEX "${MISC_PATH}/runlatex.zsh")
# Подготовить командный файл для запуска pdflatex с параметром
    file(WRITE ${CMD_RUN_LATEX} "#!/bin/zsh\n\npdflatex --shell-escape $1")
    execute_process(COMMAND chmod +x ${CMD_RUN_LATEX})

else()
    message(WARNING "Неизвестная среда сборки ${CMAKE_HOST_SYSTEM_NAME}")
endif()

##############################################################################
# Настройка в зависимости от используемого компилятора.
##############################################################################

# Требуемая версия компилятора C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# clang
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    add_compile_options(
#        "-Wall"
#        "-Wpedantic"
#        "-Wextra"
#        "-Wunused-but-set-variable"
        "-Weverything"
    )
    add_definitions(
        "-D__CLANG__"
        "-D__MYFUNCNAME__=__PRETTY_FUNCTION__"  #!!! Проверить на linux, windows!!!
    )
    # Обрезать таблицы символов для Release
    add_link_options(
        $<$<CONFIG:RELEASE>:-s>
    )
    # Обрезать таблицы символов для MinSizeRel
    add_link_options(
        $<$<CONFIG:MINSIZEREL>:-s>
    )
# clang из MacOX/OS X
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")

    add_compile_options(
#        "-Wall"
#        "-Wpedantic"
#        "-Wextra"
#        "-Wunused-but-set-variable"
        "-Weverything"
        "-Wno-padded"
        "-Wno-c++98-compat"
        "-Wno-c++98-compat-pedantic"
        "-Wno-documentation-unknown-command"
        "-Wno-poison-system-directories"
        "-Wno-global-constructors"
        "-Wno-ctad-maybe-unsupported"
        "-Wno-missing-prototypes"
        # Временно
        "-Wno-implicit-float-conversion"
        "-Wno-implicit-int-conversion"
        "-Wno-float-conversion"
        "-Wno-double-promotion"
        "-Wno-float-equal"
        "-Wno-shorten-64-to-32"
        "-Wno-old-style-cast"
        "-Wno-sign-conversion"
        "-Wno-exit-time-destructors"
    )
    add_definitions(
        "-D__CLANG__"
        "-D__MYFUNCNAME__=__PRETTY_FUNCTION__"  #!!! Проверить на MacOS!!!
    )
    # Обрезать таблицы символов для Release
    add_link_options(
        $<$<CONFIG:RELEASE>:-s>
    )
    # Обрезать таблицы символов для MinSizeRel
    add_link_options(
        $<$<CONFIG:MINSIZEREL>:-s>
    )

# gcc
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

#    # Подготовить командный файл для получения списка всех предупреждений компилятора
#    set(CMD_GEN_WARN "${MISC_PATH}/gen_gcc_warn.sh")
#    file(WRITE ${CMD_GEN_WARN} "#!/bin/bash\n\n${CMAKE_CXX_COMPILER} -Q --help=warning | sed -e \'s/^\\s*\\\(\\-\\S*\\\)\\s*\\[\\w*\\]/\\1 /gp;d\' | tr -d '\\n'")
#    execute_process(COMMAND chmod +x ${CMD_GEN_WARN})
#    # Получить список всех предупреждений компилятора
#    execute_process(COMMAND ${CMD_GEN_WARN} OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CXX_WARNINGS)
#    # Обработать список всех предупреждений компилятора для избежания заключения его в кавычки
#    separate_arguments(CXX_WARNINGS UNIX_COMMAND "${CXX_WARNINGS}")
 
    add_compile_options(
 #      ${CXX_WARNINGS}
        -Wall
        -Wpedantic
        -Wextra
        -Waddress
        -Wbool-compare
        -Wbool-operation
        -Wcast-align
        -Wcast-qual
        -Wcatch-value
        -Wchar-subscripts
        -Wcomment
        -Wctor-dtor-privacy
        -Wdisabled-optimization
        -Wformat=2
        -Wint-in-bool-context
        -Winit-self
        -Wlogical-not-parentheses
        -Wlogical-op
        -Wmaybe-uninitialized
        -Wmemset-elt-size
        -Wmemset-transposed-args
        -Wmisleading-indentation
        -Wmissing-attributes
        -Wmissing-include-dirs
        -Wmultistatement-macros
        -Wnoexcept
        -Wnonnull
        -Wnonnull-compare
 #      -Wold-style-cast
        -Woverloaded-virtual
        -Wparentheses
        -Wpessimizing-move
        -Wredundant-decls
        -Wreorder
        -Wreturn-type
#       -Wshadow
        -Wsequence-point
#       -Wsign-compare
#       -Wsign-promo
        -Wsizeof-pointer-div
        -Wsizeof-pointer-memaccess
        -Wstrict-aliasing
        -Wstrict-null-sentinel
#       -Wstrict-overflow=5
        -Wswitch
        -Wtautological-compare
        -Wtrigraphs
        -Wundef
        -Wuninitialized
        -Wunused-but-set-variable
        -Wunused-function
        -Wunused-variable
        -fdiagnostics-show-option
    )
    add_definitions(
        "-D__MYFUNCNAME__=__PRETTY_FUNCTION__"
    )
    # MinGW или MSYS для сборки под Windows
    if ((CMAKE_CXX_PLATFORM_ID STREQUAL "MinGW") OR (CMAKE_CXX_PLATFORM_ID STREQUAL "MSYS"))
        add_link_options(
            "-static"
        )
        add_definitions(
            "-D_POSIX_SOURCE"
        )
    endif()

    # Если выполняется сборка для работы под Windows системой MSYS
#    if ((CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "MSYS"))
#        add_link_options(
#            "-static-libstdc++"
#            "-static-libgcc"
#            "-static"
#           "-lwinpthread"
#        )
#    endif()


    # Обрезать таблицы символов для Release
    add_link_options(
        $<$<CONFIG:RELEASE>:-s>
    )
    # Обрезать таблицы символов для MinSizeRel
    add_link_options(
        $<$<CONFIG:MINSIZEREL>:-s>
    )

# Microsoft Visual Studio
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    add_definitions(
        "-D__MYFUNCNAME__=__FUNCSIG__"
        "-D__STDC_WANT_SECURE_LIB__=1"
        "-D_CRT_SECURE_NO_WARNINGS"
    )

    add_compile_options(
        /vd2
#       Разрешить все предупреждения
        /Wall
#       Запретить указанные предупреждения
        /wd4242 # C4242 =: преобразование "int" в "char", возможна потеря данных
        /wd4244 # C4244 =: преобразование "..." в "...", возможна потеря данных
        /wd4245 # C4245 инициализация: преобразование "..." в "...", несоответствие типов со знаком и без
        /wd4267 # C4267 аргумент: преобразование из "size_t" в "int"; возможна потеря данных
        /wd4305 # C4305 аргумент: усечение из "double" в "float"
        /wd4310 # C4310 приведение обуславливает усечение постоянного значения
        /wd4365 # C4365 преобразование "..." в "...", несоответствие типов со знаком и без
        /wd4456 # C4456 объявление "..." скрывает предыдущее локальное объявление
        /wd4458 # C4458 объявление "..." скрывает член класса
        /wd4464 # C4464 относительный путь включения содержит ".."
        /wd4514 # C4514 подставляемая функция, не используемая в ссылках, была удалена
        /wd4623 # C4623 конструктор по умолчанию неявно определен как удаленный
        /wd4625 # C4625 конструктор копий неявно определен как удаленный
        /wd4626 # C4626 оператор назначения неявно определен как удаленный
        /wd4710 # C4710 функция не является встроенной
        /wd4711 # C4711 функция "..." выбрана для автоматического преобразования во встроенную
        /wd4820 # C4820 отбивка размером "..." байт добавлена после данные-член
        /wd4866 # C4866 возможно, компилятор не выполняет вычисление справа налево для вызова
        /wd4946 # C4946 использование reinterpret_cast между связанными классами: "..." и "..."
        /wd5026 # C5026 конструктор перемещения неявно определён как удалённый
        /wd5027 # C5027 оператор назначения перемещением неявно определен как удаленный
        /wd5045 # C5045 Компилятор вставит компонент устранения рисков Spectre для загрузки памяти, если указан параметр /Qspectre
        /wd5219 # C5219 неявное преобразование "..." в "...", возможна потеря данных
        /wd5267 # C5267 определение неявного конструктор копий для '...' устарело, так как оно имеет предоставленный пользователем деструктор
#       /EHa   # Насчёт обработки исключений
    )
    # Настройка минимально требуемого размера стека
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /STACK:4000000,10000")
else()
    message(WARNING "Неизвестный компилятор ${CMAKE_CXX_COMPILER_ID}")
endif()

##############################################################################
# Настройка в зависимости от целевой среды выполнения.
##############################################################################

# Если сборка предназначена для выполнения под Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    # не выполнять установку RPATH (путь, где искать динамические библиотеки)
    # на этапе сборки
    set(CMAKE_SKIP_BUILD_RPATH TRUE)
    # при сборке не применять RPATH этапа установки (CMAKE_INSTALL_RPATH)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
    # don't add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

    # Путь для установки файла лицензии в установочном пакете
    set(INST_LIC_PATH "share/doc/${PROJECT_NAME_LOWERCASE}")

    # Путь к файлу лицензии относительно исполняемого файла
    set(REL_LIC_DIR "../${INST_LIC_PATH}")

    # Путь для установки плюгинов приложения в установочном пакете
    set(INST_PLUG_DIR "share/${PROJECT_NAME_LOWERCASE}/plugins")

    # Путь к плюгинам приложения относительно исполняемого файла
    set(REL_PLUG_DIR "../${INST_PLUG_DIR}")

    # Путь для установки плюгинов Qt в установочном пакете
    set(INST_QTPLUG_DIR "lib/${PROJECT_NAME_LOWERCASE}/plugins")

# Если сборка предназначена для выполнения под MacOS / OS X
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    # Путь для установки файла лицензии в установочном пакете
    set(INST_LIC_PATH "${BUNDLE_RESOURCE_PATH}")

    # Путь к файлу лицензии относительно исполняемого файла
    set(REL_LIC_DIR "../../../${BUNDLE_RESOURCE_PATH}")

    # Путь для установки плюгинов приложения в установочном пакете
    set(INST_PLUG_DIR "${BUNDLE_RESOURCE_PATH}/plugins")

    # Путь к плюгинам приложения относительно исполняемого файла
    set(REL_PLUG_DIR "../../../${INST_PLUG_DIR}")

    # Путь для установки плюгинов Qt в установочном пакете
    set(INST_QTPLUG_DIR "${BUNDLE_PLUGIN_PATH}")

# Если сборка предназначена для выполнения под Windows
elseif((CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "MSYS"))

    # Путь для установки файла лицензии в установочном пакете
    set(INST_LIC_PATH ".")

    # Путь к файлу лицензии относительно исполняемого файла
    set(REL_LIC_DIR ".")

    # Путь для установки плюгинов приложения в установочном пакете
    set(INST_PLUG_DIR "${PROJECT_NAME_LOWERCASE}_plugins")

    # Путь к плюгинам приложения относительно исполняемого файла
    set(REL_PLUG_DIR "${INST_PLUG_DIR}")

    # Путь для установки плюгинов Qt в установочном пакете
    set(INST_QTPLUG_DIR "plugins")

else()

    message(WARNING "Неизвестная среда выполнения " ${CMAKE_SYSTEM_NAME})

endif()

##############################################################################
# Настройка в зависимости от типа сборки.
##############################################################################

# Определить, является ли генератор мультиконфигурационным
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (isMultiConfig)
    # Типы сборки для генераторов с мульти-конфигом
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel")
    message(DEBUG "Будет собрана конфигурация, указанная при сборке")
    # Имя директории, где будут расположены директории с исполняемыми файлами
    # приложения для разных вариантов конфигурации
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_PATH}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_PATH}")
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
        message(DEBUG "Будет собрана конфигурация по умолчанию " ${CMAKE_BUILD_TYPE})
    else()
        message(DEBUG "Будет собрана выбранная конфигурация " ${CMAKE_BUILD_TYPE})
    endif()
    # Имя директории, где будет расположен исполняемый файл приложения
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_PATH}/${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_PATH}/${CMAKE_BUILD_TYPE}")
endif()

##############################################################################
# Создание файлов для Mac OS/OS X
##############################################################################

# Если сборка предназначена для выполнения под MacOS / OS X
#!!!
if(0 AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    ##############################################################################
    # Создание файла значков *.icns
    ##############################################################################

    # Список исходных и результирующих имён битмапов
    set(PNG_LIST
      "icon_16.png"   "icon_16x16.png"
      "icon_32.png"   "icon_16x16@2x.png"
      "icon_32.png"   "icon_32x32.png"
      "icon_64.png"   "icon_32x32@2x.png"
      "icon_128.png"  "icon_128x128.png"
      "icon_256.png"  "icon_128x128@2x.png"
      "icon_256.png"  "icon_256x256.png"
      "icon_512.png"  "icon_256x256@2x.png"
      "icon_512.png"  "icon_512x512.png"
      "icon_1024.png" "icon_512x512@2x.png"
    )

    # Выполнить копирование и переименование битмапов согласно списка
    list(LENGTH PNG_LIST PNG_LIST_LEN)
    math(EXPR PNG_LIST_LEN "${PNG_LIST_LEN} - 2") 
    foreach(pos0 RANGE 0 "${PNG_LIST_LEN}" 2)
        math(EXPR pos1 "${pos0} + 1") 
        list(GET PNG_LIST "${pos0}" PNG_SRC)
        list(GET PNG_LIST "${pos1}" PNG_DST)
        file(COPY_FILE
          "${CMAKE_SOURCE_DIR}/src/candle/images/icons_png/${PNG_SRC}"
          "${ICNS_PATH}/${PNG_DST}"
        )
    endforeach()

    # Преобразовать битмапы в файл icns
    execute_process(COMMAND
      iconutil
      -c icns
      "${ICNS_PATH}"
      -o "${ICONS_PATH}/${PROJECT_NAME_LOWERCASE}.icns"
    )

endif()

# Если сборка предназначена для выполнения под MacOS / OS X
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    ##############################################################################
    # Создание файла PkgInfo
    ##############################################################################

    file(WRITE
      ${MISC_PATH}/PkgInfo
      "APPL????\n"
    )

endif()

##############################################################################
# Вывод информации о проекте, инструментах, среде сборки и среде исполнения
##############################################################################

# Получить уровень выводимых сообщений
cmake_language(GET_MESSAGE_LOG_LEVEL CMAKE_LOGGING_LEVEL)
set(CMAKE_MESSAGE_LOG_LEVEL "${CMAKE_LOGGING_LEVEL}" CACHE STRING "Уровень журналирования")
# Существуют следующие уровни выводимых сообщений:
# FATAL_ERROR - серьёзная ошибка, вызывающая полную остановку сборки;
# SEND_ERROR - серьёзная ошибка, вызывающая остановку сборки без остановки выполнения скрипта Cmake;
# WARNING - предупреждение;
# AUTHOR_WARNING - предупреждение разработчика;
# DEPRECATION - ошибка или предупреждение об использовании устаревшей функции Cmake;
# NOTICE - важное сообщение, выводимое на устройство stderr;
# STATUS - краткое важное сообщение (это уровень по умолчанию);
# VERBOSE - подробная информация об особенностях сборки;
# DEBUG - подробная информация, необходимая только разработчику;
# TRACE - сообщение о ходе сборки для точного отслеживания процесса.
# Выводятся сообщения от указанного уровня (включая) и выше.
# Устанавливается заданием параметра cmake, например cmake --log-level=DEBUG

message(STATUS "##############################################################################")
message(STATUS "Проект '${CMAKE_PROJECT_NAME}' версии ${PROJECT_VERSION}")
message(STATUS "Сайт приложения ${PROJECT_HOMEPAGE_URL}")
message(STATUS "CMake версии ${CMAKE_VERSION}")
message(STATUS "Уровень журналирования CMake ${CMAKE_LOGGING_LEVEL} и выше")
message(STATUS "Компилятор ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Генератор ${CMAKE_GENERATOR}")
message(STATUS "Платформа ${CMAKE_CXX_PLATFORM_ID}")
message(STATUS "Среда сборки ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "Целевая архитектура ${TARGET_ARCH}")
message(STATUS "Целевая разрядность ${TARGET_BIT_WIDTH}")
message(STATUS "Целевая среда исполнения ${CMAKE_SYSTEM_NAME}")
if (CMAKE_BUILD_TYPE)
    message(STATUS "Конфигурация ${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "Qt версии ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
if (DEFINED QT_HOST_PATH) 
    message(STATUS "Будет использован Qt, расположенный в '${QT_HOST_PATH}'")
endif()

message(STATUS "##############################################################################")

# Создавать заголовочные файлы для файлов *.ui
set(CMAKE_AUTOUIC ON)
# Использовать метаобъектный компилятор
set(CMAKE_AUTOMOC ON)

##############################################################################
# Формирование установочного набора
##############################################################################

# Вывод сообщений об установке только если было изменение
set(CMAKE_INSTALL_MESSAGE LAZY)

# Установить файл лицензии
message(DEBUG "Установить файл лицензии")
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE
  DESTINATION ${INST_LIC_PATH}
)

# Установить значки программы для Linux
# !!!
if (0 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #!!! Можно использовать один и тот же SVG для всех разрешений
    message(DEBUG "Установить файл значка программы")
    install(FILES ${CMAKE_SOURCE_DIR}/src/candle/images/icons_png/icon_256.png
        DESTINATION "share/icons/hicolor/256x256/apps"
        RENAME "${PROJECT_NAME_LOWERCASE}.png"
    )
# Если сборка предназначена для выполнения под MacOS / OS X
#!!!
elseif(0 AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    install(FILES
      "${ICONS_PATH}/${PROJECT_NAME_LOWERCASE}.icns"
      DESTINATION "${BUNDLE_RESOURCE_PATH}"
    )
endif()


# Установить файл PkgInfo eсли сборка предназначена для выполнения под MacOS / OS X
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
     install(FILES
       "${MISC_PATH}/PkgInfo"
       DESTINATION "${BUNDLE_CONTENT_PATH}"
     )   
endif()

# Установить найденные зависимости (включая свои библиотеки)
# в директорию для библиотек
install(DIRECTORY "${FOUND_DEPS_PATH}/"
  TYPE LIB
)

# Установить плюгины Qt (библиотеки)
foreach(PLUGIN ${QT_PLUGIN_LIST})
    # Получить список файлов плюгина
    file(GLOB QT_PLUGIN_FILES_LIST
      RELATIVE "${QT_PLUGIN_PATH}/${PLUGIN}"
      "${QT_PLUGIN_PATH}/${PLUGIN}/*${CMAKE_SHARED_LIBRARY_SUFFIX}"
    )

    # Установить файлы плюгина Qt
    foreach(FILE ${QT_PLUGIN_FILES_LIST})
        install(FILES "${QT_PLUGIN_PATH}/${PLUGIN}/${FILE}"
          DESTINATION "${INST_QTPLUG_DIR}/${PLUGIN}"
        )
        # Дополнить список поиска зависимостей
        list(APPEND DEP_FILES_LIST "${QT_PLUGIN_PATH}/${PLUGIN}/${FILE}")
    endforeach(FILE)
endforeach(PLUGIN)

# Для конфигураций Debug и RelWithDebInfo установить файлы
# отладочной информации к плюгинам Qt
foreach(PLUGIN ${QT_PLUGIN_LIST})

    # Получить список файлов *.debug плюгина (Linux)
    file(GLOB QT_PLUGIN_FILES_LIST
      RELATIVE "${QT_PLUGIN_PATH}/${PLUGIN}"
      "${QT_PLUGIN_PATH}/${PLUGIN}/*.debug"
    )
    # Установить найденные файлы
    foreach(FILE ${QT_PLUGIN_FILES_LIST})
        install(FILES "${QT_PLUGIN_PATH}/${PLUGIN}/${FILE}"
          CONFIGURATIONS Debug RelWithDebInfo
          DESTINATION "${INST_QTPLUG_DIR}/${PLUGIN}"
        )
    endforeach(FILE)

    # Получить список директорий *.dSYM плюгина (Mac OS)
    file(GLOB QT_PLUGIN_FILES_LIST
      RELATIVE "${QT_PLUGIN_PATH}/${PLUGIN}"
      "${QT_PLUGIN_PATH}/${PLUGIN}/*.dSYM"
    )
    # Установить найденные директории
    foreach(FILE ${QT_PLUGIN_FILES_LIST})
        install(DIRECTORY "${QT_PLUGIN_PATH}/${PLUGIN}/${FILE}"
          CONFIGURATIONS Debug RelWithDebInfo
          DESTINATION "${INST_QTPLUG_DIR}/${PLUGIN}"
        )
    endforeach(FILE)
endforeach(PLUGIN)

##############################################################################
# Обработка компонентов (библиотеки, скрипты, исполняемый файл)
##############################################################################

add_subdirectory(src/svgtoraster)

##############################################################################
# Упаковка
##############################################################################

include(scripts/CPpack.cmake)

##############################################################################
# Статический анализ исходных текстов при помощи PVS-Studio
##############################################################################

# Проверить наличие исполняемых файлов PVS-Studio
find_program(PVS_STUDIO_ANALYZER
    HINTS "C:/Program Files/PVS-Studio" "C:/Program Files (x86)/PVS-Studio" "D:/Program Files/PVS-Studio" "D:/Program Files (x86)/PVS-Studio"
    NAMES CompilerCommandsAnalyzer.exe pvs-studio-analyzer
)

find_program(PVS_STUDIO_CONVERTER
    HINTS "C:/Program Files/PVS-Studio" "C:/Program Files (x86)/PVS-Studio" "D:/Program Files/PVS-Studio" "D:/Program Files (x86)/PVS-Studio"
    NAMES HtmlGenerator.exe plog-converter
)

# BIN path                      path to pvs-studio-analyzer (Unix) or CompilerCommandsAnalyzer.exe (Windows)
# CONVERTER path                path to plog-converter (Unix) or HtmlGenerator.exe (Windows)

if ((EXISTS ${PVS_STUDIO_ANALYZER}) AND (EXISTS ${PVS_STUDIO_CONVERTER}))

    include(scripts/PVS-Studio.cmake)

    # Анализ приложения
    pvs_studio_add_target(TARGET analyze
        FORMAT errorfile-verbose
        ANALYZE "${PROJECT_NAME_LOWERCASE}"
        MODE GA;OP;CS;64
        LOG ${BUILD_PATH}/pvs_report.txt
#       CONVERTER_ARGS -d V112,V122,V550,V601,V2014
        BIN ${PVS_STUDIO_ANALYZER}
        CONVERTER ${PVS_STUDIO_CONVERTER}
    )

else()
    message(NOTICE "--- Программа PVS Studio не найдена: будет отсутствовать возможность статического анализа")
endif()

##############################################################################
# Статический анализ исходных текстов при помощи Clang-Tidy
##############################################################################

# Проверить наличие исполняемого файла Clang-Tidy
find_program(CLANG_TIDY_EXE
     NAMES "clang-tidy"
)

if (EXISTS ${CLANG_TIDY_EXE})

    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" -checks=-*, readability-*)

endif()

##############################################################################
# Создание файлов документации (HTML, PDF)
##############################################################################

# Использовать компонент для поиска Doxygen
include(FindDoxygen)

# Должны быть найдены doxygen и dot (начиная с CMAKE 3.9)
find_package(Doxygen QUIET COMPONENTS dot)

# Если doxygen найден, создать цели формирования файлов документации
if (DOXYGEN_FOUND)
    # Настройка переменных окружения Доксигена для основной цели
    set(DOXYGEN_PROJECT_NAME            "${CMAKE_PROJECT_NAME}")
    set(DOXYGEN_PROJECT_BRIEF           "Программа ${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_NUMBER          "${PROJECT_VERSION}")
    set(DOXYGEN_OUTPUT_DIRECTORY        "${DOC_PATH}")
    set(DOXYGEN_OUTPUT_LANGUAGE         "Russian")
    set(DOXYGEN_STRIP_FROM_PATH         "src/")
    set(DOXYGEN_TAB_SIZE                "2")
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C   YES)
    set(DOXYGEN_LOOKUP_CACHE_SIZE       2)
    set(DOXYGEN_NUM_PROC_THREADS        8)
    set(DOXYGEN_QUIET                   YES)
    set(DOXYGEN_EXTRACT_ALL             YES)
    set(DOXYGEN_EXTRACT_PRIVATE         YES)
    set(DOXYGEN_EXTRACT_PRIV_VIRTUAL    YES)
    set(DOXYGEN_EXTRACT_PACKAGE         YES)
    set(DOXYGEN_EXTRACT_STATIC          YES)
    set(DOXYGEN_HIDE_SCOPE_NAMES        YES)
    set(DOXYGEN_SORT_MEMBER_DOCS        NO)
    set(DOXYGEN_STRICT_PROTO_MATCHING   YES)
    set(DOXYGEN_WARN_NO_PARAMDOC        YES)
    set(DOXYGEN_WARN_LOGFILE            "${DOC_PATH}/warnings.log")
    set(DOXYGEN_FILE_PATTERNS           *.h *.hxx *.cpp *.md)
    set(DOXYGEN_EXAMPLE_PATH            "${SOURCES_PATH}")
#   set(DOXYGEN_IMAGE_PATH              ${DOXY_IMAGES})
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE  "${CMAKE_SOURCE_DIR}/README.md")
    set(DOXYGEN_ALPHABETICAL_INDEX      NO)
    set(DOXYGEN_HTML_STYLESHEET         "${CMAKE_SOURCE_DIR}/doc/stylesheet.css")
    set(DOXYGEN_GENERATE_TREEVIEW       YES)
    set(DOXYGEN_ENUM_VALUES_PER_LINE    1)
    set(DOXYGEN_HTML_FORMULA_FORMAT     svg)
    set(DOXYGEN_FORMULA_FONTSIZE        12)
    set(DOXYGEN_USE_MATHJAX             YES)
    set(DOXYGEN_MATHJAX_VERSION         MathJax_3)
    set(DOXYGEN_MATHJAX_FORMAT          chtml)
    set(DOXYGEN_MATHJAX_RELPATH         "../../../mathjax")
    set(DOXYGEN_MATHJAX_EXTENSIONS      require ams mathtools textcomp unicode)
    set(DOXYGEN_GENERATE_LATEX          YES)
    set(DOXYGEN_USE_PDFLATEX            YES)
    set(DOXYGEN_LATEX_CMD_NAME          ${CMD_RUN_LATEX})
    set(DOXYGEN_EXTRA_PACKAGES          "{amsmath, verbatimbox, svg}")
    set(DOXYGEN_LATEX_HEADER            "${CMAKE_SOURCE_DIR}/doc/header.tex")
    set(DOXYGEN_LATEX_EXTRA_STYLESHEET  "${CMAKE_SOURCE_DIR}/doc/doxygen.sty")
    set(DOXYGEN_LATEX_BATCHMODE         YES)
    set(DOXYGEN_MACRO_EXPANSION         YES)
    set(DOXYGEN_PREDEFINED              "__DOXYGEN__")
    set(DOXYGEN_SKIP_FUNCTION_MACROS    NO)
    set(DOXYGEN_HAVE_DOT                YES)
    set(DOXYGEN_INCLUDE_GRAPH           NO)
    set(DOXYGEN_INCLUDED_BY_GRAPH       NO)
    set(DOXYGEN_DIRECTORY_GRAPH         NO)
    set(DOXYGEN_DOT_IMAGE_FORMAT        svg)
    set(DOXYGEN_INTERACTIVE_SVG         YES)
    set(DOXYGEN_DOT_GRAPH_MAX_NODES     200)
    set(DOXYGEN_MAX_DOT_GRAPH_DEPTH     1000)
    set(DOXYGEN_VERBATIM_HEADERS        NO)

    # Формирование файлов документации html при помощи doxygen
    doxygen_add_docs(html_doc_proc
      "${CMAKE_SOURCE_DIR}/TODO.md"
      "${CMAKE_SOURCE_DIR}/README.md"
      "${CMAKE_SOURCE_DIR}/src"
#     WORKING_DIRECTORY ${SOURCES_PATH}
      COMMENT "Создание документации ${MAIN_NAME} в формате html"
    )

    # Удаление файла warnings.log, если он нулевой длины
    add_custom_target(html_doc
        # Команда удаления файла warnings.log нулевой длины
        ${CMAKE_COMMAND}
          -DFILENAME=warnings.log
          -P "${CMAKE_SOURCE_DIR}/scripts/remove_zerofile.cmake"
        WORKING_DIRECTORY ${DOC_PATH}
        COMMENT "Удаление файлов нулевой длины"
        VERBATIM
    )
    add_dependencies(html_doc html_doc_proc)

    # Формирование файла документации pdf при помощи doxygen
    add_custom_target(pdf_doc
        make ${CMD_TO_NUL}
        WORKING_DIRECTORY ${DOC_PATH}/latex
        COMMENT "Создание документации ${MAIN_NAME} в формате pdf"
    )
    add_dependencies(pdf_doc html_doc)
endif(DOXYGEN_FOUND)
