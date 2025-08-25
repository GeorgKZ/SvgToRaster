##############################################################################
# Информация о компоненте
##############################################################################

# Имя компонента по имени директории, содержащей компонент
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR STEM LAST_ONLY MAIN_NAME)

# Вывод информации о собираемом компоненте
message(STATUS "##############################################################################")
message(STATUS "Проект '${CMAKE_PROJECT_NAME}', компонент '${MAIN_NAME}'")
message(STATUS "##############################################################################")

# RPATH, который будет использован в Mac OS/OS X на этапе установки
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib/${PROJECT_NAME_LOWERCASE}/lib")

##############################################################################
# Директории с файлами, необходимыми для сборки
##############################################################################

# Путь к директории с исходными текстами
set(SOURCES_PATH "${CMAKE_SOURCE_DIR}/src/${MAIN_NAME}")

# Список заголовков исходных текстов приложения
file(GLOB SOURCE_HEADERS_LIST
    ${SOURCES_PATH}/*.h
)

# Список файлов c++ исходных текстов приложения
file(GLOB SOURCE_FILE_LIST
    ${SOURCES_PATH}/*.cpp
)

# Определения с номером версии
add_definitions(
    -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DVERSION_TWEAK=${PROJECT_VERSION_TWEAK}
    -DVERSION_STR="${PROJECT_VERSION}"
    -DPROJECT_NAME="${PROJECT_NAME}"
    -DREL_PLUG_DIR="${REL_PLUG_DIR}"
    -DREL_LIC_DIR="${REL_LIC_DIR}"
)

##############################################################################
# Настройка ресурсов в зависимости от целевой среды выполнения.
##############################################################################

# При сборке для работы в среде Windows
if ((CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "MSYS"))

    # Строка ресурса, содержащая путь к файлу значка ICO
    set(ICON_RESOURCE_STRING "IDI_ICON1 ICON DISCARDABLE \"${ICON_FILE}\"\n\n")

    # Создать файл ресурса по шаблону
    file(WRITE
        ${MISC_PATH}/${MAIN_NAME}.rc

       "${ICON_RESOURCE_STRING}"
        "#include <windows.h>\n"
        "\n"
        "#define VER_FILEVERSION          ${PROJECT_VERSION_MAJOR}, ${PROJECT_VERSION_MINOR}, ${PROJECT_VERSION_PATCH}\n"
        "#define VER_FILEVERSION_STR      \"${PROJECT_VERSION}\"\n"
        "#define VER_PRODUCTVERSION       VER_FILEVERSION\n"
        "#define VER_PRODUCTVERSION_STR   VER_FILEVERSION_STR\n"
        "#define VER_FILEDESCRIPTION_STR  \"${PROJECT_DESCRIPTION} ${TARGET_BIT_WIDTH}-bit\"\n"
        "#define VER_INTERNALNAME_STR     \"${MAIN_NAME}\"\n"
        "#define VER_LEGALCOPYRIGHT_STR   \"\"\n"
        "#define VER_ORIGINALFILENAME_STR \"${MAIN_NAME}.exe\"\n"
        "#define VER_PRODUCTNAME_STR      \"${CMAKE_PROJECT_NAME} ${TARGET_BIT_WIDTH}-bit for Win7, Win8, Win10, Win11\"\n"
        "\n"
        "VS_VERSION_INFO VERSIONINFO\n"
        "FILEVERSION     VER_FILEVERSION,0\n"
        "PRODUCTVERSION  VER_PRODUCTVERSION,0\n"
        "BEGIN\n"
        "    BLOCK \"StringFileInfo\"\n"
        "    BEGIN\n"
        "        BLOCK \"041904E3\"\n"
        "        BEGIN\n"
        "            VALUE \"FileDescription\",    VER_FILEDESCRIPTION_STR \"\\0\"\n"
        "            VALUE \"FileVersion\",        VER_FILEVERSION_STR \"\\0\"\n"
        "            VALUE \"InternalName\",       VER_INTERNALNAME_STR \"\\0\"\n"
        "            VALUE \"LegalCopyright\",     VER_LEGALCOPYRIGHT_STR \"\\0\"\n"
        "            VALUE \"OriginalFilename\",   VER_ORIGINALFILENAME_STR \"\\0\"\n"
        "            VALUE \"ProductName\",        VER_PRODUCTNAME_STR \"\\0\"\n"
        "            VALUE \"ProductVersion\",     VER_PRODUCTVERSION_STR \"\\0\"\n"
        "        END\n"
        "    END\n"
        "\n"
        "    BLOCK \"VarFileInfo\"\n"
        "    BEGIN\n"
        "        VALUE \"Translation\", 0x419, 1251\n"
        "    END\n"
        "END"
    )

    # Путь к файлу ресурсов для сборки исполняемого файла Windows
    set(RESOURCES_DEP "${MISC_PATH}/${MAIN_NAME}.rc")
#    cmake_path(NATIVE_PATH LOCAL_RESOURCES_DEP NORMALIZE RESOURCES_DEP)

    # Добавить флаги для сборки консольного приложения Win32
    add_definitions(
        -DWIN32
        -D_CONSOLE
        -D_UNICODE
        -DUNICODE
    )
endif()

##############################################################################
# Определение исполняемого файла
##############################################################################

# Требуемые компоненты Qt6
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS OpenGL REQUIRED)
find_package(Qt6 COMPONENTS SerialPort REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED)
find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS UiTools REQUIRED)
find_package(Qt6 COMPONENTS Quick REQUIRED)
find_package(Qt6 COMPONENTS Multimedia REQUIRED)
find_package(Qt6 COMPONENTS MultimediaWidgets REQUIRED)

# Сформировать список файлов с описанием интерфейса
file(GLOB UI "${SOURCES_PATH}/*.ui")

# Зависимости исполняемого файла приложения
qt_add_executable(${MAIN_NAME}
    MACOSX_BUNDLE
    ${SOURCE_FILE_LIST}
    ${SOURCE_HEADERS_LIST}
    ${UI}
    ${RESOURCES_DEP}
)

# Директории с включаемыми файлами
target_include_directories(${MAIN_NAME}
    PRIVATE ${SOURCES_PATH}
)

# Добавить необходимые библиотеки Qt
target_link_libraries(${MAIN_NAME} PUBLIC
    Qt6::Core
#    Qt6::Gui
#    Qt6::OpenGL
#    Qt6::SerialPort
#    Qt6::Qml
#    Qt6::OpenGLWidgets
#    Qt6::Widgets
    Qt6::UiTools
#    Qt6::Quick
#    Qt6::Multimedia
#    Qt6::MultimediaWidgets
)

##############################################################################
# Настройка ресурса с файлами переводов
##############################################################################

# Подключить подсистему лингвистики Qt
find_package(Qt6 COMPONENTS LinguistTools REQUIRED)

# Добавить файлы переводов в список файлов TS_FILES
#file(GLOB TS_FILES "${SOURCES_PATH}/translations/${MAIN_NAME}_*.ts")

# Обработать файлы перевода без интеграции в исполняемый файл
#qt6_add_translations(${MAIN_NAME}
#    TS_FILES ${TS_FILES}
#    QM_FILES_OUTPUT_VARIABLE QM_FILES
#    LUPDATE_OPTIONS -silent -source-language en_US
#    LRELEASE_OPTIONS -silent
#)

# Обработать файлы перевода приложения для интеграции в исполняемый файл,
# перебрать все поддерживаемые языки
set(QM_FILES "")
FOREACH(L ${QT_LANG})
    string(SUBSTRING ${L} 0 2 LANG_F)

    # Обновить файл перевода
    set(TS_FILE "${SOURCES_PATH}/translations/${MAIN_NAME}_${LANG_F}.ts")
    set(TS_SOURCES ${SOURCE_FILE_LIST} ${SOURCE_HEADERS_LIST} ${UI})
    add_custom_command(
        OUTPUT ${TS_FILE}
        COMMAND ${LUPDATE_CMD} ${TS_SOURCES} -silent -source-language en_US -target-language ${L} -ts ${TS_FILE}
        COMMENT "lupdate ${TS_FILE}"
        DEPENDS ${TS_SOURCES}
    )

    # Скомпилировать файл перевода
    set(QM_FILE "${TRANSLATE_PATH}/${MAIN_NAME}_${LANG_F}.qm")
    add_custom_command(
        OUTPUT ${QM_FILE}
        COMMAND ${LRELEASE_CMD} ${TS_FILE} -silent -qm ${QM_FILE}
        COMMENT "lrelease ${TS_FILE}"
        DEPENDS ${TS_FILE}
    )

    add_custom_target("translation_${MAIN_NAME}__${LANG_F}" ALL
        DEPENDS ${QM_FILE}
    )
    list(APPEND QM_FILES ${QM_FILE})
ENDFOREACH(L)

# Добавить переводы-ресурсы к ресурсам исполняемого файла
qt6_add_resources(${MAIN_NAME} "translations"
    PREFIX "/translations"
    BIG_RESOURCES
    BASE ${TRANSLATE_PATH}
    FILES ${QM_FILES}
)

# Определить расположение файлов перевода
if(DEFINED ENV{QT_ROOT})
    set(QT_TBASE "${QT_DIR}/../../../translations")
else()
    set(QT_TBASE "/usr/share/qt6/translations")
endif()

# Перебрать поддерживаемые языки
foreach(L ${QT_LANG})
    string(SUBSTRING ${L} 0 2 LANG_F)
    # Перебрать необходимые файлы перевода
    foreach(I ${QT_SYSTEMS})
        # Добавить файл перевода к списку, установить ему алиас
        if(EXISTS "${QT_TBASE}/${I}_${LANG_F}.qm")
            list(APPEND QM_SYSTEM_FILES "${QT_TBASE}/${I}_${LANG_F}.qm")
        endif()
    endforeach(I)
endforeach(L)

# Добавить системные переводы-ресурсы к ресурсам исполняемого файла
qt6_add_resources(${MAIN_NAME} "system_translations"
  PREFIX "/system_translations"
  BIG_RESOURCES
  BASE ${QT_TBASE}
  FILES ${QM_SYSTEM_FILES}
)

##############################################################################
# Настройка свойств, зависимых от целевой среды выполнения.
##############################################################################

set_target_properties(${MAIN_NAME} PROPERTIES
  WIN32_EXECUTABLE TRUE
  MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_INFO_PLIST "${SOURCES_PATH}/Info.plist.in"
  MACOSX_BUNDLE_GUI_IDENTIFIER "com.github.GeorgKZ.${PROJECT_NAME}"
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE_ICON_FILE "${PROJECT_NAME_LOWERCASE}.icns"
)

##############################################################################
# Формирование набора зависимостей
##############################################################################

# Добавить цель (исполняемый файл) в список файлов, для которых будут найдены зависимости
list(APPEND DEP_FILES_LIST "${FIND_DEPS_PATH}/${MAIN_NAME}${CMAKE_EXECUTABLE_SUFFIX}")

# Сохранить файл со приском файлов, для которых будут найдены зависимости
file(WRITE "${MISC_PATH}/dep_src.txt" "${DEP_FILES_LIST}")

# Подготовка набора динамических библиотек, от которых зависит цель
add_custom_target(get_deps_${MAIN_NAME} ALL
            ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${MAIN_NAME}>" "${FIND_DEPS_PATH}/"
    COMMAND ${CMAKE_COMMAND}
      -DARGUMENT=dep_src.txt
      -DROOT_DIR="${CMAKE_SOURCE_DIR}"
      -DMISC_PATH="${MISC_PATH}"
      -DFOUND_DEPS_PATH="${FOUND_DEPS_PATH}"
      -DFIND_DEPS_PATH="${FIND_DEPS_PATH}"
      -DQT_DIR="${QT_DIR}"
      -P "${CMAKE_SOURCE_DIR}/scripts/collect_deps.cmake"
    COMMENT "Подготовка зависимостей цели ${MAIN_NAME}"
)
add_dependencies(get_deps_${MAIN_NAME} ${MAIN_NAME})

##############################################################################
# Формирование установочного набора
##############################################################################

# Вывод сообщений об установке только если было изменение
set(CMAKE_INSTALL_MESSAGE LAZY)

# Исполняемый файл
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    install(TARGETS ${MAIN_NAME}
      RUNTIME
      BUNDLE DESTINATION .
    )

else()

    install(TARGETS ${MAIN_NAME}
      RUNTIME
)

endif()
