##############################################################################
# Информация о проекте
##############################################################################

# Имя, версия, описание проекта
set(CUSTOM_PROJECT_NAME "SvgToRaster")
set(CUSTOM_VERSION "1.0")
set(CUSTOM_DESCRIPTION "Простое преобразование файла формата SVG в файл растрового формата на основе возможностей Qt")
set(CUSTOM_HOMEPAGE_URL "https://github.com/GeorgKZ/SvgToRaster")
set(CUSTOM_LANGUAGES CXX)

# Поставщик установочного пакета
set(CUSTOM_PROJECT_VENDOR "GeorgKZ")

# Контактные данные поставщика установочного пакета
set(CUSTOM_PROJECT_MAINTAINER "${PROJECT_VENDOR} https://github.com/GeorgKZ")

# Путь к файлу значка SVG проекта
#set(CUSTOM_SVG_ICON_FILE "${CMAKE_SOURCE_DIR}/icons/icon.svg")

# Список компонентов проекта с путями, которые включаются как поддиректории
set(PROJECT_MODULES_LIST
  "svgtoraster"
)

# Путь к директории с изображениями для Doxygen
#set(CUSTOM_IMAGES "${CMAKE_SOURCE_DIR}/doc")

# Использовать MathJax, использовать расширения
#set(CUSTOM_MATHJAX require ams mathtools textcomp unicode)

# Аргументы анализатора PVS-Studio
#set(CUSTOM_PVS_ARGS "-d V112,V122,V550,V601,V2014")

##############################################################################
# Настройки Qt
##############################################################################

# Список необходимых для приложения плюгинов
set(QT_PLUGIN_LIST
  "iconengines"
  "imageformats"
  "platforms"
)

# Подсистемы Qt, для которых требуются переводы
set(QT_SYSTEMS "qt;qtbase;qtmultimedia")

# Список необходимых для приложения модулей Qt
set(QT_MODULES_LIST
  "Core"                 
  "UiTools"              
)

# Настройка языков локализации Qt:
# английский, русский
set(QT_LANG "en_US;ru_RU")
